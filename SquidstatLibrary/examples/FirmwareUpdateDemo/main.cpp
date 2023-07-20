/** @page NonBlockTestDemo
 *
 *  @verbinclude main.cpp
 */
/*
* The Admiral Instruments API gives more control of the device and gives you the tools to integrate running
* our experiments in your pipeline and automating your workflow.
* Our API lets you programmatically update firmware of Device.
* 
*
* Below is an example for update the firmware programtically. 
*
*/

#include "AisDeviceTracker.h"
#include <QCoreApplication>
#include <qdebug.h>
#include <qfileinfo.h>




int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    auto tracker = AisDeviceTracker::Instance();
    QObject::connect(tracker, &AisDeviceTracker::firmwareUpdateNotification, &a, [=](const QString& message) {
           qInfo() << message;
        });

    if (argc == 1) { 
        auto numberOfDevices = tracker->updateFirmwareOnAllAvailableDevices();
        if (numberOfDevices == 0) {
            qInfo() << "No devices need to be updated at this time. If this is incorrect, ensure all devices are connected and powered on.";
        } else {
            qInfo() << "Firmware update starting on " << numberOfDevices << "devices.";
        }
        
    } else {

        if (argc == 2) {
            auto comPort = argv[1];
            
            QRegExp rx("^[Cc][Oo][Mm][0-9]+$");
            if (rx.exactMatch(comPort) == false) {
                qInfo() << " Arguments are not valid. Example: " << QFileInfo(QCoreApplication::applicationFilePath()).fileName() << " COM3";
          
            } else {
                auto error = tracker->updateFirmwareOnComPort(comPort);
                if (error) {
                    qInfo() << error.message();
                    
                }
            }
        }
    }

    a.exec();
}
