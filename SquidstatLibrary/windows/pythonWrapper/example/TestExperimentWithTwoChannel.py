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

app = QApplication()

tracker = AisDeviceTracker.Instance()

tracker.newDeviceConnected.connect(lambda deviceName: print("Device is Connected: %s" % deviceName))
tracker.connectToDeviceOnComPort("COM19")

handler = tracker.getInstrumentHandler("Ace1102");

handler.activeDCDataReady.connect(lambda channel, data: print("Channel %d - timestamp: %.9f, workingElectrodeVoltage: %.9f" % (channel, data.timestamp, data.workingElectrodeVoltage)))
handler.activeACDataReady.connect(lambda channel, data: print("Channel %d - frequency: %.9f, absoluteImpedance: %.9f, phaseAngle: %.9f" % (channel, data.frequency, data.absoluteImpedance, data.phaseAngle)))
handler.experimentNewElementStarting.connect(lambda channel, data: print("Channel %d - New Node beginning: %s, step number: %d, step sub: %d" % (channel, data.stepName, data.stepNumber, data.substepNumber)))
handler.experimentStopped.connect(lambda channel: print("Channel %d - Experiment Completed" % channel))


cvElement = AisConstantPotElement(5, 1, 10)
eisElement = AisEISPotentiostaticElement(10000, 1, 10, 0.15, 0.1);
ccElement = AisConstantCurrentElement(1, 1, 10);


experiment = AisExperiment();
experiment.appendElement(ccElement,1)
experiment.appendElement(cvElement,1)


subExperiment = AisExperiment()
subExperiment.appendElement(ccElement, 1);
subExperiment.appendElement(cvElement, 1);


# start Expeiment on channel 2;
handler.uploadExperimentToChannel(1,experiment)
handler.startUploadedExperiment(1)

# start Expeiment on channel 1;
handler.uploadExperimentToChannel(0,subExperiment)
handler.startUploadedExperiment(0)


sys.exit(app.exec_())