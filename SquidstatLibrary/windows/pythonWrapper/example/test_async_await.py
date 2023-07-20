import sys
import struct
import asyncio
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

app = QApplication([])


async def delayed_quit():
    await asyncio.sleep(5)  # Add some delay
    app.quit()

def experiment_complete_handler(channel):
    print("Experiment Completed: %d" % channel)
    asyncio.run(delayed_quit())




async def main():
    tracker = AisDeviceTracker.Instance()
    tracker.newDeviceConnected.connect(lambda deviceName: print("Device is Connected: %s" % deviceName))
    error = tracker.connectToDeviceOnComPort("COM4")

    if error:
        print(error.message())
        #return

    await asyncio.sleep(5)  # Add some initial delay before asking for hanlder.
    
    handler = tracker.getInstrumentHandler("Cycler1518")
    handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
    handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency), "absoluteImpedance: ", "{:.9f}".format(data.absoluteImpedance), "phaseAngle: ", "{:.9f}".format(data.phaseAngle)))
    handler.experimentNewElementStarting.connect(lambda channel, data: print("New Node beginning:", data.stepName, "step number: ", data.stepNumber, " step sub : ", data.substepNumber))

    handler.experimentStopped.connect(experiment_complete_handler)

    experiment = AisExperiment()
    cvElement = AisConstantPotElement(5, 1, 5)
    ccElement = AisConstantCurrentElement(1, 1, 5)

    subExperiment = AisExperiment()
    subExperiment.appendElement(ccElement, 1)
    subExperiment.appendElement(cvElement, 1)

    experiment.appendElement(ccElement, 1)
    experiment.appendElement(cvElement, 1)

    handler.uploadExperimentToChannel(0, experiment)
    handler.startUploadedExperiment(0)

    
    await asyncio.sleep(5)  # Add some initial delay
    
   

    
dd = main()
asyncio.run(dd)
sys.exit(app.exec_())  # Start the event loop

