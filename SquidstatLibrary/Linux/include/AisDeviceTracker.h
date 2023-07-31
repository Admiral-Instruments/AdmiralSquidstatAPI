#ifndef SQUIDSTATLIBRARY_AISDEVICETRACKER_H
#define SQUIDSTATLIBRARY_AISDEVICETRACKER_H

#include "AisErrorCode.h"
#include "AisSquidstatGlobal.h"
#include <QObject>
#include <memory>



class AisDeviceTrackerPrivate;
class AisInstrumentHandler;

/**
 * @brief This class is used track device connections to the computer. It can establish connection with plugged-in devices.
 * It also provides instrument handlers specific to each connected device which can provide control of the specific device like starting experiments.
*/
class SQUIDSTATLIBRARY_EXPORT AisDeviceTracker final : public QObject
{
    Q_OBJECT
public:
    ~AisDeviceTracker() override;
    static AisDeviceTracker *Instance();

    /**
     * @brief establish a connection with a device connected on a USB port.
     * @param comPort the communication port to connect through.
     * @return AisErrorCode::Success if a connection was established with the device through the given communication port.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::Unknown
     * - AisErrorCode::FirmwareNotSupported
     * - AisErrorCode::ConnectionFailed
     * @note emits newDeviceConnected() signal with the device name if establishing the connection was successful.
     * @note You need to specify the communication port specific to your computer. For example, on PC, you may find your port number through the 'device manager'.
     * An example would be "COM15".
    */
    AisErrorCode connectToDeviceOnComPort(const QString &comPort);

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
    const AisInstrumentHandler &getInstrumentHandler(const QString &deviceName) const;

    /**
     * @brief get a list of all the connected devices.
     * @return a list of all the connected devices.
    */
    const std::list<QString> getConnectedDevices() const;

    /**
     * @brief connect all devices physically plugged to the computer.
     *
     * This will automatically detect all the communication ports that have devices plugged in and establish a connection with each.
     * @return the number of <em>new</em> devices that have successfully established a connection with the computer.
     * If a device has already been connected before calling this function, it will not be counted in the return value.
     * @note emits newDeviceConnected() signal with the device name for each successful connection.
    */
    int connectAllPluggedInDevices();

    /**
     * @brief update firmware on connected device at USB port.
     * @param comPort the communication port to connect through.
     * @return AisErrorCode::Success if firmware update successfully initiated through the given communication port.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FirmwareUptodate
     * - AisErrorCode::ConnectionFailed
     * @note emits firmwareUpdateNotification() signal to provide firmware update progress.
     * @note You need to specify the communication port specific to your computer. For example, on PC, you may find your port number through the 'device manager'.
     * An example would be "COM15".
    */

    AisErrorCode updateFirmwareOnComPort(QString comport) const;

    /**
     * @brief request firmware update for all available devices.
     *
     * This will automatically detect devices not currently in use and update firmware if necessary.
     * @return the number of devices that have successfully requested for firmware update.
     * If a device has already been updated firmware before calling this function, it will not be counted in the return value.
     * If any error is generated while requesting firmware update, it will not be counted in the return value.
     * @note emits firmwareUpdateNotification() signal will provide notification regarding firmware update of all devices.
     * @note You can update firmware when you reset the device physically through reset button.
     * @see updateFirmwareOnComPort
    */
    int updateFirmwareOnAllAvailableDevices();

signals:
    /**
     * @brief a signal to be emitted whenever a new connection has been successfully established with a device.
     * @param deviceName the name of the newly connected device.
     * @note this signal will be emitted for each newly connected device whenever either connectToDeviceOnComPort() or connectAllPluggedInDevices() successfully established connections.
    */
    void newDeviceConnected(const QString &deviceName);

    /**
     * @brief a signal to be emitted whenever a device has been disconnected.
     * @param deviceName the name of the newly disconnected device.
    */
    void deviceDisconnected(const QString &deviceName);

    void firmwareUpdateNotification(const QString& message);
    
private:
    AisDeviceTracker();
    AisDeviceTracker(const AisDeviceTracker &);
    void operator=(const AisDeviceTracker &);

    std::unique_ptr<AisDeviceTrackerPrivate> m_data;

   
};

#endif
