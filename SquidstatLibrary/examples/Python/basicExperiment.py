"""! @example basicExperiment.py """
##! [Setup]
import sys
from PySide6.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisExperiment, AisErrorCode
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisConstantPotElement

# Define relavant device information, for easy access
COMPORT = "COM16"
CHANNEL = 0

app = QApplication()

tracker = AisDeviceTracker.Instance()
##! [Setup]

##! [Experiment]
#           Voltage = 1V, Sampling Interval = 1s, Duration = 30s
cvElement = AisConstantPotElement(1, 1, 30)

# After this point, the experiment is empty, so we need to add some elements to it
experiment = AisExperiment()
# Append the constant potential element, and tell the experiment to execute that element 1 time
success = experiment.appendElement(cvElement, 1)

# Check if the element was added successfully
if not success:
    print("Error adding element to experiment")
    app.quit()
##! [Experiment]

##! [SignalLambda]
def connectSignals(handler):
    ##! [DataSignals]
    handler.activeDCDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Current: {data.current} Voltage: {data.workingElectrodeVoltage} CE Voltage : {data.counterElectrodeVoltage}"))
    handler.activeACDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Frequency: {data.frequency} Absolute Impedance: {data.absoluteImpedance}"))
    ##! [DataSignals]
    ##! [HandlerSignals]

    # Whenever a new node in the element starts, note: some Ais Elements contain multiple logical nodes
    # i.e AisCyclicVoltammatryElement contains 4 nodes for each linear segment of each cycle plus a quiet time node if enabled
    # So this lambda would be executed atleast 4 times for each cycle
    handler.experimentNewElementStarting.connect(lambda channel, info: print(f"New element starting: {info.stepName}"))
    # Whenever an experiment completes or is manually stopped, this will execute
    handler.experimentStopped.connect(lambda channel, reason: (print(f"Experiment Stopped Signal {channel}, {reason}"), app.quit()))
    handler.deviceError.connect(lambda channel, error: print(f"Device Error: {error}"))
    ##! [HandlerSignals]
##! [SignalLambda]

def startExperiment(deviceName):
    print(f"New Device Connected: {deviceName}")
    ##! [Start]
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
    ##! [Start]

##! [TrackerSignals]
tracker.newDeviceConnected.connect(startExperiment)
tracker.deviceDisconnected.connect(lambda deviceName: print(f"Device Disconnected: {deviceName}"))
##! [TrackerSignals]

##! [ConnectToDevice]
error = tracker.connectToDeviceOnComPort(COMPORT)
# Check if connection was successful
if error.value() != AisErrorCode.Success:
    print(error.message())
    sys.exit()
##! [ConnectToDevice]
    
# Calling sys.exit(app.exec()) will keep the program running until the application is exited
sys.exit(app.exec())