import os
import socket
import threading
import time

# Create a TCP/IP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
activeSockets = [client_socket]

# Define the server address and port
SERVER_HOST = "localhost"
SERVER_PORT = 12345

# Function to send a command to the server
def send_command(command):
    # Send the command to the server
    try:
        client_socket.send(command.encode())
    except:
        print("Connection was closed by host")
        os._exit(1)

    # Receive and print the response from the server
    response = client_socket.recv(1024).decode()
    print("Server response:", response)

def interupt_listener():
    print("Press <CTRL>+c to stop the program at any time.")
    try:
        while True:
            input()
    except (EOFError, KeyboardInterrupt):
        pass
    for socket in activeSockets:
        socket.close()
    os._exit(1)

# Try and open a socket to the server
try:
    client_socket.connect((SERVER_HOST, SERVER_PORT))
except Exception as ex:
    print("Unable to establish connection to server:\n%s" % ex)
    exit()

print("Connected to the server.")

# Get a duration from the user
duration = 0
while duration == 0:
    try:
        duration = int(input("Enter a duration for the Open Circuit Potential: "))
    except ValueError:
        duration = 0
    if(duration < 1):
        print("Invalid entry.")
        duration = 0

# Send the start command to the server with the duration
send_command(f'startExperiment {duration}')

interupt_thread = threading.Thread(target=interupt_listener)
interupt_thread.start()

# Listen for information from the server, which at this point will be data and the experiment stop message
while True:
    try:
        data = client_socket.recv(1024).decode()
    except (ConnectionAbortedError, BrokenPipeError):
        # This exception will be raised when the user presses <ENTER>
        print("Finishing connection")
        break
    except ConnectionResetError:
        print("The server closed the connection suddenly.")
        break

    if not data:
        break

    # Handle the data that was received.
    print(data)

    if("Experiment Completed: " in data):
        break

os._exit(1)
