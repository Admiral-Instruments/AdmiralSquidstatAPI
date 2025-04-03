"""! @example linkedChannels.py 
	This example will show you how linked channels can be used to combine the multiple channels on a device, in order to amplify the output for a single experiment.
	AisInstrumentHandler::setLinkedChannels MUST be called before each experiment that uses paralleled channels.
	Once linked, these channels must be controlled by ONLY the master channel, which is returned by the AIsInstrumentHandler::setLinkedChannels function.

	@note This feature is only available on Cycler models.
"""
import sys
from PySide6.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker, AisInstrumentHandler, AisErrorCode, AisConstantCurrentElement, AisExperiment

# Define relavant device information, for easy access
COMPORT = "COM1"

app = QApplication()

tracker = AisDeviceTracker.Instance()

# Build an experiment
ccElement = AisConstantCurrentElement(10, 1, 30)
experiment = AisExperiment()
success = experiment.appendElement(ccElement)

if not success:
	print("Error adding element to experiment")
	app.quit()

def connectSignals(handler):
	handler.activeDCDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Current: {data.current} Voltage: {data.workingElectrodeVoltage} CE Voltage : {data.counterElectrodeVoltage}"))
	handler.deviceError.connect(lambda channel, error: print(f"Device Error: {error}"))
	handler.experimentStopped.connect(lambda channel, reason: (print(f"Experiment Stopped Signal {channel}, {reason}"), app.quit()))

def startExperiment(deviceName):
	handler = tracker.getInstrumentHandler(deviceName)
	
	# Here we want to link channels 0 and 1 together, so we pass in a vector of the channels to link
    # It will return which of the channels is the master channel, this should be used to control the experiment
	masterChannel = handler.setLinkedChannels([ 0, 1 ])
	
	connectSignals(handler)
	
	error = handler.uploadExperimentToChannel(masterChannel, experiment)
	if error.value() != AisErrorCode.Success:
		print(error.message())
		app.quit()
	
	# Start the previously uploaded experiment on the master channel 
	error = handler.startUploadedExperiment(masterChannel)
	if error.value() != AisErrorCode.Success:
		print(error.message())
		app.quit()
		
	
tracker.newDeviceConnected.connect(startExperiment)
tracker.deviceDisconnected.connect(lambda deviceName: print(f"Device Disconnected: {deviceName}"))

error = tracker.connectToDeviceOnComPort(COMPORT)
if error.value() != AisErrorCode.Success:
	print(error.message()) 
	sys.exit()
# Calling sys.exit(app.exec()) will keep the program running until the application is exited
sys.exit(app.exec())