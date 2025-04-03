"""! @example pulseData.py """

import sys
from PySide6.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisCompRange
from SquidstatPyLibrary import AisDCData
from SquidstatPyLibrary import AisACData
from SquidstatPyLibrary import AisExperimentNode
from SquidstatPyLibrary import AisErrorCode
from SquidstatPyLibrary import AisExperiment
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisCyclicVoltammetryElement
from SquidstatPyLibrary import AisDiffPulseVoltammetryElement
from SquidstatPyLibrary import AisDataManipulator
from SquidstatPyLibrary import AisPulseType

# do you want headers in your file?
write_header = True
# instantiate the data manipulator 
data_manipulator = AisDataManipulator()
# setup COM port
COMPORT = "COM5"
CHANNEL = 0

def create_logic(handler):
    def on_active_dc_data_ready(channel, data):
        # define global parameters
        global write_header, data_manipulator
        
        # convert time to UTC
        # utc_time = handler.getExperimentUTCStartTime(0)
        # read data via data manipulator
        data_manipulator.loadPrimaryData(data)

        # upon completed pulse, write data with header
        if data_manipulator.isPulseCompleted():
        
            # if write_header = true
            # prints header for every pulse data point
            if write_header:
                print("time, Pulse_current, base_current, diff_current, base_voltage, pulse_voltage")
                write_header = False
            
            # will print data as defined by the header print statement above
            print(f"{data.timestamp}, {data_manipulator.getPulseCurrent()}, {data_manipulator.getBaseCurrent()}, "
                f"{data_manipulator.getPulseCurrent() - data_manipulator.getBaseCurrent()}, "
                f"{data_manipulator.getBaseVoltage()}, {data_manipulator.getPulseVoltage()}")

    # function for printing ac data when it is received
    def on_active_ac_data_ready(channel, data):
        print(f"channel: {channel}, frequency: {data.frequency}, absoluteImpedance: {data.absoluteImpedance}, "
            f"phaseAngle: {data.phaseAngle}, timestamp: {data.timestamp}")

    # function for printing a new node when it begins
    def on_experiment_new_element_starting(channel, data):
        print(f"New Node beginning {data.stepName}, step number {data.stepNumber}, step sub: {data.substepNumber}")

    # fucntion for printing an experiment has stopped
    def on_experiment_stopped(channel, reason):
        print(f"Experiment has completed on channel {channel}, {reason}")
        QApplication.quit()

    # function to print an experiment has paused
    def on_experiment_paused(channel):
        print(f"Experiment on channel {channel} has been paused")

    # function to print an experiment has resumed
    def on_experiment_resumed(channel):
        print(f"Experiment resumed on channel {channel}")

    # pass dc data to data manipulator function
    handler.activeDCDataReady.connect(on_active_dc_data_ready)
    
    # pass ac data to print function
    handler.activeACDataReady.connect(on_active_ac_data_ready)
    
    # pass new node starting to print function
    handler.experimentNewElementStarting.connect(on_experiment_new_element_starting)
    
    # pass experiment stopped to print function
    handler.experimentStopped.connect(on_experiment_stopped)
    
    # pass experiment puased to print function
    handler.experimentPaused.connect(on_experiment_paused)
    handler.experimentResumed.connect(on_experiment_resumed)

# setup experiment
def main():
    # initialize the application
    app = QApplication()
    
    # get a device tracker
    tracker = AisDeviceTracker.Instance()

    # Create the AisDiffPulseVoltammetryElement pulse experiment.
    # startPotential (V), endPotential (V), potentialStep (V), pulseHeight (V), pulseWidth (s), pulsePeriod (s)
    dpv_element = AisDiffPulseVoltammetryElement(-0.115, 0.115, 0.005, 0.01, 0.02, 0.2)
    
    # set start voltage VS reference
    dpv_element.setStartVoltageVsOCP(False)
    
    # set end voltage VS reference
    dpv_element.setEndVoltageVsOCP(False)
    
    # set current range
    # will range up, but will not range down
    dpv_element.setApproxMaxCurrent(0.2)
    
    # initialize an experiment
    experiment = AisExperiment()
    
    # append differential pulse element to the experiment list
    # will run 1 time
    success = experiment.appendElement(dpv_element, 1)
    if not success:
        print("Error building experiment")
        sys.exit()

    # Create an `AisDataManipulator` class for calculating advance parameters.
    data_manipulator.setPulseType(AisPulseType.DifferentialPulse, dpv_element.getPulseWidth(), dpv_element.getPulsePeriod())
    
    # if device is connected, get name and use logic function to handle events
    def on_new_device_connected(device_name):
        
        # get instrument handler using device name
        handler = tracker.getInstrumentHandler(device_name)
        
        # create the required connections for the handler.
        create_logic(handler)
        
        # uplaod experiment to device.
        error = handler.uploadExperimentToChannel(CHANNEL, experiment)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(f"Error: {error.message()}")
            app.quit()
        
        # start experiment on device on defined channel
        error = handler.startUploadedExperiment(CHANNEL)
        if error.value() != AisErrorCode.ErrorCode.Success:
            print(f"Error: {error.message()}")
            app.quit()

    # connect call back handler which is called on connection of device.
    tracker.newDeviceConnected.connect(on_new_device_connected)
    
    # print which device has disconnected to terminal
    tracker.deviceDisconnected.connect(lambda device_name: print(f"{device_name} has been disconnected"))

    # if error is encountered, print to terminal
    error = tracker.connectToDeviceOnComPort(COMPORT)
    if error.value() != AisErrorCode.ErrorCode.Success:
        print(f"Error: {error.message()}")
        sys.exit()

    # exit application
    sys.exit(app.exec())

# run main
if __name__ == "__main__":
    main()
