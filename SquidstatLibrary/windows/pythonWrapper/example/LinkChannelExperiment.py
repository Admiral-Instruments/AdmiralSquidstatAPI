import sys
import struct
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

# initialize the application
app = QApplication()

# initialize a device tracker
tracker = AisDeviceTracker.Instance()

# print device name when tracker makes a connection
tracker.newDeviceConnected.connect(lambda deviceName: print(f"Connected to {deviceName}"))
# set com port to connect to device.
tracker.connectToDeviceOnComPort("COM15")
# get the handler using device name.
handler = tracker.getInstrumentHandler("Cycler1409")

# print DC data to temrinal when it comes in
handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
# print AC data to terminal when it comes in
handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency), "absoluteImpedance: ", "{:.9f}".format(data.absoluteImpedance), "phaseAngle: ", "{:.9f}".format(data.phaseAngle)))
# print when a new node starts
handler.experimentNewElementStarting.connect(lambda channel, data: print("New Node beginning:", data.stepName, "step number: ", data.stepNumber, " step sub : ", data.substepNumber))
# print when an experiment has completed
handler.experimentStopped.connect(lambda channel : print(f"Experiment Completed on channel {channel}"))

# initialize an experiment
experiment = AisExperiment()
# define a constant voltage element
# voltage set point (V), sampling interval (s), duration (s)
cvElement = AisConstantPotElement(-0.2, 0.1, 5)
# define a potentiostatic EIS experiment
# starting frequency (Hz), ending frequency (Hz), stepsPerDecade, voltageBias (V), voltageAmplitude (V)
eisElement = AisEISPotentiostaticElement(10000, 1, 10, 0.15, 0.1)
# define a constant current experiment
# current set point (A), sampling interval (s), duration (s)
ccElement = AisConstantCurrentElement(-0.2, 0.1, 5)

# initialize sub experiment
subExperiment = AisExperiment()
# add constant current to sub experiment
# runs one time
subExperiment.appendElement(ccElement, 1)
# add constant voltage to sub experiment
# runs one time
subExperiment.appendElement(cvElement, 1)

# append constant current to the first position of the experiment list
experiment.appendElement(ccElement,1)
# append constant voltage to the second position of the experiment list
experiment.appendElement(cvElement,1)
# append sub experiment to the experiment list
# will repeat once
# sets positions 3,4,5,6 in experiment queueu
experiment.appendSubExperiment(subExperiment, 2)
# append potentiostatic EIS experiment to experiment list in position 7
experiment.appendElement(eisElement,1)

# setup channels 3 and 4 in a bipolar configuration
masterchannel = handler.setBipolarLinkedChannels([2, 3])

# # setup channels 3 and 4 in a parallel configuration
# masterchannel = handler.setLinkedChannels([2, 3])

# upload experiment to linked channels (3 and 4)
handler.uploadExperimentToChannel(masterchannel,experiment)
# start experiment on linked channels
handler.startUploadedExperiment(masterchannel)

# exit application
sys.exit(app.exec_())