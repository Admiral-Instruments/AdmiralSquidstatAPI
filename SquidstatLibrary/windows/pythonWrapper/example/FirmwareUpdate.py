import sys
from PySide2.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker

# initialize the application
app = QApplication()

# get a device tracker
tracker = AisDeviceTracker.Instance()

# quit firmware update when complete
def firmware_update_callback(message):
    print("message: %s" % message)
    if "progress 100" in message.lower():
        print("Firmware update completed. Quitting application.")
        app.quit()

# connect call back function which will trigger while updating firmware on device.
tracker.firmwareUpdateNotification.connect(firmware_update_callback)

# get number of devices the need a required firmware update
number_of_devices = tracker.updateFirmwareOnAllAvailableDevices()

# check if anything connected
if number_of_devices == 0:
    # print to console if no instruments were found
    print("Firmware update has not started on any device.")
else:
    # print number of devices to console that are being updated
    print("Firmware update started on " + str(number_of_devices) + " device(s).")

# quit app when complete
sys.exit(app.exec_())
