import os
import socket
import threading
from PySide2.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisExperiment
from SquidstatPyLibrary import AisOpenCircuitElement
from SquidstatPyLibrary import AisErrorCode

# Define the server address and port
HOST = 'localhost'
PORT = 12345

# The comport the Squidstat is connected to
SQUIDCOMPORT = "COM4"
SQUIDNAME = "Plus1700"

# Create the QT application
app = QApplication([])
activeSockets = []

# This will build a start the Open Circuit Potential experiment
def start_ocp_experiment(handler, durationSec=60):
    # Create an experiment with elements
    experiment = AisExperiment()
    ocpElement = AisOpenCircuitElement(durationSec, 1)

    experiment.appendElement(ocpElement, 1)

    # Upload the experiment to channel 0
    error = handler.uploadExperimentToChannel(0, experiment)
    if error.value() != AisErrorCode.ErrorCode.Success:
        return error

    # Start the experiment
    return(handler.startUploadedExperiment(0))

# Send a specified command to our Squidstat
def command_to_device(command, handler):
    #Check if we had an argument associated with the command
    splitCommand = command.split(" ")
    action = splitCommand[0]
    actionArg = 0
    if(len(splitCommand) > 1):
        try:
            actionArg = int(splitCommand[1])
        except:
            actionArg = 0

    response = None
    if action == 'startExperiment':
        #print("Starting experiment...")
        response = start_ocp_experiment(handler, actionArg)
    elif action == 'stopExperiment':
        #print("Stopping experiment...")
        response = handler.stopExperiment(0)
    else:
        #print("Invalid command:", command)
        pass
    return response

# Handle commands from the client
def handle_command(command, handler, client_socket):
    # Send a response back to the client
    responseMsg = "Unknown Command"
    response = command_to_device(command, handler)
    if(response != None):
        responseMsg = response.message()
    response = "{}".format(responseMsg)
    client_socket.send(response.encode())

# Listen for the client's messages, and disconnect signals and terminate program when finished
def handle_client(handler, client_socket):
    print("Client connected")

    while True:
        # Receive data from the client
        try:
            data = client_socket.recv(1024).decode()
        except ConnectionResetError:
            break

        # Check if the client has closed the connection
        if not data:
            break

        # Handle the command
        handle_command(data, handler, client_socket)


    handler.activeDCDataReady.disconnect()
    handler.activeACDataReady.disconnect()
    handler.experimentNewElementStarting.disconnect()
    handler.experimentStopped.disconnect()
    command_to_device("stopExperiment", handler)
    # Close the client socket
    client_socket.close()
    print("Client disconnected")
    os._exit(1)

# Send data the the client based on the type of event (Hooked up to signals)
def send_data_to_client(client_socket, event_type, data):
    if event_type == "DCData":
        message = "timestamp: {:.9f}, workingElectrodeVoltage: {:.9f}".format(data.timestamp, data.workingElectrodeVoltage)
    elif event_type == "ACData":
        message = "frequency: {:.9f}, absoluteImpedance: {:.9f}, phaseAngle: {:.9f}".format(data.frequency, data.absoluteImpedance, data.phaseAngle)
    elif event_type == "NewElement":
        message = "New Node beginning: {}, step number: {}, step sub: {}".format(data.stepName, data.stepNumber, data.substepNumber)
    elif event_type == "ExperimentCompleted":
        message = "Experiment Completed: {}".format(data)
    else:
        return

    client_socket.send(message.encode())

def terminate_program():
    print("Press <CTRL>+c to close the server")
    try:
        while True:
            input()
    except (EOFError, KeyboardInterrupt):
        pass
    for socket in activeSockets:
        socket.close()
    app.quit()
    os._exit(1)


# Create the device tracker and connect to the Squidstat we will be using
print(f"Attempting to connect to the Squidstat {SQUIDNAME} on {SQUIDCOMPORT}...")
tracker = AisDeviceTracker.Instance()
tracker.newDeviceConnected.connect(lambda deviceName: print("Device is Connected: %s" % deviceName))
error = tracker.connectToDeviceOnComPort(SQUIDCOMPORT)

if error.value() != AisErrorCode.ErrorCode.Success:
    print(error.message())
    exit()

# Create the instrument handler
handler = tracker.getInstrumentHandler(SQUIDNAME)
print("Connection successful\n")

# Create the TCP/IP socket and bind it to our host
print("Starting server...")
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
activeSockets.append(server_socket)
server_socket.bind((HOST, PORT))

# Listen for incoming connections
server_socket.listen(1)

print("Server started successfully. Waiting for client connection...")

terminal_thread = threading.Thread(target=terminate_program)
terminal_thread.start()

# Accept a client connection
client_socket, client_address = server_socket.accept()
activeSockets.append(client_socket)

# Connect the signals to send data to the client
handler.activeDCDataReady.connect(lambda channel, data: send_data_to_client(client_socket, "DCData", data))
handler.activeACDataReady.connect(lambda channel, data: send_data_to_client(client_socket, "ACData", data))
handler.experimentNewElementStarting.connect(lambda channel, data: send_data_to_client(client_socket, "NewElement", data))
handler.experimentStopped.connect(lambda channel: send_data_to_client(client_socket, "ExperimentCompleted", channel))

# Start the listening process in a separate thread
listening_thread = threading.Thread(target=handle_client, args=(handler, client_socket))
listening_thread.start()

# Start the QT event loop
app.exec_()