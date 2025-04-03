/**
 * \example pulseData.cpp
 * This example demonstrates how to use the `AisDataManipulator` class to calculate advanced parameters from raw DC data recieved from pulse experiments.
 */
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "AisExperiment.h"
#include "AisDataManipulator.h"

#include "experiments/builder_elements/AisDiffPulseVoltammetryElement.h"

#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

// Define relevant device information, for easy access
#define COMPORT "COM1"
#define CHANNEL 0

int main()
{
    char** test = nullptr;
    int args;
    QCoreApplication a(args, test);
    auto tracker = AisDeviceTracker::Instance();

    // Create the AisDiffPulseVoltammetryElement element and add it to the experiment
    std::shared_ptr<AisExperiment> experiment = nullptr;
    AisDiffPulseVoltammetryElement dpv_element(-0.4, 0.5, 0.01, 0.1, 0.1, 0.45);
    dpv_element.setStartVoltageVsOCP(false);
    dpv_element.setEndVoltageVsOCP(false);
    dpv_element.setApproxMaxCurrent(0.001);
    experiment = std::make_shared<AisExperiment>();
    experiment->appendElement(dpv_element, 1);

     // Create an `AisDataManipulator` class for calculating advance parameters.
    std::shared_ptr<AisDataManipulator> dataManipulator = std::make_shared<AisDataManipulator>();
    dataManipulator->setPulseType(AisPulseType::DifferentialPulse, dpv_element.getPulseWidth(), dpv_element.getPulsePeriod());

    auto connectSignals = [=](const AisInstrumentHandler* handler) {
        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            auto utcTime = handler->getExperimentUTCStartTime(0);

            dataManipulator->loadPrimaryData(data);

            if (dataManipulator->isPulseCompleted()) {
                static bool writeheader = true;

                if (writeheader) {

                    qDebug() << "time"
                             << ", Pulse_current"
                             << ", base_current"
                             << ", diff_current"
                             << ", base_voltage"
                             << ", pulse_volatge";
                    writeheader = false;
                }
                qDebug() << data.timestamp << ", " << dataManipulator->getPulseCurrent() << ", " << dataManipulator->getBaseCurrent() << ", " << dataManipulator->getPulseCurrent() - dataManipulator->getBaseCurrent() << ", " << dataManipulator->getBaseVoltage() << "," << dataManipulator->getPulseVoltage();
            }
        });

        QObject::connect(handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData& data) {
            qDebug() << "channel: " << (int)channel << "frequency :" << data.frequency << "   absoluteImpedance: " << data.absoluteImpedance << "   phaseAngle : " << data.phaseAngle << "  timestamp : " << data.timestamp;
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode& data) {
            qDebug() << "New Node beginning " << data.stepName << " step number  " << data.stepNumber << " step sub : " << data.substepNumber;
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel, const QString& reason) {
            qDebug() << "Experiment Completed Signal " << channel << "Reason : " << reason;
        });
        QObject::connect(handler, &AisInstrumentHandler::experimentPaused, [=](uint8_t channel) {
            qDebug() << "Experiment Paused " << channel;
        });
        QObject::connect(handler, &AisInstrumentHandler::experimentResumed, [=](uint8_t channel) {
            qDebug() << "Experiment Resume " << channel;
        });
    };

    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, &a, [=](const QString& deviceName) {
        auto& handler = tracker->getInstrumentHandler(deviceName);
        connectSignals(&handler);
        {
            std::shared_ptr<AisExperiment> deleteExp = std::make_shared<AisExperiment>(*experiment);
            auto error = handler.uploadExperimentToChannel(CHANNEL, deleteExp);
            if (error) {
                qDebug() << "Error: " << error.message();
            }
        }
        auto error = handler.startUploadedExperiment(0);
        if (error) {
            qDebug() << "Error: " << error.message();
        }
    });

    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, &a, [=](const QString& deviceName) {
        qDebug() << deviceName << "is disconnected ";
    });

    auto error = tracker->connectToDeviceOnComPort(COMPORT);
    if (error) {
        qDebug() << "Error: " << error.message();
    }

    a.exec();
}
