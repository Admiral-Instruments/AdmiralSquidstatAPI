/*
* This demo assumes familiarity with 1_BasicDemo.cpp.
* This demo shows how to output incoming data to CSV files.
*/

#include "AisExperiment.h"
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"
#include <QCoreApplication>
#include <QTimer>
#include <qdebug.h>
#include <qfile.h>
#include <qstandardpaths.h>



int main()
{
    // Environment Setup
    char** test = nullptr;
    int args;
    QCoreApplication a(args, test); // this creates a non-GUI Qt application

    AisConstantPotElement cvElement( // constructing a constant potential element
        1, // voltage: 1v
        1, // sampling interval: 1s
        30 // duration: 30s
    );

    // We create another element of a different type
    AisConstantCurrentElement ccElement( // construct a constant current element and assign it to "ccElement"
        0.001, // current: 1mA
        1, // sampling interval: 1s
        60 // duration: 60s
    );

    // We create a custom experiment and add the elements to it
    auto customExperiment = std::make_shared<AisExperiment>(); // at this point, it is an empty custom experiment, so, we add the elements we created to it.
    customExperiment->appendElement(ccElement, 1); // append the CC element to the end of the experiment and set it to run 1 time
    customExperiment->appendElement(cvElement, 1); // append the CV element to the end of the experiment and set it to run 1 time

    QString filePath;
    // Lambda function: https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp

    auto createLogic = [=, &filePath](const AisInstrumentHandler* handler) {

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=, &filePath](uint8_t channel, const AisExperimentNode& node) {
            auto utcTime = handler->getExperimentUTCStartTime(0);
            auto name = "/" + QString::number(node.stepNumber) + " " + node.stepName + " " + QString::number(utcTime) + ".csv";
            filePath = (QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)) + name);

            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&file);
            out << "Time Stamp,"
                << "Counter Electrode Voltage,"
                << "Working Electrode Voltage,"
                << "Current"
                << "\n";
            file.close();

            qDebug() << "New element beginning: " << node.stepName << "step: " << node.stepNumber;
        });

        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady, [=, &filePath](uint8_t channel, const AisDCData& data) {
            qDebug() << "current :" << data.current << "   voltage: " << data.workingElectrodeVoltage << "   counter electrode : " << data.counterElectrodeVoltage << "  timestamp : " << data.timestamp;

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

        QObject::connect(handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData& data) {
            qDebug() << data.frequency << "         " << data.absoluteImpedance << "        " << data.phaseAngle;
            // you may output the AC data to a CSV file in the same manner as shown for the DC data
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel) {
            qDebug() << "Experiment Completed Signal " << channel;
        });
    };

    // this is a signal-slot connection where the slot assigns the logic to the device handler when newDeviceConnected signal is emitted.
    auto tracker = AisDeviceTracker::Instance(); // create a tracker that tracks connected devices
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, &a, [&](const QString& deviceName) {
        auto& handler = tracker->getInstrumentHandler(deviceName);
        createLogic(&handler); // controlling experiments is to be done only after a device handler has been assigned. That is why it is placed inside this slot.

        auto error = handler.uploadExperimentToChannel(0, customExperiment); // upload to a specific channel (first arg) an experiment (second arg) on the given device
        if (error) {
            qDebug() << error.message();
            return;
        }
        error = handler.startUploadedExperiment(0); // start the previously uploaded experiment on a specific channel. The argument is the channel number
        if (error) {
            qDebug() << error.message();
            return;
        }
    });

    // here we have a signal and slot for when a device has been disconnected
    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, &a, [=](const QString& deviceName) {
        qDebug() << deviceName << "is disconnected ";
    });

    //tracker->connectToDeviceOnComPort("COM5"); // change the port number to your device. For example, in windows, you can find it from the device manager
    int connectedDeviceNum = tracker->connectAllPluggedInDevices();
    if (connectedDeviceNum == 0) {
        qDebug() << "No devices were found. Ensure that all Squidstats are connected and powered on. If the problem persists, run 'FirmwareUpdateDemo' to update all connected Squidstats.";
    }

    a.exec();
}
