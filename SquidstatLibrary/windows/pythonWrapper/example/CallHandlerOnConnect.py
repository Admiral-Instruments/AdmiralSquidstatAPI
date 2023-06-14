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

app = QApplication([])

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
        handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
        handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency), "absoluteImpedance: ", "{:.9f}".format(data.absoluteImpedance), "phaseAngle: ", "{:.9f}".format(data.phaseAngle)))
        handler.experimentNewElementStarting.connect(lambda channel, data: writeDataToPort(data.stepName))
        handler.experimentStopped.connect(lambda channel: print("Experiment Completed: %d" % channel))
        
        experiment = AisExperiment()

        ccElement = AisConstantCurrentElement(1, 1, 10)
        opencircuitElement = AisOpenCircuitElement(10, 2)

        experiment.appendElement(ccElement, 1)
        experiment.appendElement(opencircuitElement, 1)

        error = handler.uploadExperimentToChannel(0, experiment)
        if error:
            print(error.message())
            return

        handler.startUploadedExperiment(0)
        if error:
            print(error.message())

tracker.newDeviceConnected.connect(onNewDeviceConnected)

error = tracker.connectToDeviceOnComPort("COM5")
if error:
    print(error.message())

serialPortReader.dataReceived.connect(lambda data: print("Received data from COM port 3:", data))
serialPortThread = QThread()

serialPortReader.moveToThread(serialPortThread)

serialPortThread.start()

sys.exit(app.exec_())