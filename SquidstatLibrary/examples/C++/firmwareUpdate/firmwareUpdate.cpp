/** \example firmwareUpdate.cpp */
//! [FirmwareUpdate]
#include "AisInstrumentHandler.h"
#include "AisDeviceTracker.h"

#include <QCoreApplication>
#include <QThread>
#include <QDebug>

#define COMPORT "COM1"

int main()
{
    int args;
    QCoreApplication a(args, nullptr);

    auto tracker = AisDeviceTracker::Instance();

    //! [FirmwareUpdateNotification]
    QObject::connect(tracker, &AisDeviceTracker::firmwareUpdateNotification, [=](const QString& message) {
        qDebug() << message;
        if (message.contains("firmware is updated.")) {
            // Now instrument is ready to go
        }
    });
    //! [FirmwareUpdateNotification]

    // Attempt to connect to the device
    auto error = tracker->connectToDeviceOnComPort(COMPORT);
    if (error == AisErrorCode::FirmwareNotSupported) {
        error = tracker->updateFirmwareOnComPort(COMPORT);

        // Some other error occured
        if (error != AisErrorCode::Success) {
			qDebug() << "Error: " << error.message();
		}

    } else if (error != AisErrorCode::Success) {
        qDebug() << "Error: " << error.message();
    } else {
        qDebug() << "Device firmware is up to date";
    }

    return a.exec();
}
//! [FirmwareUpdate]