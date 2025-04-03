"""! @example nonblockingExperiment.py """
import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QTimer
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisErrorCode


# Define relavant device information, for easy access

CHANNEL = 0

app = QApplication()

tracker = AisDeviceTracker.Instance()

def connectSignals(handler):
	handler.activeDCDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Current: {data.current} Voltage: {data.workingElectrodeVoltage} CE Voltage : {data.counterElectrodeVoltage}"))
	handler.activeACDataReady.connect(lambda channel, data: print(f"Frequency: {data.frequency} Absolute Impedance: {data.absoluteImpedance} Phase Angle: {data.phaseAngle}"))
	handler.experimentNewElementStarting.connect(lambda channel, nodeInfo: print(f"New Node Beginning"))
	handler.experimentStopped.connect(lambda channel, reason: print(f"Experiment Stopped Signal {channel}, {reason}"))

def startExperiment(deviceName):
	handler = tracker.getInstrumentHandler(deviceName)

	connectSignals(handler)

	error = handler.startManualExperiment(CHANNEL)
	if error.value() != AisErrorCode.Success:
		print(error.message())
		app.quit()

	error = handler.setManualModeSamplingInterval(CHANNEL, 2)
	if error.value() != AisErrorCode.Success:
		print(error.message())
		app.quit()

	error = handler.setManualModeConstantVoltage(CHANNEL, 2)
	if error.value() != AisErrorCode.Success:
		print(error.message())
		app.quit()

	error = handler.setManualModeOCP(CHANNEL)
	if error.value() != AisErrorCode.Success:
		print(error.message())
		app.quit()
	
	def stopExperiment():
		error = handler.stopExperiment(CHANNEL)
		if error.value() != AisErrorCode.Success:
			print(error.message())
			app.quit()
			
	QTimer.singleShot(25000, stopExperiment)

	# Allow the recent loop to process other events while the current experiment is running
	while handler.isChannelBusy(CHANNEL):
		app.processEvents()
    
    # Process any remaining events
	app.processEvents()
	
tracker.newDeviceConnected.connect(startExperiment)

numDevices = tracker.connectAllPluggedInDevices()
if numDevices == 0:
	print("No devices connected") 
	sys.exit()

# Calling sys.exit(app.exec()) will keep the program running until the application is exited
sys.exit(app.exec())