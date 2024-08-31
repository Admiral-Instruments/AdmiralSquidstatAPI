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
#include <regex> 


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    auto tracker = AisDeviceTracker::Instance();
    QObject::connect(tracker, &AisDeviceTracker::firmwareUpdateNotification, &a, [=](const QString& message) {
        qInfo() << message;
        });

    if (argc == 1) {
        auto nmberOfDevice = tracker->updateFirmwareOnAllAvailableDevices();
        if (nmberOfDevice == 0) {
            qInfo() << "Firmware update did not start on any device";
        }
        else {
            qInfo() << "Firmware update started on " << nmberOfDevice << " device(s).";
        }
    }
    else {
        if (argc == 2) {
            std::string comPort = argv[1];

            
            std::regex rx("^[Cc][Oo][Mm][0-9]+$");
            if (!std::regex_match(comPort, rx)) {
                qInfo() << "Arguments are not valid. Example:" << QFileInfo(QCoreApplication::applicationFilePath()).fileName() << " COM3";
            }
            else {
                auto error = tracker->updateFirmwareOnComPort(QString::fromStdString(comPort));
                if (error) {
                    qInfo() << error.message();
                }
            }
        }
    }

    return a.exec();
}
