/**
* \example dataOutput.cpp
* This example shows how to capture data from the device, and write it to a CSV file using Qt's QFile and QTextStream classes.
*/

#include "AisExperiment.h"
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"

#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>

// Define relevant device information, for easy access
#define COMPORT "COM1"
#define CHANNEL 0

int main()
{
    char** test = nullptr;
    int args;
    QCoreApplication a(args, test);

    auto tracker = AisDeviceTracker::Instance(); 

    // Build the experiment
    AisConstantPotElement cvElement(
        1,      // voltage: 1v
        1,      // sampling interval: 1s
        30      // duration: 30s
    );
    AisConstantCurrentElement ccElement(
        0.001,  // current: 1mA
        1,      // sampling interval: 1s
        60      // duration: 60s
    );
    auto customExperiment = std::make_shared<AisExperiment>();
    customExperiment->appendElement(ccElement, 1);
    customExperiment->appendElement(cvElement, 1);

    // Static QString variable to store the current elements file path
    static QString filePath;

    // Create a lambda funnction to connect signals to the handler
    auto connectSignals = [=](const AisInstrumentHandler& handler) {
        QObject::connect(&handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode& node) {
            // Create a unique file name for each element, with the format "stepNumber_stepName_expStartTime.csv"
            static int fileNum = 1;
            auto name = "/" + QString::number(fileNum) + "_" + QString::number(node.stepNumber) + "_" + node.stepName + ".csv";
            filePath = QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)) + name;

            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            // Writing headers to file
            QTextStream out(&file);
            out << "Time Stamp,"
                << "Counter Electrode Voltage,"
                << "Working Electrode Voltage,"
                << "Current"
                << "\n";
            file.close();

            qDebug() << "New element beginning: " << node.stepName << "step: " << node.stepNumber;
        });

        QObject::connect(&handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "current :" << data.current << "   voltage: " << data.workingElectrodeVoltage << "   counter electrode : " << data.counterElectrodeVoltage << "  timestamp : " << data.timestamp;

            // Save the DC data to the file created at element beginning
            QFile file(filePath);
            if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
                return;
            QTextStream out(&file);
            out << data.timestamp << ","
                << data.counterElectrodeVoltage << ","
                << data.workingElectrodeVoltage << ","
                << data.current
                << "\n";
            file.close();
        });

        // The experiment created only uses DC elements, so this will not be executed.
        // However, ac data can be handled and written to a csv like seen above with dc data
        QObject::connect(&handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData& data) {
            qDebug() << data.frequency << "         " << data.absoluteImpedance << "        " << data.phaseAngle;
        });

        QObject::connect(&handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel, const QString& reason) {
            qDebug() << "Experiment Completed Signal " << channel << "Reason : " << reason;
        });
    };

    // When device is connected, setup connections, and upload/start the experiment
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, [=](const QString& deviceName) {
        auto& handler = tracker->getInstrumentHandler(deviceName);

        connectSignals(handler);

        AisErrorCode error = handler.uploadExperimentToChannel(0, customExperiment);
        if (error) {
            qDebug() << error.message();
            return 0;
        }

        error = handler.startUploadedExperiment(CHANNEL);
        if (error) {
            qDebug() << error.message();
            return 0;
        }
    });

    AisErrorCode error = tracker->connectToDeviceOnComPort(COMPORT);
    if (error != error.Success) {
        qDebug() << error.message();
        return 0;
    }

    return a.exec();
}