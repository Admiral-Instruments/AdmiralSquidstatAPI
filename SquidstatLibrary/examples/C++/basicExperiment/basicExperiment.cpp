/** \example basicExperiment.cpp */
//! [Setup]
#include "AisDeviceTracker.h"
#include "AisExperiment.h"
#include "AisInstrumentHandler.h"

#include "experiments/builder_elements/AisConstantPotElement.h"

#include <QCoreApplication>
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
    //! [Setup]

    //! [Experiment]
    //       Voltage = 1V, Sampling Interval = 1s, Duration = 30s
    AisConstantPotElement cvElement(1, 1, 30);

    //! Create empty experiment, using <a href="">shared_ptr </a> to not worry about lifetime/memory management
    auto customExperiment = std::make_shared<AisExperiment>();
    // Append the constant potential element, and tell it to execute that element 1 time
    customExperiment->appendElement(cvElement, 1);
    //! [Experiment]
    //! [SignalLambda]
    auto connectSignals = [=](const AisInstrumentHandler& handler) {
        //! [DataSignals]
        QObject::connect(&handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "Timestamp: " << data.timestamp << " Current: " << data.current << " Voltage: " << data.workingElectrodeVoltage << " CE Voltage : " << data.counterElectrodeVoltage;
        });
        QObject::connect(&handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData& data) {
            qDebug() << "Timestamp: " << data.timestamp << " Frequency: " << data.frequency << "" << data.absoluteImpedance;
        });
        //! [DataSignals]
        //! [HandlerSignals]
        // Whenever a new node in the element starts, note: some Ais Elements contain multiple logical nodes
        // i.e AisCyclicVoltammatryElement contains 4 nodes for each linear segment of each cycle plus a quiet time node if enabled
        // So this lambda would be executed atleast 4 times for each cycle
        QObject::connect(&handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode& info) {
            qDebug() << "New element starting: " << info.stepName;
        });
        // Whenever an experiment completes or is manually stopped, this will execute
        QObject::connect(&handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel, const QString& reason) {
            qDebug() << "Experiment Stopped Signal " << channel << "Reason : " << reason;
        });
        QObject::connect(&handler, &AisInstrumentHandler::deviceError, [=](uint8_t channel, const QString& error) {
            qDebug() << "Device Error: " << error;
        });
        //! [HandlerSignals]
    };
    //! [SignalLambda]
    //! [TrackerSignals]
    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, [=](const QString& deviceName) {
        qDebug() << "New Device Connected: " << deviceName;
    });
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, [=](const QString& deviceName) {
        qDebug() << "New Device Connected: " << deviceName;
        //! [TrackerSignals]
        //! [Start]
        auto& handler = tracker->getInstrumentHandler(deviceName);

        connectSignals(handler);

        AisErrorCode error = handler.uploadExperimentToChannel(CHANNEL, customExperiment);
        if (error) {
            qDebug() << error.message();
            return;
        }

        // Start the previously uploaded experiment on the same channel
        error = handler.startUploadedExperiment(CHANNEL);

        // Exit the application if there is any error starting the experiment
        if (error) {
            qDebug() << error.message();
            return;
        }
        //! [Start]
    });
    //! [ConnectToDevice]
    AisErrorCode error = tracker->connectToDeviceOnComPort(COMPORT);
    if (error != error.Success) {
        qDebug() << error.message();
        return 0;
    }
    //! [ConnectToDevice]

    // Returning a.axec() executes the event loop, which continues running until the application is exited
    return a.exec();
}