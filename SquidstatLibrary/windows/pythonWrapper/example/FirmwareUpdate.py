import sys
import struct
from PySide2.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker

app = QApplication()

tracker = AisDeviceTracker.Instance()

tracker.firmwareUpdateNotification.connect(lambda message: print("message: %s" % message))
nmberOfDevice = tracker.updateFirmwareOnAllAvailableDevices();
if (nmberOfDevice == 0):
    print ("Firmware update is not start in any of device");
else:
    print ("Firmware update start in "+str(nmberOfDevice) +"device.")



sys.exit(app.exec_())