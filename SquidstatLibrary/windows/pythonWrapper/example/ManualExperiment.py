import sys
from PySide2.QtCore import QIODevice, QDataStream, QByteArray, QObject, Signal, QTimer
from PySide2.QtSerialPort import QSerialPort, QSerialPortInfo
from PySide2.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisCompRange
from SquidstatPyLibrary import AisDCData
from SquidstatPyLibrary import AisACData
from SquidstatPyLibrary import AisExperimentNode
from SquidstatPyLibrary import AisErrorCode
from SquidstatPyLibrary import AisExperiment
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisConstantPotElement
from SquidstatPyLibrary import AisEISPotentiostaticElement
from SquidstatPyLibrary import AisConstantCurrentElement
from SquidstatPyLibrary import AisOpenCircuitElement

# setup COM port
COMPort = "COM7"

# initialize the application
app = QApplication([])

# get the device tracker.
tracker = AisDeviceTracker.Instance()

# interact with data and send experiments to device
def onNewDeviceConnected(deviceName):
    # print which device has been connected
    print(f"Connected to: {deviceName}")
    
    # get the instrument handler using device name.
    handler = tracker.getInstrumentHandler(deviceName)
    
    # if a device exists in the handler then we can interact with the data and upload/start/stop/pause/resume experiments
    if handler:
    
        # manages DC data input and output
        # add more variables if you want to print more data to the console
        handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
    
        # manages AC data input and output
        # add more variables if you want to print more data to the console
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency), "absoluteImpedance: ", "{:.9f}".format(data.absoluteImpedance), "phaseAngle: ", "{:.9f}".format(data.phaseAngle)))
        
        # print when a new node starts to the console
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        
        # called when an experiment has completed
        handler.experimentStopped.connect(lambda channel: print(f"Experiment Completed: {channel}"))
        
        # define the channel you want to use. 
        # correct value is channel # - 1
        # below is channel 4
        experimentChannel = 3
        
        # start Manaul Experiment on channel 0
        error = handler.startManualExperiment(experimentChannel)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print("return code")
            print(error.message())
            return

        # set Sampling intervierval (1 s) for Manaul Experiment
        error = handler.setManualModeSamplingInterval(experimentChannel, 1)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(error.message())
            return

        # set constant current to (channel, 2 mA)
        error = handler.setManualModeConstantCurrent(experimentChannel, 0.002)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(error.message())
            return

        # Changes the manual current set point (350 mA) when the timer gets to 1 s (1000 ms)
        # QTimer.singleShot(1000, lambda: handler.setManualModeConstantCurrent(experimentChannel, 0.35))

        # # set constant votlage to (channel, 50 mV)
        # error = handler.setManualModeConstantVoltage(experimentChannel, 0.05)
        # if error.value() != AisErrorCode.ErrorCode.Success:
        #     print(error.message())
        #     return
        
        # Changes the manual voltage set point (0.5 V) when the timer gets to 3 s (3000 ms)
        # QTimer.singleShot(3000, lambda: handler.setManualModeConstantVoltage(experimentChannel, 0.5))

        # Timer stops the experiment after 25 seconds (25000 milliseconds)
        QTimer.singleShot(25000, lambda: handler.stopExperiment(experimentChannel))

# enable device in tracker
tracker.newDeviceConnected.connect(onNewDeviceConnected)

# define which comport to use
error = tracker.connectToDeviceOnComPort(COMPort)
if error:
    print(error.message())

# exit application
sys.exit(app.exec_())