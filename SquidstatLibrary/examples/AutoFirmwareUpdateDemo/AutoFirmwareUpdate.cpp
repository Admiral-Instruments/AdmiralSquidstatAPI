#include "AisInstrumentHandler.h"
#include "AisDeviceTracker.h"
#include "AisExperiment.h"

#include "experiments/builder_elements/AisOpenCircuitElement.h"

#include <QCoreApplication>
#include <QTimer>
#include <QThread>
#include <qdebug.h>

#define COMPORT "COM5"
#define CHANNEL 0

int main()
{

    int args;
    QCoreApplication a(args, nullptr);
    auto tracker = AisDeviceTracker::Instance();

    // Custom Experiment with one constant current element
    std::shared_ptr<AisExperiment> experiment = std::make_shared<AisExperiment>();
    AisOpenCircuitElement opencircuitElement(10,1);
    experiment->appendElement(opencircuitElement, 1);

    // This set up the signals and slots for each device that gets connected
    auto createLogic = [=](const AisInstrumentHandler* handler) {
        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            auto utcTime = handler->getExperimentUTCStartTime(0);

            qDebug() << "current :" << data.current << "   voltage: " << data.workingElectrodeVoltage << "   counter electrode : "
                << data.counterElectrodeVoltage << "  timestamp : " << data.timestamp
                << " start UTC time: " << qSetRealNumberPrecision(20) << utcTime;
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode& data) {
            qDebug() << "New Node beginning " << data.stepName << " step number  " << data.stepNumber << " step sub : " << data.substepNumber;
        });
        QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel) {
            qDebug() << "Experiment Completed on channel" << channel;
        });
    };

    // When a device is connected, create the signals and slots to print status messages, and then start the experiment.
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, &a, [=](const QString& deviceName) {
        auto& handler = tracker->getInstrumentHandler(deviceName);
        createLogic(&handler);
        handler.uploadExperimentToChannel(CHANNEL, experiment);
        qDebug() << "Starting experiment on" << deviceName << "channel" << CHANNEL+1;
        handler.startUploadedExperiment(CHANNEL);
    });

    // While a device is updating firmware, print out the messages.
    // When the update is complete, connect to the device, which will start the experiment
    QObject::connect(tracker, &AisDeviceTracker::firmwareUpdateNotification, [=](const QString& message) {
        qInfo() << message;
        if (message.contains("firmware is updated.")) {
            const int retryCount = 1;
            // Give the Squidstat some time to reconnect
            AisErrorCode error(AisErrorCode::ConnectionFailed);
            for (int i = 0; i < retryCount && error == error.ConnectionFailed; i++) {
                QThread::sleep(1); //Give the last Squidstat a moment to re-establish the comport
                error = tracker->connectToDeviceOnComPort(COMPORT);
            }
            if (error != error.Success) {
                qDebug() << "Error: " << error.message();
            }
        }
    });

    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, &a, [=](const QString& deviceName) {
        qDebug() << deviceName << "is disconnected ";
    });

    // Attempt to connect to the device just before starting the QT app.
    auto error = tracker->connectToDeviceOnComPort(COMPORT);
    if (error != error.Success) {
        if (error == error.FirmwareNotSupported) {
            qDebug() << "Firmware is out of date for the device on" << COMPORT;
            tracker->updateFirmwareOnComPort(COMPORT);
        }
        else {
            qDebug() << "Error: " << error.message();
        }
    }

    a.exec();
}
