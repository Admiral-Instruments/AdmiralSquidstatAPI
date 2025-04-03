/**
 * \example nonblockingExperiment.cpp 
 * This non blocking example show how we can start and run an experiment, while still being able to process other events.
 * Say there is other logic that needs to be processed while an experiment is running, this example shows how we can continue to process events while our experiment and relavant logic is running.
 * The Admiral Instruments API gives more control of the device and gives you the tools to integrate running
 * our experiments in your pipeline and automating your workflow.
 * Our API lets you programmatically start an experiment, pause an experiment and stop an experiment.
 * For example, you may want to start an experiment, and stop after certain time, and exit the program. 
 *
 * Below is an example for start a manual experiment, stop after 25 s, and exit the program after. 
 *
 */

#include "AisExperiment.h"
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

// Define relevant device information, for easy access
#define COMPORT "COM1"
#define CHANNEL 0
#define INSTRUMENT_NAME "Plus2001"

int main()
{
    char** test = nullptr;
    int args;
    QCoreApplication a(args, test);

    auto tracker = AisDeviceTracker::Instance();

    auto createLogic = [=] (const AisInstrumentHandler* handler) {
        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady,  [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "Timestamp : " << data.timestamp << "  Current :" << data.current << "  Voltage: " << data.workingElectrodeVoltage << "  CE Voltage: " << data.counterElectrodeVoltage ;
        });

        QObject::connect(handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData& data) {
            qDebug() << "Frequency: " << data.frequency << "  Absolute Impedance:" << data.absoluteImpedance << "  Phase Angle" << data.phaseAngle;
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode& nodeInfo) {
            qDebug() << "New Node beging ";
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel, const QString& reason) {
            qDebug() << "Experiment Compleletd Signal " << channel << "Reason: " << reason;
        });
    };

    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, [=, &a](const QString& deviceName) {
        qDebug() << "New Device Connected: " << deviceName;

        auto& handler = tracker->getInstrumentHandler(INSTRUMENT_NAME);

        createLogic(&handler);

        auto error = handler.startManualExperiment(CHANNEL);
        if (error) {
            qDebug() << error.message();
            return;
        }
        error = handler.setManualModeSamplingInterval(CHANNEL, 2);
        if (error) {
            qDebug() << error.message();
            return;
        }

        error = handler.setManualModeConstantVoltage(CHANNEL, 2);
        if (error) {
            qDebug() << error.message();
            return;
        }
        QTimer::singleShot(25000, [=, &handler]() { 
            auto error = handler.stopExperiment(CHANNEL); 
            if (error) {
                qDebug() << error.message();
                return;
            }
        });

        // Allow the event loop to process other events while the current experiment is running
        // isChannelBusy will return true while the channel is running an experiment
        while (handler.isChannelBusy(CHANNEL)) {
            a.processEvents();
        }

        // Process any remaining events
        a.processEvents();
    });

    auto connectdevices = tracker->connectAllPluggedInDevices();
    if (connectdevices == 0)
    {
        qDebug() << "No devices connected";
        return 0;
    }

    return 0;
}
