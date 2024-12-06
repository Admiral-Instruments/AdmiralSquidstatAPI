import sys
import struct
from PySide6.QtWidgets import QApplication
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

# initialize the application
app = QApplication()

# get device tracker
tracker = AisDeviceTracker.Instance()

# print device name when connected
tracker.newDeviceConnected.connect(lambda deviceName: print("Device is Connected: %s" % deviceName))

# request to connect to device using com port
tracker.connectToDeviceOnComPort("COM19")

# provide instrument serial number to get the handler.
handler = tracker.getInstrumentHandler("Plus2201");

# manages DC data input and output
# add more variables if you want to print more data to the console
handler.activeDCDataReady.connect(lambda channel, data: print("Channel %d - timestamp: %.9f, workingElectrodeVoltage: %.9f" % (channel, data.timestamp, data.workingElectrodeVoltage)))

# manages AC data input and output
# add more variables if you want to print more data to the console
handler.activeACDataReady.connect(lambda channel, data: print("Channel %d - frequency: %.9f, absoluteImpedance: %.9f, phaseAngle: %.9f" % (channel, data.frequency, data.absoluteImpedance, data.phaseAngle)))

# print when new node starts
handler.experimentNewElementStarting.connect(lambda channel, data: print("Channel %d - New Node beginning: %s, step number: %d, step sub: %d" % (channel, data.stepName, data.stepNumber, data.substepNumber)))

# print when experiment has completed
handler.experimentStopped.connect(lambda channel: print("Channel %d - Experiment Completed" % channel))

# define constant potential experiment at 0.5 V with a sampling interval of 1 s and a duration of 10 s
cvElement = AisConstantPotElement(0.5, 1, 10)

# define a potentiostatic EIS experiment 
# starting frequency (Hz), ending frequency (Hz), stepsPerDecade, voltageBias (V), voltageAmplitude (V)
eisElement = AisEISPotentiostaticElement(10000, 1, 10, 0.15, 0.1)

# define constant current experiment at 0.1 A with a sampling interval of 1 s and a duration of 10 s
ccElement = AisConstantCurrentElement(0.1, 1, 10)

# initialize an experiment
experiment = AisExperiment()

# add constant current element to first position of experiment list
# experiment runs 1 time
experiment.appendElement(ccElement,1)

# add constant voltage element to second position of experiment list
# experiment runs 1 time
experiment.appendElement(cvElement,1)

# initialize a sub experiment
subExperiment = AisExperiment()

# add constant current experiment to first position of the sub experiment list
# experiment runs 2 times
subExperiment.appendElement(ccElement, 2)

# add EIS experiment to second position of the sub experiment list
# experiment runs 2 time
subExperiment.appendElement(eisElement, 2)


# upload experiment on channel 2
handler.uploadExperimentToChannel(1, experiment)

# start experiment on channel 2
handler.startUploadedExperiment(1)

# upload sub experiment on channel 1
handler.uploadExperimentToChannel(0, subExperiment)

# start experiment on channel 1
handler.startUploadedExperiment(0)

# exit application
sys.exit(app.exec())