/** \example manualExperiment.cpp */
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"

#include <QCoreApplication>
#include <QDebug>

#include <QTimer> // For QTimer::singleShot

// Define relevant device information, for easy access
#define COMPORT "COM1"
#define CHANNEL 0

int main()
{
    char** test = nullptr;
    int args;

    QCoreApplication a(args, test);

    auto tracker = AisDeviceTracker::Instance();

    // Create a lambda function to connect signals to the handler
    auto connectSignals = [=](const AisInstrumentHandler& handler) {
        QObject::connect(&handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "Timestamp: " << data.timestamp << " Current: " << data.current << " Voltage: " << data.workingElectrodeVoltage << " CE Voltage : " << data.counterElectrodeVoltage;
        });
        QObject::connect(&handler, &AisInstrumentHandler::deviceError, [=](uint8_t channel, const QString& error) {
            qDebug() << "Device Error: " << error;
        });
        QObject::connect(&handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel, const QString& reason) {
            qDebug() << reason;
            qDebug() << "Experiment has ended. Closing application.";
            QCoreApplication::quit();
        });
    };

    // Create a lambda function to run the experiment
    auto runExperiment = [=](const AisInstrumentHandler& handler) {
        //! [Start]
        //The default starting mode is always Open Circuit Potential
        qDebug() << "Starting manual mode at open circuit potential";
        AisErrorCode error = handler.startManualExperiment(CHANNEL);
        if (error != AisErrorCode::Success) {
            qDebug() << error.message();
            QCoreApplication::quit();
        }
        //! [Start]

        //! [ManualModes]
        // In this section we connect singleshot QTimers to lambda functions that call our mode changing functions.
        // These lambdas are called asynchronously when the QTimers expire.

        // 5 seconds after starting experiment, change to Constant Current at .1A
        QTimer::singleShot(5000, [=, &handler]() {
            qDebug() << "Switching to constant current at .1A";
            AisErrorCode error = handler.setManualModeConstantCurrent(CHANNEL, .1);
            if (error != AisErrorCode::Success) {
                qDebug() << error.message();
            }
        });

        // 15 seconds after starting experiment, change to Constant Voltage at 1V
        QTimer::singleShot(15000, [=, &handler]() {
            qDebug() << "Switching to constant voltage at 1V";
            AisErrorCode error = handler.setManualModeConstantVoltage(CHANNEL, 1);
            if (error != AisErrorCode::Success) {
                qDebug() << error.message();
            }
        });

        // 25 seconds after starting experiment, change back into Open Circuit Potential mode.
        QTimer::singleShot(25000, [=, &handler]() {
            qDebug() << "Switching to open circuit potential";
            AisErrorCode error = handler.setManualModeOCP(CHANNEL);
            if (error != AisErrorCode::Success) {
                qDebug() << error.message();
            }
        });
        //! [ManualModes]

        //! [Stop]
        // Stop the experiment after 30 seconds
        QTimer::singleShot(30000, [=, &handler]() {
            qDebug() << "Stopping experiment";
            if (handler.stopExperiment(CHANNEL) != AisErrorCode::Success) {
                qDebug() << error.message();
            }
        });
        //! [Stop]
    };

    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, [=](const QString& deviceName) {
        qDebug() << "New Device Connected: " << deviceName;

        // When instrument is connected, grab the handler and setup/start the experiment
        auto& handler = tracker->getInstrumentHandler(deviceName);

        connectSignals(handler);
        runExperiment(handler);
    });

    AisErrorCode error = tracker->connectToDeviceOnComPort(COMPORT);
    if (error != error.Success) {
        qDebug() << error.message();
        return 0;
    }

    return a.exec();
}