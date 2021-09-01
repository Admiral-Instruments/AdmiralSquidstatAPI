#ifndef SQUIDSTATLIBRARY_AISDEVICETRACKER_H
#define SQUIDSTATLIBRARY_AISDEVICETRACKER_H


#include <memory>
#include <QObject>
#include "AisSquidstatGlobal.h"

class AisDeviceTrackerPrivate;
class AisInstrumentHandler;

/**
 * @brief This class is used track device connections to the computer. It can establish connection with plugged-in devices. 
 * It also provides instrument handlers specific to each connected device which can provide control of the specific device like starting experiments.
*/
class SQUIDSTATLIBRARY_EXPORT AisDeviceTracker final: public QObject {
    Q_OBJECT
public:
    ~AisDeviceTracker() override;
    static AisDeviceTracker* Instance();

    /**
     * @brief establish a connection with a device connected on a USB port.
     * @param comPort the communication port to connect through.
     * @return true if a connection was established with the device through the given communication port.
     * @note emits newDeviceConnected() signal with the device name if establishing the connection was successful.
     * @note You need to specify the communication port specific to your computer. For example, on PC, you may find your port number through the 'device manager'.
     * An example would be "COM15".
    */
    bool connectToDeviceOnComPort(const QString& comPort);

    /**
     * @brief get an instrument handler to control a specific device.
     * @param deviceName the name of the connected device to get the instrument handler for.
     * @return the instrument handler that controls the specified device.
     * @note You may get a list of the connected devices using getConnectedDevices(). 
     * Also, whenever a device has been connected by calling connectToDeviceOnComPort(), a signal is emitted with the device name.
     * A signal and slot example is shown \ref basics "here".
     * @see AisInstrumentHandler
     * @see connectToDeviceOnComPort()
     * @see getConnectedDevices()
    */
    const AisInstrumentHandler& getInstrumentHandler(const QString& deviceName) const;

    /**
     * @brief get a list of all the connected devices.
     * @return a list of all the connected devices.
    */
    const std::list<QString> getConnectedDevices() const;

    /**
     * @brief connect all devices physically plugged to the computer.
     * 
     * This will automatically detect all the communication ports that have devices plugged in and establish a connection with each.
     * @return the number of devices that have successfully established a connection with the computer.
     * @note emits newDeviceConnected() signal with the device name for each successful connection.
    */
    int connectAllPluggedInDevices();

signals:
    /**
     * @brief a signal to be emitted whenever a new connection has been successfully established with a device.
     * @param deviceName the name of the newly connected device.
     * @note this signal will be emitted for each newly connected device whenever either connectToDeviceOnComPort() or connectAllPluggedInDevices() successfully established connections.
    */
    void newDeviceConnected(const QString& deviceName);

    /**
     * @brief a signal to be emitted whenever a device has been disconnected.
     * @param deviceName the name of the newly disconnected device.
    */
    void deviceDisconnected(const QString& deviceName);

private:
    AisDeviceTracker();
    AisDeviceTracker(const AisDeviceTracker&);
    void operator=(const AisDeviceTracker&);

    std::unique_ptr<AisDeviceTrackerPrivate> m_data;
};

#endif
