"""! @example firmwareUpdate.py """
##! [FirmwareUpdate]
import sys
from PySide6.QtWidgets import QApplication

from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisErrorCode

# Define relavant device information, for easy access
COMPORT = "COM16"

app = QApplication()

tracker = AisDeviceTracker.Instance()

def onProgressMessage(message):
    print(message)
    if message.__contains__("firmware is updated"):
        app.quit()

##! [FirmwareUpdateNotification]
tracker.firmwareUpdateNotification.connect(onProgressMessage)
##! [FirmwareUpdateNotification]

# Attempt to connect to the device
error = tracker.connectToDeviceOnComPort(COMPORT)
if error.value() == AisErrorCode.FirmwareNotSupported:
    error = tracker.updateFirmwareOnComPort(COMPORT)
    
    # Some other error occured
    if error.value() != AisErrorCode.Success:
        print(f"Error: {error.message()}")
        sys.exit()
elif error.value() != AisErrorCode.Success:
    print(f"Error: {error.message()}")
    sys.exit()
else:
    print("Device is already up to date.")
    sys.exit()
    
sys.exit(app.exec())
##! [FirmwareUpdate]