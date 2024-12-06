import sys
import struct
import asyncio
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
app = QApplication([])

# Add delay before quitting
async def delayed_quit():
    await asyncio.sleep(5) 
    app.quit()

# function to print when experiment has completed
def experiment_complete_handler(channel):
    print(f"Experiment Completed on channel {channel}")
    asyncio.run(delayed_quit())


# main function setup as async
async def main():
    # initialize a device tracker
    tracker = AisDeviceTracker.Instance()
    # connect to device associated with the tracker
    # print device serial number
    tracker.newDeviceConnected.connect(lambda deviceName: print(f"Connected Device: {deviceName}"))
    # connect to device on com port 4
    error = tracker.connectToDeviceOnComPort("COM4")
    if error:
        print(error.message())

    # Add initial delay before asking for hanlder (5 s)
    await asyncio.sleep(5)  
    
    # use serial number to get handler for instrument
    handler = tracker.getInstrumentHandler("Cycler1518")
    # manages DC data input and output
    # add more variables if you want to print more data to the console
    handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
    # manages AC data input and output
    # add more variables if you want to print more data to the console
    handler.activeACDataReady.connect(lambda channel, data: print("frequency:", "{:.9f}".format(data.frequency), "absoluteImpedance: ", "{:.9f}".format(data.absoluteImpedance), "phaseAngle: ", "{:.9f}".format(data.phaseAngle)))
    # print when a new node starts to the console
    handler.experimentNewElementStarting.connect(lambda channel, data: print("New Node beginning:", data.stepName, "step number: ", data.stepNumber, " step sub : ", data.substepNumber))
    # called when an experiment has completed
    handler.experimentStopped.connect(experiment_complete_handler)

    # initialize an experiment
    experiment = AisExperiment()
    # define a constant potential experiment at 0.2 V, with 1 s sampling time, and a duration of 10 s
    cvElement = AisConstantPotElement(0.2, 1, 10)
    # define a constant current experiment at 0.1 A, with 0.1 s sampling time, and a duration of 5 s
    ccElement = AisConstantCurrentElement(0.1, 0.1, 5)

    # initialize a sub experiment
    subExperiment = AisExperiment()
    # add constant current experiment to position 1 of the sub experiment
    # this experiment will run 1 time
    subExperiment.appendElement(ccElement, 1)
    # add constant potential experiment to positions 2 and 3 of the sub experiment
    # this experiment will run 2 times
    subExperiment.appendElement(cvElement, 2)

    # add constant current experiment to position 1 and 2 of the main experiment
    # this experiment will run 2 times
    experiment.appendElement(ccElement, 2)
    # add constant potential experiment to position 3 of the main experiment
    # this experiment will run 1 time
    experiment.appendElement(cvElement, 1)
    
    # add the sub experiment to the main experiment
    # the sub experiment will run 2 times
    experiment.appendSubExperiment(subExperiment, 2)

    # upload experiment list to the given channel
    handler.uploadExperimentToChannel(0, experiment)
    # start the expiment on channel
    handler.startUploadedExperiment(0)

    # Add initial delay (5 s)
    await asyncio.sleep(5)  
    
   

# setup main to be ran as async    
dd = main()
asyncio.run(dd)

# exit application
sys.exit(app.exec())  # Start the event loop

