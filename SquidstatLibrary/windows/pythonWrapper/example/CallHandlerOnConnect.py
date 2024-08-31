import sys
from PySide2.QtCore import QIODevice, QDataStream, QByteArray, QThread, QObject, Signal
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

# initialize the application
app = QApplication([])

# class for reading and writing data from serial port
class SerialPortReader(QObject):
    # incoming data
    dataReceived = Signal(str)
    
    # initialize self and port
    def __init__(self, port):
        super().__init__()
        self.port = port
        
    # open port if closed
    # get data and decode
    # emit data
    def run(self):
        if not self.port.isOpen():
            self.port.open(QIODevice.ReadOnly)
        while self.port.isOpen():
            if self.port.waitForReadyRead():
                data = self.port.readAll().data().decode()
                self.dataReceived.emit(data)
    
    # open port if closed
    # write data and encode
    def writeData(self, data):
        if not self.port.isOpen():
            self.port.open(QIODevice.WriteOnly)
        self.port.write(data.encode())

    # check if port is open then close if it is
    def closePort(self):
        if self.port.isOpen():
            self.port.close()
            
# setup COM port
serialPort = QSerialPort("COM3")
# set baud rate
serialPort.setBaudRate(QSerialPort.Baud9600)
# set data type
serialPort.setDataBits(QSerialPort.Data8)
# define a serial port reader
serialPortReader = SerialPortReader(serialPort)

# function to call to write data
def writeDataToPort(data):
    serialPortReader.writeData(data)

# initialize a device tracker
tracker = AisDeviceTracker.Instance()

# interact with data and send experiments to device
def onNewDeviceConnected(deviceName):
    # print which device has been connected
    print(f"Connected to: {deviceName}")
    # send instrument name to handler
    handler = tracker.getInstrumentHandler(deviceName)
    # if a device exists in the handler then we can interact with the data and send experiments
    if handler:
        # manages DC data input and output
        # add more variables if you want to print more data to the console
        handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
        # manages AC data input and output
        # add more variables if you want to print more data to the console
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency), "absoluteImpedance: ", "{:.9f}".format(data.absoluteImpedance), "phaseAngle: ", "{:.9f}".format(data.phaseAngle)))
        # print when a new node starts
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        # called when an experiment has completed
        handler.experimentStopped.connect(lambda channel: print(f"Experiment completed on channel {channel}"))

        # define the channel you want to use. 
        # correct value is channel # - 1
        # below is channel 4
        experimentChannel = 3

        # initialize an experiment
        experiment = AisExperiment()

        # define a constant current experiment at 0.1 A, with 1 s sampling time, and a duration of 10 s
        ccElement = AisConstantCurrentElement(0.1, 1, 10)
        # define an open circuit experiment with a duration of 10 s and a sampling time of 2 s
        opencircuitElement = AisOpenCircuitElement(10, 2)

        # add constant current as the first experiment in the list
        # experiment runs 1 time
        experiment.appendElement(ccElement, 1)
        # add open circuit as the second experiment in the list
        # experiment runs 2 times
        experiment.appendElement(opencircuitElement, 2)

        # upload experiment to device on desired channel
        # print error if there is an issue 
        error = handler.uploadExperimentToChannel(experimentChannel, experiment)
        if error:
            print(error.message())
            return

        # start desired experiment list
        # print error if there is an issue
        error = handler.startUploadedExperiment(0)
        if error:
            print(error.message())

# connect to device associated with the tracker
tracker.newDeviceConnected.connect(onNewDeviceConnected)

# COM5 is reading the data sent from COM3
error = tracker.connectToDeviceOnComPort("COM5")
if error:
    print(error.message())

# Print data from external connected device on COM port 3
serialPortReader.dataReceived.connect(lambda data: print("Received data from COM port 3:", data))
# setup a thread
serialPortThread = QThread()
# pushes object to another sub thread
serialPortReader.moveToThread(serialPortThread)
# start the sub thread
serialPortThread.start()
# exit program
sys.exit(app.exec_())