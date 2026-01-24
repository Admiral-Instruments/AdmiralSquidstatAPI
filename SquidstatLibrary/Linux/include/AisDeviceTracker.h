#ifndef SQUIDSTATLIBRARY_AISDEVICETRACKER_H
#define SQUIDSTATLIBRARY_AISDEVICETRACKER_H

#include "AisErrorCode.h"
#include "AisSquidstatGlobal.h"
#include <QObject>
#include <memory>

class AisDeviceTrackerPrivate;
class AisInstrumentHandler;

/**
 *
 * @ingroup InstrumentControl
 *
 * @brief This class is used track device connections to the computer.
 * It also provides instrument handlers specific to each connected device which provide control of the relevant device.
*/
class SQUIDSTATLIBRARY_EXPORT AisDeviceTracker final : public QObject
{
    Q_OBJECT
public:
    ~AisDeviceTracker() override;

    /**
     * @brief get the instance of the device tracker.
     * @return the static instance of the AisDeviceTracker
     */
    static AisDeviceTracker *Instance();

    /**
     * @brief Establish a connection with a device connected on a USB port via its name.
     * @details Move a device handler to the specified thread. All signals from that handler will then be emitted from that thread.
     * @param deviceName The name of the device to connect. (case sensitive)
     * @retval AisErrorCode::Success if the device was connected, or is currently active. No AisDeviceTracker::newDeviceConnected signal will be emitted if the device is already connected.
     * @retval AisErrorCode::DeviceNotFound if the device was not found on any comport or it is open in another application
     * @note This function will briefly open all available Squidstat serial connections and then close them. It may conflict with other applications attempting to initialize Squidstats.
     * @note emits newDeviceConnected() signal with the device name if establishing the connection was successful.
     */
    AisErrorCode connectToDevice(const QString& deviceName);

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
     * @param deviceName the name of the connected device to get the instrument handler for (case sensitive).
     * @return The instrument handler that controls the specified device. If the device has not been connected, it will return a stale device.
     * @note You may get a list of the connected devices using getConnectedDevices().
     * Also, whenever a device has been connected by calling connectToDeviceOnComPort(), a signal is emitted with the device name.
     * @note Device handlers will become stale if a device disconnects, and you will need to aquire new instances on reconnection.
     * @see AisInstrumentHandler
     * @see AisdeviceTracker::connectToDeviceOnComPort()
     * @see AisdeviceTracker::getConnectedDevices()
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
     * @param comport the communication port to connect through.
     * @return AisErrorCode::Success if firmware update successfully initiated through the given communication port.
     * If not successful, possible returned errors are:
     * - AisErrorCode::FirmwareUptodate
     * - AisErrorCode::ConnectionFailed
     * @note emits firmwareUpdateNotification() signal to provide firmware update progress.
     * @note You need to specify the communication port specific to your computer. For example, on PC, you may find your port number through the 'device manager'.
     * An example would be "COM15".
    */
    AisErrorCode updateFirmwareOnComPort(const QString& comport) const;

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

    /**
     * @brief Allow to collect device error message in file for debugging purpose.
     * @note by default it will be true.
     * @param save When set to 'false,' it will not write logs to the file. When set to 'true,' it will begin writing device error logs to the file.
     * @see setLogFilePath
    */
    void saveLogToFile(bool save);

    /**
     * @brief This will help to change the log file directory.
     * @note by default it will be Document/Admiral Instrument/API
     * @param path Set the path value at which you want to save the log file.
     * @note If you set 'false' for 'saveLogToFile,' it will not generate the log file. It is recommended to set it to 'true' or leave the permission as the default setting.
     * @see saveLogToFile
    */
    void setLogFilePath(const QString& path);

    /**
     * @brief Calling this function will remove all disconnected devices from memory. Any pointers and references to these devices will be invalidated.
     * @note This function should only be called after handling a disconnection event and removing all references to the disconnected device.
     * @note This is a temporary patch function to fix memory leaks over long periods of time with many disconnection events. In a future implementation this function will no longer be needed.
    */
    void purgeStaleDevices();


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
    void deviceDisconnected(const QString& deviceName);

    /**
     * @brief a signal which is emitted regularaly during a firmware update, providing information about the progress of the update.
     * @param message a string containing the progress percentage message.
     * @note If the device is in reset mode when the firmware is updated, the device name will be "Not Available" for all firmware update messaging.
     */
    void firmwareUpdateNotification(const QString& message);

    /**
     * @brief Emitted when a device finishes the firmware update process.
     * @param deviceName The name of the device which is finished updating.
     * @param success Indicates whether the firmware updated successfully.
     * @note If the device is in reset mode when the firmware is updated, the device name will be "Not Available" for all firmware update messaging.
     */
    void firmwareUpdateFinished(const QString& deviceName, bool success);

private:
    AisDeviceTracker();
    AisDeviceTracker(const AisDeviceTracker &);
    void operator=(const AisDeviceTracker &);


    AisDeviceTrackerPrivate* m_data = nullptr;
};

#endif