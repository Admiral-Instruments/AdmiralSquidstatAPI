/** 
 * \example linkedChannels.cpp 
 * This example will show you how linked channels can be used to combine the multiple channels on a device, in order to amplify the output for a single experiment.
 * AisInstrumentHandler::setLinkedChannels MUST be called before each experiment that uses paralleled channels.
 * Once linked, these channels must be controlled by ONLY the master channel, which is returned by the AIsInstrumentHandler::setLinkedChannels function.
 * 
 * @note This feature is only available on Cycler models.
 */
#include "AisDeviceTracker.h"
#include "AisExperiment.h"
#include "AisInstrumentHandler.h"

#include "experiments/builder_elements/AisConstantCurrentElement.h"

#include <QCoreApplication>
#include <QDebug>

// Define relevant device information, for easy access
#define COMPORT "COM1"

int main()
{
    char** test = nullptr;
    int args;

    QCoreApplication a(args, test);

    auto tracker = AisDeviceTracker::Instance();

    // Create an experiment
    AisConstantCurrentElement ccElement(10, 1, 30);
    auto customExperiment = std::make_shared<AisExperiment>();
    customExperiment->appendElement(ccElement, 1);

    auto connectSignals = [=](const AisInstrumentHandler& handler) {
        QObject::connect(&handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "Timestamp: " << data.timestamp << " Current: " << data.current << " Voltage: " << data.workingElectrodeVoltage << " CE Voltage : " << data.counterElectrodeVoltage;
        });
        QObject::connect(&handler, &AisInstrumentHandler::deviceError, [=](uint8_t channel, const QString& error) {
            qDebug() << "Device Error: " << error;
        });
    };

    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, [=](const QString& deviceName) {
        qDebug() << "New Device Connected: " << deviceName;
        
        auto& handler = tracker->getInstrumentHandler(deviceName);

        // Here we want to link channels 0 and 1 together, so we pass in a vector of the channels to link
        // It will return which of the channels is the master channel, this should be used to control the experiment
        int8_t masterChannel = handler.setLinkedChannels({ 0, 1 });

        connectSignals(handler);

        AisErrorCode error = handler.uploadExperimentToChannel(masterChannel, customExperiment);
        if (error) {
            qDebug() << error.message();
        }

        // Start the previously uploaded experiment on the master channel
        error = handler.startUploadedExperiment(masterChannel);
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