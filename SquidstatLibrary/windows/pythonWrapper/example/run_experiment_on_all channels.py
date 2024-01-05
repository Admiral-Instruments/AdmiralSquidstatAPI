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

# function to add a ',' between data and create the single line.
def convert_to_csv_line(data_list):
    return ','.join(str(item) for item in data_list) 


# define a class to read data from a serial port at which another (other then squidstat) is connected.
class SerialPortReader(QObject):
    dataReceived = Signal(str)

    # init self
    def __init__(self, port):
        super().__init__()
        self.port = port

    # open port as read only if closed
    # when port is open wait to receive data
    # then decode data
    # send our received data
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

    # check if port is open and close it if it is
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

# class for writing data on a thread for each channel
class WriterThread(QThread):
    # data for file to generate a plot with 
    plotData = Signal(int, float, float, float)
    stopToPlot = Signal(int)

    # init self with values to be written
    def __init__(self, numberOfchannel):
        super().__init__()
        self.timestamps = []
        self.voltages = []
        self.currents = []
        self.csv_writers = [WriteCSV(f'dataFile_channel{channel}.csv') for channel in range(numberOfchannel)]

    # setup file with headers and store incoming data in plotData
    def run(self):
        for csv_writer in self.csv_writers:
            csv_writer.write_header(['Timestamp', 'Working Electrode Voltage', 'Current'])
        self.plotData.connect(self.add_data)
        self.stopToPlot.connect(self.close)

    # add data into list
    def add_data(self,channel, timestamp, voltage, current):
        self.timestamps.append(timestamp)
        self.voltages.append(voltage)
        self.currents.append(current)
        self.csv_writers[channel].write_data([timestamp, voltage, current])

    # close writer for a channel
    def close(self, channel):
        self.csv_writers[channel].close()


# setup serial port
serialPort = QSerialPort("COM3")
# setup baud rate
serialPort.setBaudRate(QSerialPort.Baud9600)
# set up data type
serialPort.setDataBits(QSerialPort.Data8)
# define a serial port reader
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
    # get instrument handler using device name.
    handler = tracker.getInstrumentHandler(deviceName)
    # if a device exists in the handler then we can interact with the data and upload/start/stop experiments
    if handler:
        # set number of writer threads required
        writerThread = WriterThread(handler.getNumberOfChannels())
        # start the writer threads
        writerThread.start()

        # manages DC data input and output
        # add more variables if you want to print more data to the console
        handler.activeDCDataReady.connect(lambda channel, data: (
            print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ",
                  "{:.9f}".format(data.workingElectrodeVoltage), "current: ", "{:.9f}".format(data.current)),
            writerThread.plotData.emit(channel, data.timestamp, data.workingElectrodeVoltage, data.current)
        ))

        # manages AC data input and output
        # add more variables if you want to print more data to the console
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency),
                                                                      "absoluteImpedance: ", "{:.9f}".format(
                                                                          data.absoluteImpedance), "phaseAngle: ",
                                                                      "{:.9f}".format(data.phaseAngle)))
        # write when new node is starting
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        # print when experiment has stopped
        handler.experimentStopped.connect(lambda channel: (print(f"Experiment completed on channel {channel}",
                                                           writerThread.stopToPlot.emit(channel)))

        # initialize an experiment
        experiment = AisExperiment()

        # define a constant current experiment at 0.1 A, with 1 s sampling time, and a duration of 10 s
        ccElement = AisConstantCurrentElement(0.1, 1, 10)
        # define an open circuit experiment with a duration of 10 s and a sampling time of 2 s
        opencircuitElement = AisOpenCircuitElement(10, 2)

        # add constant current as the first element in the list
        # experiment runs 1 time
        experiment.appendElement(ccElement, 1)
        # add open circuit as the second element in the list
        # experiment runs 2 times
        experiment.appendElement(opencircuitElement, 2)

        # upload experiment on channel 1
        error = handler.uploadExperimentToChannel(0, experiment)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(error.message())
            return	
		
        # upload experiment on channel 2
        error = handler.uploadExperimentToChannel(1, experiment)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(error.message())
            return	
			
        # upload experiment on channel 3
        error = handler.uploadExperimentToChannel(2, experiment)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(error.message())
            return	

        # upload experiment on channel 4	
        error = handler.uploadExperimentToChannel(3, experiment)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(error.message())
            return	
			
		# start experiment on desired channel
        # print error if there is an issue 
        error = handler.startUploadedExperiment(0)
        if error:
            print(error.message())
        
        error = handler.startUploadedExperiment(1)
        if error:
            print(error.message())
        
        error = handler.startUploadedExperiment(2)
        if error:
            print(error.message())
        
        error = handler.startUploadedExperiment(3)
        if error:
            print(error.message())

# connect call back funcation with the tracker.
# on successful Connection of the device, Call back method is call
tracker.newDeviceConnected.connect(onNewDeviceConnected)

# request to connect the Squidstat device from COM 4.
error = tracker.connectToDeviceOnComPort("COM4")
if error:
    print(error.message())

# read the data from other device in parallel to reading data from Squidstat.
serialPortReader.dataReceived.connect(lambda data: print("Received data from COM port 3:", data))
# setup a sub thread to operate the another device with Squidstat. 
serialPortThread = QThread()
# pushes object to sub thread
serialPortReader.moveToThread(serialPortThread)
# start the sub thread
serialPortThread.start()
# exit program
sys.exit(app.exec_())