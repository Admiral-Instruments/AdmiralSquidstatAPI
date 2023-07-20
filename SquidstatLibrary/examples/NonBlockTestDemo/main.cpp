/** @page NonBlockTestDemo
 *
 *  @verbinclude main.cpp
 */
/*
* The Admiral Instruments API gives more control of the device and gives you the tools to integrate running
* our experiments in your pipeline and automating your workflow.
* Our API lets you programmatically start an experiment, pause an experiment and stop an experiment.
* For example, you may want to start an experiment, and stop after certain time, and exit the program. 
*
* Below is an example for start a manual Experiment experiment, stop after 25 s, and exit the program after. 
*
*/

#include "AisExperiment.h"
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"
#include <QCoreApplication>
#include <qdebug.h>
#include <qtimer.h>

#define SQUIDSTAT_NAME "Cycler1460"

int main()
{

    char** test = nullptr;
    int args;
    QCoreApplication a(args, test);

    auto createLogic = [=] (const AisInstrumentHandler* handler) {
        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady,  [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "channel : " << channel <<" current :" << data.current << "   voltage: " << data.workingElectrodeVoltage
                << "   counter electrode : " << data.counterElectrodeVoltage << "  timestamp : " << data.timestamp;
        });

        QObject::connect(handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData& data) {
            qDebug() << data.frequency << "         " << data.absoluteImpedance << "        " << data.phaseAngle;
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode&) {
            qDebug() << "New Node beging ";
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentStopped,  [=](uint8_t channel) {
            qDebug() << "Experiment Compleletd Signal " << channel;
        });
    };

    auto tracker = AisDeviceTracker::Instance();
    auto connectdevices  =  tracker->connectAllPluggedInDevices();
    auto listofDevices = tracker->getConnectedDevices();
    auto& handler = tracker->getInstrumentHandler(SQUIDSTAT_NAME);
    //If number of channels is -1, the device was not found.
    if (handler.getNumberOfChannels() == -1) {
        qDebug() << "Unable to connect to" << SQUIDSTAT_NAME << ". Ensure that it is connected and powered on. If the problem persists, run 'FirmwareUpdateDemo' to update all connected Squidstats.";
        return(-1);
    }

    createLogic(&handler);

    auto error = handler.startManualExperiment(0);
    handler.setManualModeSamplingInterval(0, 2);

    handler.setManualModeConstantVoltage(0, 2);
    QTimer::singleShot(25000, [=,&handler]() { handler.stopExperiment(0); });


    while (handler.isChannelBusy(1) || handler.isChannelBusy(0)) {
        a.processEvents();
    }

    a.processEvents();
    qDebug() << "Program is not block by any thread.";
}
