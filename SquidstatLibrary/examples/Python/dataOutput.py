"""! @example dataOutput.py """
import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import  QTextStream, QFile, QStandardPaths, QIODevice
from SquidstatPyLibrary import AisDeviceTracker, AisErrorCode
from SquidstatPyLibrary import AisExperiment
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisConstantPotElement
from SquidstatPyLibrary import AisConstantCurrentElement

# Define relavant device information, for easy access
COMPORT = "COM1"
CHANNEL = 0

app = QApplication()

tracker = AisDeviceTracker.Instance()

cvElement = AisConstantPotElement(1, 1, 30)
ccElement = AisConstantCurrentElement(0.001, 1, 60)

experiment = AisExperiment()

success = True

success &= experiment.appendElement(cvElement, 1)
success &= experiment.appendElement(ccElement, 1)

if not success:
    print("Error building experiment")
    sys.exit()

filePath = ""
fileNum = 1

def onNewElementStarting(channel, info):
    name = f"/{fileNum}_{info.stepNumber}_{info.stepName}.csv"
    filePath = QStandardPaths.writableLocation(QStandardPaths.DesktopLocation) + name

    file = QFile(filePath)
    if not file.open(QIODevice.WriteOnly | QIODevice.Text):
        return

    # Writing headers to file
    out = QTextStream(file)
    out << "Time Stamp, Counter Electrode Voltage, Working Electrode Voltage, Current \n"
    file.close()

    print(f"New element beginning: {info.stepName} step: {info.stepNumber}")

def onActiveDCDataReady(channel, data):
    print(f"current: {data.current}  voltage: {data.workingElectrodeVoltage}  counter electrode: {data.counterElectrodeVoltage}  timestamp: {data.timestamp}")

    # Save the DC data to the file created at element beginning
    file = QFile(filePath)
    if not file.open(QIODevice.Append | QIODevice.WriteOnly | QIODevice.Text):
        return
    
    out = QTextStream(file)
    out << data.timestamp << ","<< data.counterElectrodeVoltage << "," << data.workingElectrodeVoltage << "," << data.current << "\n"
    file.close()

def connectSignals(handler):  
    handler.activeDCDataReady.connect(onActiveDCDataReady)
    handler.activeACDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Frequency: {data.frequency} Absolute Impedance: {data.absoluteImpedance}"))
    handler.experimentNewElementStarting.connect(onNewElementStarting)
    handler.experimentStopped.connect(lambda channel, reason: (print(f"Experiment Stopped Signal {channel}, {reason}"), app.quit()))
    handler.deviceError.connect(lambda channel, error: print(f"Device Error: {error}"))

def runExperiment(deviceName):
    handler = tracker.getInstrumentHandler(deviceName)
    
    connectSignals(handler)

    error = handler.uploadExperimentToChannel(CHANNEL, experiment)
    # Exit the application if there is any error uploading experiment
    if error.value() != AisErrorCode.Success:
        print(error.message())
        app.quit()

    error = handler.startUploadedExperiment(CHANNEL)
    # Exit the application if there is any error starting experiment
    if error.value() != AisErrorCode.Success:
        print(error.message())
        app.quit()

tracker.newDeviceConnected.connect(runExperiment)
tracker.deviceDisconnected.connect(lambda deviceName: print(f"Device Disconnected: {deviceName}"))

error = tracker.connectToDeviceOnComPort(COMPORT)
# Check if connection was successful
if error.value() != AisErrorCode.Success:
    print(error.message())
    sys.exit()
    
# Calling sys.exit(app.exec()) will keep the program running until the application is exited
sys.exit(app.exec())