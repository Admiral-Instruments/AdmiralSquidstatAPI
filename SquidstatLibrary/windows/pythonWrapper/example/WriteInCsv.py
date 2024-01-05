import sys
from PySide2.QtCore import QIODevice, QThread, QObject, Signal
from PySide2.QtSerialPort import QSerialPort
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

# convert incoming data to string with single line
def convert_to_csv_line(data_list):
    return ','.join(str(item) for item in data_list) 

# class for reading and writing data from serial port
class SerialPortReader(QObject):
    # define the Qt signal. 
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

    # check if port is open, close port if open
    def closePort(self):
        if self.port.isOpen():
            self.port.close()

   
# class for writing data to a csv file
class WriteCSV:
    # init filename and file
    def __init__(self, filename):
        self.filename = filename
        self.file = None

    # open file and write headers
    def write_header(self, header):
        if self.file is None:
            self.file = open(self.filename, 'w')
        self.file.write(convert_to_csv_line(header) + '\n')

    # write data to file
    def write_data(self, data):
        if self.file is not None:
            self.file.write(convert_to_csv_line(data) + '\n')

    # close file when we are done  
    def close(self):
        if self.file is not None:
            self.file.close()


# class to handle the plot and write functionality
class PlottingThread(QThread):
    # define the signal.
    plotData = Signal(float, float)
    stopToPlot = Signal()
   
   # init self with values to be written
    def __init__(self, csv_writer):
        super().__init__()
        self.timestamps = []
        self.voltages = []
        self.csv_writer = csv_writer

    # setup data file with headers and connect the call back function on emitting of Qt signal.
    def run(self):
        self.csv_writer.write_header(['Timestamp', 'Working Electrode Voltage'])
        self.plotData.connect(self.add_data)
        self.stopToPlot.connect(self.close)

    # add data into list as well as call back handler you can use to plot the data on graph. ( Currently we are not using any plot.)
    def add_data(self, timestamp, voltage):
        self.timestamps.append(timestamp)
        self.voltages.append(voltage)
        self.csv_writer.write_data([timestamp, voltage])

    # close writer for a channel
    def close(self):
        self.csv_writer.close()


# setup serial port 
serialPort = QSerialPort("COM3")
# setup baud rate
serialPort.setBaudRate(QSerialPort.Baud9600)
# set up data type
serialPort.setDataBits(QSerialPort.Data8)
# define a serial port reader thread.
serialPortReader = SerialPortReader(serialPort)


# function to write data from the serial port reader to file
def writeDataToPort(data):
    serialPortReader.writeData(data)

# instantiate a device tracker
tracker = AisDeviceTracker.Instance()

# interact with data and send experiments to device
def onNewDeviceConnected(deviceName):
    # print which device has been connected
    print(f"Connected to: {deviceName}")
    # get handler using device name.
    handler = tracker.getInstrumentHandler(deviceName)
    # if handler is present for the particular device then we can interact with the data and upload/start/stop/puase/resume experiments
    if handler:
        # setup file name
        csv_writer = WriteCSV('dataFile.csv')
        # add csv file to plotting thread
        plottingThread = PlottingThread(csv_writer)
        # start thread
        plottingThread.start()

        # manages DC data input and output
        # add more variables if you want to print more data to the console
        handler.activeDCDataReady.connect(lambda channel, data: (
            print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ",
                  "{:.9f}".format(data.workingElectrodeVoltage)),
            plottingThread.plotData.emit(data.timestamp, data.workingElectrodeVoltage)
        ))

        # manages AC data input and output
        # add more variables if you want to print more data to the console
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency),
                                                                      "absoluteImpedance: ", "{:.9f}".format(
                                                                          data.absoluteImpedance), "phaseAngle: ",
                                                                      "{:.9f}".format(data.phaseAngle)))
        # write when new node is starting
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        # print when experiment has stopped, stop plotting thread
        handler.experimentStopped.connect(lambda channel: (print(f"Experiment completed on channel {channel}"),
                                                   plottingThread.stopToPlot.emit()))

        # initialize an experiment
        experiment = AisExperiment()

        # define a constant current experiment at 0.1 A, with 1 s sampling time, and a duration of 10 s
        ccElement = AisConstantCurrentElement(0.1, 1, 10)
        # define an open circuit experiment with a duration of 10 s and a sampling time of 2 s
        opencircuitElement = AisOpenCircuitElement(10, 2)

        # add constant current as the first element in the list
        # element runs 1 time
        experiment.appendElement(ccElement, 1)
        # add open circuit as the second and thirds elements in the list
        # element runs 2 times
        experiment.appendElement(opencircuitElement, 2)

        # upload experiment to channel 1
        error = handler.uploadExperimentToChannel(0, experiment)
        if error != 0:
            print(error.message())
            # return

        # start experiment on channel 1
        error = handler.startUploadedExperiment(0)
        if error != 0:
            print(error.message())

# connect to device associated with the tracker
tracker.newDeviceConnected.connect(onNewDeviceConnected)

# Request the device to connect using com port 4
error = tracker.connectToDeviceOnComPort("COM4")
if error:
    print(error.message())

# print the data which is received from another device (other then Squidstat)
serialPortReader.dataReceived.connect(lambda data: print("Received data from COM port 3:", data))
# setup a sub thread
serialPortThread = QThread()
# pushes object to another thread
serialPortReader.moveToThread(serialPortThread)
# start the sub thread
serialPortThread.start()
# exit program
sys.exit(app.exec_())