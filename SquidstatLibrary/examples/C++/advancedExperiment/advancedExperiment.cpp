/** \example advancedExperiment.cpp */
#include "AisDeviceTracker.h"
#include "AisExperiment.h"
#include "AisInstrumentHandler.h"

#include "experiments/builder_elements/AisOpenCircuitElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"
#include "experiments/builder_elements/AisEISGalvanostaticElement.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"

#include <QCoreApplication>
#include <QDebug>

// Define relevant device information, for easy access
#define COMPORT "COM5"
#define CHANNEL 0
#define INSTRUMENT_NAME "PLus1366"

int main()
{
    char** test = nullptr;
    int args;

    QCoreApplication a(args, test);

    auto tracker = AisDeviceTracker::Instance();

    bool success = true;
    //! [BuildExperiment]
    auto customExperiment = std::make_shared<AisExperiment>();

    //! [Step1]
    AisOpenCircuitElement ocpElement(1, 10);
    success &= customExperiment->appendElement(ocpElement);
    //! [Step1]

    //! [Step2]
    int voltage = 0;
    for (int i = 0; i < 4; i++) {
        AisConstantPotElement cvElement(voltage, 0.1, 5);
        success &= customExperiment->appendElement(cvElement, 1);
        voltage += 0.1; // Adding 100 mV
    }
    //! [Step2]

    //! [Step3]
    AisExperiment eisSubExperiment;

    AisEISGalvanostaticElement galvEISElement(10, 10000, 10, 0.01, 0.1);
    AisOpenCircuitElement ocpElement2(1, 5);

    success &= eisSubExperiment.appendElement(galvEISElement, 1);
    success &= eisSubExperiment.appendElement(ocpElement2, 1);

    success &= customExperiment->appendSubExperiment(eisSubExperiment, 3);
    //! [Step3]

    //! [Step4]
    AisConstantCurrentElement ccElement(0.1, 1, 10);
    success &= customExperiment->appendElement(ccElement, 2);
    //! [Step4]
    
    if (!success) {
        qDebug() << "Error building experiment";
        return 0;
    }
    //! [BuildExperiment]
    
    auto connectSignals = [=](const AisInstrumentHandler& handler) {
        QObject::connect(&handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            qDebug() << "Timestamp: " << data.timestamp << " Current: " << data.current << " Voltage: " << data.workingElectrodeVoltage << " CE Voltage : " << data.counterElectrodeVoltage;
        });
        QObject::connect(&handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode& info) {
            qDebug() << "New element starting: " << info.stepName;
        });
        QObject::connect(&handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel, const QString& reason) {
            qDebug() << "Experiment Stopped Signal " << channel << "Reason : " << reason;
        });
        QObject::connect(&handler, &AisInstrumentHandler::deviceError, [=](uint8_t channel, const QString& error) {
            qDebug() << "Device Error: " << error;
        });
    };

    // When device is connected, setup connections, and upload/start the experiment
    //! [Start]
  	QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, [=](const QString& deviceName) {
        qDebug() << "New Device Connected: " << deviceName;
        auto& handler = tracker->getInstrumentHandler(INSTRUMENT_NAME);

        connectSignals(handler);

        AisErrorCode error = handler.uploadExperimentToChannel(CHANNEL, customExperiment);
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
    //! [Start]

    return a.exec();
}