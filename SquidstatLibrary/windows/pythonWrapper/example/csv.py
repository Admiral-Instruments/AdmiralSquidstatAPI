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

app = QApplication([])


def convert_to_csv_line(data_list):
        return ','.join(str(item) for item in data_list) 


class SerialPortReader(QObject):
    dataReceived = Signal(str)

    def __init__(self, port):
        super().__init__()
        self.port = port

    def run(self):
        if not self.port.isOpen():
            self.port.open(QIODevice.ReadOnly)
        while self.port.isOpen():
            if self.port.waitForReadyRead():
                data = self.port.readAll().data().decode()
                self.dataReceived.emit(data)

    def writeData(self, data):
        if not self.port.isOpen():
            self.port.open(QIODevice.WriteOnly)
        self.port.write(data.encode())

    def closePort(self):
        if self.port.isOpen():
            self.port.close()

   

class WriteCSV:
    def __init__(self, filename):
        self.filename = filename
        self.file = None

    def write_header(self, header):
        if self.file is None:
            self.file = open(self.filename, 'w')
        self.file.write(convert_to_csv_line(header) + '\n')

    def write_data(self, data):
        if self.file is not None:
            self.file.write(convert_to_csv_line(data) + '\n')
            
    def close(self):
        if self.file is not None:
            self.file.close()


class PlottingThread(QThread):
    plotData = Signal(float, float)
    stopToPlot = Signal()
   
    def __init__(self, csv_writer):
        super().__init__()
        self.timestamps = []
        self.voltages = []
        self.csv_writer = csv_writer

    def run(self):
        self.csv_writer.write_header(['Timestamp', 'Working Electrode Voltage'])
        self.plotData.connect(self.add_data)
        self.stopToPlot.connect(self.close)

    def add_data(self, timestamp, voltage):
        
        self.timestamps.append(timestamp)
        self.voltages.append(voltage)
        self.csv_writer.write_data([timestamp, voltage])

    def close(self):
        self.csv_writer.close()


serialPort = QSerialPort("COM3")
serialPort.setBaudRate(QSerialPort.Baud9600)
serialPort.setDataBits(QSerialPort.Data8)

serialPortReader = SerialPortReader(serialPort)


def writeDataToPort(data):
    serialPortReader.writeData(data)


tracker = AisDeviceTracker.Instance()


def onNewDeviceConnected(deviceName):
    print("Device is Connected: %s" % deviceName)
    handler = tracker.getInstrumentHandler(deviceName)
    if handler:
        csv_writer = WriteCSV('dataFile.csv')
        plottingThread = PlottingThread(csv_writer)
        plottingThread.start()

        handler.activeDCDataReady.connect(lambda channel, data: (
            print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ",
                  "{:.9f}".format(data.workingElectrodeVoltage)),
            plottingThread.plotData.emit(data.timestamp, data.workingElectrodeVoltage)
        ))
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency),
                                                                      "absoluteImpedance: ", "{:.9f}".format(
                                                                          data.absoluteImpedance), "phaseAngle: ",
                                                                      "{:.9f}".format(data.phaseAngle)))
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        handler.experimentStopped.connect(lambda channel: (print("Experiment Completed: %d" % channel),
                                                   plottingThread.stopToPlot.emit()))

        experiment = AisExperiment()

        ccElement = AisConstantCurrentElement(1, 1, 10)
        opencircuitElement = AisOpenCircuitElement(10, 2)

        experiment.appendElement(ccElement, 1)
        experiment.appendElement(opencircuitElement, 1)

        error = handler.uploadExperimentToChannel(0, experiment)
        if error != 0:
            print(error.message())
            # return

        error = handler.startUploadedExperiment(0)
        if error != 0:
            print(error.message())

tracker.newDeviceConnected.connect(onNewDeviceConnected)

error = tracker.connectToDeviceOnComPort("COM4")
if error:
    print(error.message())

serialPortReader.dataReceived.connect(lambda data: print("Received data from COM port 3:", data))
serialPortThread = QThread()

serialPortReader.moveToThread(serialPortThread)

serialPortThread.start()

sys.exit(app.exec_())