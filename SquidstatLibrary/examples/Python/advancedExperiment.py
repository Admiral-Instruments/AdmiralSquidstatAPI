"""! @example advancedExperiment.py """
#! [Setup]
import sys
from PySide6.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker, AisExperiment, AisInstrumentHandler, AisErrorCode, AisOpenCircuitElement, AisConstantPotElement, AisConstantCurrentElement, AisEISGalvanostaticElement

#Define relavant device information, for easy access
COMPORT = "COM1"
CHANNEL = 0
INSTRUMENT_NAME = "Plus2000"

app = QApplication()

tracker = AisDeviceTracker.Instance()

success = True
##! [BuildExperiment]
customExperiment = AisExperiment()
# Step 1
##! [Step1]
ocpElement = AisOpenCircuitElement(1, 10)
success &= customExperiment.appendElement(ocpElement)
##! [Step1]
# Step 2
##! [Step2]
voltage = 0
for i in range(0, 4):
    cvElement = AisConstantPotElement(voltage, 0.1, 5)
    success &= customExperiment.appendElement(cvElement, 1)
    voltage = voltage + 0.1
##! [Step2]
# Step 3
##! [Step3]
eisSubExperiment = AisExperiment()
    
galvEISElement = AisEISGalvanostaticElement(10, 10000, 10, 0.01, 0.1)
ocpElement2 = AisOpenCircuitElement(1, 5)

success &= eisSubExperiment.appendElement(galvEISElement, 1)
success &= eisSubExperiment.appendElement(ocpElement2, 1)

success &= customExperiment.appendSubExperiment(eisSubExperiment, 3)
##! [Step3]
# Step 4
##! [Step4]
ccElement = AisConstantCurrentElement(0.1, 1, 10)
success &= customExperiment.appendElement(ccElement, 2)
##! [Step4]
if not success:
    print("Error building experiment")
    sys.exit()
##! [BuildExperiment]

# When device is connected, setup connections, and upload/start the experiment
def connectSignals(handler):
    handler.activeDCDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Current: {data.current} Voltage: {data.workingElectrodeVoltage} CE Voltage : {data.counterElectrodeVoltage}"))
    handler.activeACDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Frequency: {data.frequency} Absolute Impedance: {data.absoluteImpedance}"))
    handler.experimentNewElementStarting.connect(lambda channel, info: print(f"New element starting: {info.stepName}"))
    handler.experimentStopped.connect(lambda channel, reason: (print(f"Experiment Stopped Signal {channel}, {reason}"), app.quit()))
    handler.deviceError.connect(lambda channel, error: print(f"Device Error: {error}"))

##! [Start]
def startExperiment():
    handler = tracker.getInstrumentHandler(INSTRUMENT_NAME)
    
    connectSignals(handler)

    error = handler.uploadExperimentToChannel(CHANNEL, customExperiment)
    if error.value() != AisErrorCode.Success:
        print(error.message())
        app.quit()

    error = handler.startUploadedExperiment(CHANNEL)
    if error.value() != AisErrorCode.Success:
        print(error.message())
        app.quit()

tracker.newDeviceConnected.connect(startExperiment)

error = tracker.connectToDeviceOnComPort(COMPORT)
if error.value() != AisErrorCode.Success:
    print(error.message())
    sys.exit()
##! [Start]

sys.exit(app.exec())