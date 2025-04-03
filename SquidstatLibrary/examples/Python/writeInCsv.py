"""! @example writeinCSV.py 
This is an example of the writeinCSV.py file, which helps control Squidstat in parallel with other devices. 
In this example, we inform other devices using SerialPortReader::writeData when a new element starts executing 
inside Squidstat, and also print the data received from the other device using the SerialPortReader::dataReceived signal. 
All operations occur in parallel with the Squidstat operation.

The Squidstat data is also written to a CSV file.

In detail:
1. The `SerialPortReader` class handles reading from and writing to a serial port of other device.
2. The `WriteCSV` class is responsible for writing data to a CSV file which is received from Squidstat.
3. The `writingThread` class is used to manage the data writing in csv file process in a separate thread.
"""

import sys
from PySide6.QtCore import QIODevice, QThread, QObject, Signal
from PySide6.QtSerialPort import QSerialPort
from PySide6.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisErrorCode
from SquidstatPyLibrary import AisExperiment
from SquidstatPyLibrary import AisConstantCurrentElement
from SquidstatPyLibrary import AisOpenCircuitElement

# initialize the application
app = QApplication([])

# convert incoming data to string with single line
def convert_to_csv_line(data_list):
    return ','.join(str(item) for item in data_list) 

# \cond EXCLUDE_FROM_DOX
# class for reading and writing data from serial port of other device.
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
            successfullyOpen = self.port.open(QIODevice.WriteOnly)
            if not successfullyOpen:
                print("USB port is not open.")
        self.port.write(data.encode())

    # check if port is open, close port if open
    def closePort(self):
        if self.port.isOpen():
            self.port.close()

   
# class for writing data to a csv file. data received from Squidstat.
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


# class to handle the write funcationality on seprate thread in pareller operation of Squidstat and other device.
class writingThread(QThread):
    # define the signal.
    writeData = Signal(float, float)
    stopTowrite = Signal()
   
   # init self with values to be written
    def __init__(self, csv_writer):
        super().__init__()
        self.timestamps = []
        self.voltages = []
        self.csv_writer = csv_writer

    # setup data file with headers and connect the call back function on emitting of Qt signal.
    def run(self):
        self.csv_writer.write_header(['Timestamp', 'Working Electrode Voltage'])
        self.writeData.connect(self.add_data)
        self.stopTowrite.connect(self.close)

    # add data into list as well as call back handler you can use to write the data in csv file.
    def add_data(self, timestamp, voltage):
        self.timestamps.append(timestamp)
        self.voltages.append(voltage)
        self.csv_writer.write_data([timestamp, voltage])

    # close writer for a channel
    def close(self):
        self.csv_writer.close()
# \endcond

# setup serial port of other device.
serialPort = QSerialPort("COM3")
# setup baud rate of other device.
serialPort.setBaudRate(QSerialPort.Baud9600)
# set up communication data type of other device.
serialPort.setDataBits(QSerialPort.Data8)
# define a serial port reader thread.
serialPortReader = SerialPortReader(serialPort)


# function to write data from the serial port of other device.
def writeDataToPort(data):
    serialPortReader.writeData(data)

# instantiate a Squidstat device tracker
tracker = AisDeviceTracker.Instance()

# interact with data and send experiments to Squidstat device
def onNewDeviceConnected(deviceName):
    # print which device has been connected
    print(f"Connected to: {deviceName}")
    # get handler using device name.
    handler = tracker.getInstrumentHandler(deviceName)
    # if handler is present for the particular device then we can interact with the data and upload/start/stop/puase/resume experiments
    if handler:
        # setup file name
        csv_writer = WriteCSV('dataFile.csv')
        # add csv file to writing thread.
        writingThread = writingThread(csv_writer)
        # start sub thread for write funcationality.
        writingThread.start()

        # manages DC data input and output
        # add more variables if you want to print more data to the console
        # send the signal to writing thread to write the information in csv file.
        handler.activeDCDataReady.connect(lambda channel, data: (
            print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ",
                  "{:.9f}".format(data.workingElectrodeVoltage)),
            writingThread.writeData.emit(data.timestamp, data.workingElectrodeVoltage)
        ))

        # manages AC data input and output
        # add more variables if you want to print more data to the console
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency),
                                                                      "absoluteImpedance: ", "{:.9f}".format(
                                                                          data.absoluteImpedance), "phaseAngle: ",
                                                                      "{:.9f}".format(data.phaseAngle)))
        # write when new node is starting
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        # print when experiment has stopped, stop writeting thread
        # send the signal to writing thread experiment is completed, which will close the csv file.
        handler.experimentStopped.connect(lambda channel: (print(f"Experiment completed on channel {channel}"), writingThread.stopTowrite.emit(), app.quit()))

        # initialize an experiment
        experiment = AisExperiment()

        # define a constant current experiment at 0.1 A, with 1 s sampling time, and a duration of 10 s
        ccElement = AisConstantCurrentElement(0.1, 1, 10)
        # define an open circuit experiment with a duration of 10 s and a sampling time of 2 s
        opencircuitElement = AisOpenCircuitElement(10, 2)

        # add constant current as the first element in the list
        # element runs 1 time
        successfullyadd = experiment.appendElement(ccElement, 1)
        # add open circuit as the second and thirds elements in the list
        # element runs 2 times
        successfullyadd |= experiment.appendElement(opencircuitElement, 2)

        if not successfullyadd:
            print("Error adding element to experiment")
            app.quit()

        # upload experiment to channel 1
        error = handler.uploadExperimentToChannel(0, experiment)
        if error.value() != AisErrorCode.Success:
            print(error.message())
            app.quit()

        # start experiment on channel 1
        error = handler.startUploadedExperiment(0)
        if error.value() != AisErrorCode.Success:
            print(error.message())
            app.quit()

# connect to device associated with the tracker
tracker.newDeviceConnected.connect(onNewDeviceConnected)

# Request the device to connect using com port 4
error = tracker.connectToDeviceOnComPort("COM4")
if error:
    print(error.message())
    sys.exit()

# print the data which is received from another device (other then Squidstat)
serialPortReader.dataReceived.connect(lambda data: print("Received data from COM port 3:", data))
# setup a sub thread for read and write the information from the other device.
serialPortThread = QThread()
# pushes object to another thread
serialPortReader.moveToThread(serialPortThread)
# start the sub thread of other device.
serialPortThread.start()
# exit program
sys.exit(app.exec())