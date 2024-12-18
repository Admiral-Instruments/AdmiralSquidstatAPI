
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include <QCoreApplication>
#include <qdebug.h>
#include <qtimer.h>

#define SQUIDSTAT_NAME "Cycler1460"

int main()
{

    char** test = nullptr;

    int args;
    QCoreApplication a(args, test);

    auto connectSignals = [=](const AisInstrumentHandler* handler) {
        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "channel: " << (int)channel << "current :" << data.current << "   voltage: " << data.workingElectrodeVoltage << "   counter electrode : " << data.counterElectrodeVoltage << "  timestamp : " << data.timestamp;
        });

        QObject::connect(handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData&) {

        });

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode&) {
            qDebug() << "New element starting";
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel) {
            qDebug() << "Experiment Stopped Signal " << channel;
        });
    };

    auto tracker = AisDeviceTracker::Instance();
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, &a, [=](const QString& deviceName) {
        qDebug() << deviceName << "is connected ";
    });

    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, &a, [=](const QString& deviceName) {
        qDebug() << deviceName << "is disconnected ";
    });

    auto connectdevices = tracker->connectAllPluggedInDevices();
    auto& handler = tracker->getInstrumentHandler(SQUIDSTAT_NAME);
    //If number of channels is -1, the device was not found.
    if (handler.getNumberOfChannels() == -1) {
        qDebug() << "Unable to connect to" << SQUIDSTAT_NAME << ". Ensure that it is connected and powered on. If the problem persists, run 'FirmwareUpdateDemo' to update all connected Squidstats.";
        return(-1);
    }

    connectSignals(&handler);

    auto error = handler.startManualExperiment(1);
    if (error) {
        qDebug() << "Manual Experiment is not start. Error is generated.";
        qDebug() << error.message();
    }

    // set sampling interval 2 s on channel 1.
    handler.setManualModeSamplingInterval(1, 2);

    // set 2 V on channel 1.
    handler.setManualModeConstantVoltage(1, 2);

    // after 25 s Manual Experiment is stop.
    QTimer::singleShot(25000, [=, &handler]() { handler.stopExperiment(1); });

    a.exec();
}
