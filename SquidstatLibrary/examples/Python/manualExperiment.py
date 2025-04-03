"""! @example manualExperiment.py """
import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QTimer
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisErrorCode


# Define relavant device information, for easy access
COMPORT = "COM1"
CHANNEL = 0

app = QApplication()

tracker = AisDeviceTracker.Instance()

def handleStopExperiment(handler, reason):
    print(reason)
    print("Experiment has ended. Closing application.")
    app.quit()


def connectSignal(handler):
    handler.activeDCDataReady.connect(lambda channel, data: print(f"Timestamp: {data.timestamp} Current: {data.current} Voltage: {data.workingElectrodeVoltage} CE Voltage : {data.counterElectrodeVoltage}"))
    handler.deviceError.connect(lambda channel, error: print(f"Device Error: {error}"))
    handler.experimentStopped.connect(handleStopExperiment)

def startExperiment(deviceName):
    handler = tracker.getInstrumentHandler(deviceName)

    connectSignal(handler)

    ##! [Start]

    # The default starting mode is always Open Circut Potential.

    print("Starting manual mode at open circuit potential")
    error = handler.startManualExperiment(CHANNEL)
    if error.value() != AisErrorCode.Success:
        print(error.message())
        app.quit()
    ##! [Start]

    ##! [ManualModes]

    # In this section we create wrapper functions for the manual mode changing functions.
    # These wrappers are called asynchronously when singleshot QTimers expire.

    # This function changes the instrument to Constant Current at .1A
    def setConstantCurrent(channel):
        print("Switching to constant current at .1A")
        error = handler.setManualModeConstantCurrent(channel, .1)
        if error.value() != AisErrorCode.Success:
            print(error.message())
    # It is called 5 seconds after the experiment starts
    QTimer.singleShot(5000, lambda:setConstantCurrent(CHANNEL))

    # This function changes the instrument to Constant Voltage at 1V
    def setConstantVoltage(channel):
        print("Switching to constant voltage at 1V")
        error = handler.setManualModeConstantVoltage(channel, 1)
        if error.value() != AisErrorCode.Success:
            print(error.message())
    # It is called 15 seconds after the experiment starts
    QTimer.singleShot(15000, lambda:setConstantVoltage(CHANNEL))

    # This function changes the instrument to Open Circuit Potential
    def setOpenCircuit(channel):
        print("Switching to open circuit potential")
        error = handler.setManualModeOCP(channel)
        if error.value() != AisErrorCode.Success:
            print(error.message())
    # It is called 25 seconds after the experiment starts
    QTimer.singleShot(25000, lambda:setOpenCircuit(CHANNEL))

    ##! [ManualModes]

    ##! [Stop]

    # Stop experiment after 30 seconds
    def stopExperiment(channel):
        print("Stopping experiment.")
        error = handler.stopExperiment(channel)
        if error.value() != AisErrorCode.Success:
            print(error.message())
    QTimer.singleShot(30000, lambda:stopExperiment(CHANNEL))

    ##! [Stop]

tracker.newDeviceConnected.connect(startExperiment)
tracker.deviceDisconnected.connect(lambda deviceName: print(f"Device Disconnected: {deviceName}"))

error = tracker.connectToDeviceOnComPort(COMPORT)
if error.value() != AisErrorCode.Success:
    print(error.message())
    sys.exit()
# Calling sys.exit(app.exec()) will keep the program running until the application is exited
sys.exit(app.exec())