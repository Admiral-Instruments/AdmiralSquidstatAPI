#ifndef SQUIDSTATLIBRARY_AISINSTRUMENTHANDLER_H
#define SQUIDSTATLIBRARY_AISINSTRUMENTHANDLER_H

#include <ctime>

#include <QObject>

#include "AisCompRange.h"
#include "AisDataPoints.h"
#include "AisErrorCode.h"
#include "AisSquidstatGlobal.h"

class AisInstrumentHandlerPrivate;
class AisExperiment;

    /**
 * @brief this class provides control of the device including starting, pausing, resuming and stopping an experiment on a channel
 * as well as reading the data and other controls of the device. 
 * 
 * You may get an instrument handler instance of this class by calling AisDeviceTracker::getInstrumentHandler
 * where you can get the device name either by calling AisDeviceTracker::getConnectedDevices
 * or whenever the signal newDeviceConnected() is emitted.
*/
class SQUIDSTATLIBRARY_EXPORT AisInstrumentHandler final : public QObject {
    Q_OBJECT
    AisInstrumentHandlerPrivate* m_data;
public:
    /// @private
    explicit AisInstrumentHandler(AisInstrumentHandlerPrivate* privateData);
    /// @private
    ~AisInstrumentHandler();

    /**
     * @brief upload an already created custom experiment to a specific channel on the device.
     * 
     * Any running experiment is run on a specific device on a specific channel. 
     * This function uploads an experiment to a channel so that you may start, pause, resume and stop the experiment.
     * All of these four control functionalities and others require a channel number to control the experiment. 
     * Therefore, if we have several channels, we need to keep track of which experiment is on which channel.
     * 
     * @param channel the channel number to upload the experiment to.
     * @param experiment the custom experiment to be uploaded to the channel.
     * @return AisErrorCode::Success if the experiment was successfully uploaded to the channel. 
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedToUploadExperiment
     * - AisErrorCode::ExperimentIsEmpty
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::BusyChannel
     * - AisErrorCode::InvalidParameters
     * 
     * This returns AisErrorCode::Success only when given a valid channel number that is not busy on a connected device.
     * @see isChannelBusy
    */
    AisErrorCode uploadExperimentToChannel(uint8_t channel, std::shared_ptr<AisExperiment> experiment) const;

    /**
    * @brief upload an already created custom experiment to a specific channel on the device.
    *
    * Any running experiment is run on a specific device on a specific channel.
    * This function uploads an experiment to a channel so that you may start, pause, resume and stop the experiment.
    * All of these four control functionalities and others require a channel number to control the experiment.
    * Therefore, if we have several channels, we need to keep track of which experiment is on which channel.
    *
    * @param channel the channel number to upload the experiment to.
    * @param experiment the custom experiment to be uploaded to the channel.
    * @return AisErrorCode::Success if the experiment was successfully uploaded to the channel.
    * If not successful, possible returned errors are:
    * - AisErrorCode::FailedToUploadExperiment
    * - AisErrorCode::ExperimentIsEmpty
    * - AisErrorCode::DeviceNotFound
    * - AisErrorCode::InvalidChannel
    * - AisErrorCode::BusyChannel
    * - AisErrorCode::InvalidParameters
    *
    * This returns AisErrorCode::Success only when given a valid channel number that is not busy on a connected device.
    * @see isChannelBusy
    */
    AisErrorCode uploadExperimentToChannel(uint8_t channel, const AisExperiment& experiment) const;

    /**
     * @brief start the previously uploaded experiment on the specific channel. 
     * @param channel the channel number to start the experiment on.
     * @return AisErrorCode::Success if the experiment was successfully started on the channel.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::DeviceCommunicationFailed
     * - AisErrorCode::ExperimentNotUploaded
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::BusyChannel
     * @see uploadExperimentToChannel
     * @see isChannelBusy
    */
    AisErrorCode startUploadedExperiment(uint8_t channel) const;

    /**
     * @brief skip the current experiment step and proceed to the next.
     * 
     * When running an element that has several steps like going from CC to CV, 
     * then skipping the step goes to the next step within the element.
     * When having several elements in the custom experiment and the current element has one step or we are at the last step within the element,
     * then skipping the step results in going to the next element. If this is the final step of the final element, the experiment will stop.
     * @param channel a valid channel number with an experiment to skip the step.
     * @return AisErrorCode::Success the experiment step was successfully skipped 
     * If not successful, possible returned errors are: 
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::ChannelNotBusy
     * - AisErrorCode::DeviceCommunicationFailed

    */
    AisErrorCode skipExperimentStep(uint8_t channel) const;

    /**
     * @brief pause a running experiment on the channel.
     * @param channel the channel number to pause the experiment on.
     * @return true if an experiment was successfully paused on the channel and false otherwise.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedToPauseExperiment
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::ChannelNotBusy
     * 
     * This will return AisErrorCode::Success only if there is currently a running experiment on a valid channel on a connected device.
    */
    AisErrorCode pauseExperiment(uint8_t channel) const;

    /**
     * @brief resume a paused experiment on the channel.
     * @param channel the channel number to resume the experiment on.
     * @return AisErrorCode::Success if an experiment was successfully resumed on the channel.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedToResumeExperiment
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::ChannelNotBusy
     * 
     * This will return AisErrorCode::Success only if there is currently a paused experiment on a valid channel on a connected device.
    */
    AisErrorCode resumeExperiment(uint8_t channel) const;

    /**
     * @brief stop a running or a paused experiment on the channel.
     * @param channel the channel number to stop the experiment on.
     * @return AisErrorCode::Success if an experiment was successfully stopped on the channel.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedToStopExperiment
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * 
     * This will only return AisErrorCode::Success if there is currently a running or a paused experiment on a valid channel on a connected device.
    */
    AisErrorCode stopExperiment(uint8_t channel) const;

    /**
     * @brief get UTC time for the start of the experiment in seconds.
     *
     * This will give the time in seconds between the origin of UTC time and the start of the experiment aka Unix Epoch.
     * @param channel the channel for which to get the start time of the experiment.
     * @return the Unix Epoch up to the start of the experiment in seconds.
    */
    double getExperimentUTCStartTime(uint8_t channel) const;

    /**
     * @brief set IR compensation.
     * @param channel the channel for which to set the IR compensation.
     * @param uncompensatedResistance the value of the uncompensated resistance in Ohms.
     * @param compensationLevel the compensation percentage (0%-100%). This is unit-less.
     * @return AisErrorCode::Success if setting the IR compensation was successful.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::InvalidParameters
     * 
    */
    AisErrorCode setIRComp(uint8_t channel, double uncompensatedResistance, double compensationLevel) const;

    /**
     * @brief set a compensation range with stability factor and bandwidth index.
     * @param channel the channel for which to set the compensation range.
     * @param compRange an object of type compRange that is initialized with a stability factor (0-10) and a bandwidth index (0-10).
     * @return AisErrorCode::Success if setting the IR compensation was successful.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::InvalidParameters
     * @see AisCompRange
    */
    AisErrorCode setCompRange(uint8_t channel, const AisCompRange& compRange) const;

    /**
     * @brief connect several channels together in parallel mode.
     * 
     * You may connect a list of channels so you can get a higher combined output current of all channels. 
     * Note that this is only applicable to the cycler model.
     * @param channels a list of channels to be linked.
     * @return the master channel out of the given list of channels. 
     * The master channel is your interface to upload an experiment to and then control it.
     * @note this functionality is only applicable to the cycler model.
    */
    int8_t setLinkedChannels(std::vector<uint8_t> channels) const;

    /**
     * @brief connect two channels together in bipolar mode.
     *
     * You may combine two channels to expand the voltage range to include negative voltages.
     * Note that this is only applicable to the cycler model. For 4 channel Cycler models, you can combine channels 1 and 2 or channels 3 and 4. You cannot use any other channel combinations.
     * @param channels a list of two channels to be oprate in bipolar mode.
     * @return the master channel out of the given list of two channels.
     * The master channel is your interface to upload an experiment to and then control it.
     * If not successful set in bipolar mode, possible returned errors as -1.
     * @note this functionality is only applicable to the cycler model.
    */
    int8_t setBipolarLinkedChannels(std::vector<uint8_t> channels) const;


    /**
     * @brief tells whether the given channel is bipolar mode
     * @param channel the channel number to check if it is bipolar mode
     * @return true only if given a valid channel number that has bipolar mode.
    */
    bool hasBipolarMode(uint8_t channel) const;

    /**
     * @brief get a list of channels linked to the given channel.
     * @param channel a valid channel number to find which other channels are linked to it.
     * @return a list of channels linked to the channel parameter.
    */
    std::vector<uint8_t> getLinkedChannels(uint8_t channel) const;

    /**
     * @brief tells whether the given channel is busy or not.
     * @param channel the channel number to check if it is busy or not.
     * @return true only if given a valid channel number that has either a running or a paused experiment.
    */
    bool isChannelBusy(uint8_t channel) const;

    /**
     * @brief tells whether the given channel has a paused experiment or not.
     * @param channel the channel number to check if it has a paused experiment.
     * @return true only if given a valid channel number that has an experiment that has been paused.
    */
    bool isChannelPaused(uint8_t channel) const;

    /**
     * @brief get a list of the currently free channels.
     * @return a list of the currently free channels. If all channels are busy, an empty list is returned.
    */
    std::vector<uint8_t> getFreeChannels() const;

    /**
     * @brief get the number of all the channels on this device.
     * @return the number of channels on the connected device. If no device found, -1 will be returned.
    */
    int getNumberOfChannels() const;

    /**
     * @brief delete the recover data from device.
     * @return AisErrorCode::Success if request is sucessfully send for delete the data.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::DeviceCommunicationFailed
    */
    AisErrorCode eraseRecoverData() const;

    /**
     * @brief start a manual experiment.
     * 
     * With manual experiments, users can turn on any connected channel and toggle between open circuit mode and 
     * voltage or current setpoints that can be changed in real-time and run for indefinite periods. 
     * @param channel a valid channel number to run the manual experiment on.
     * @return AisErrorCode::Success if the manual experiment was successfully started.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedManualModeStartExperiment
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::BusyChannel
    */
    AisErrorCode startManualExperiment(uint8_t channel) const;

    /**
     * @brief set an interval for sampling the data.
     * @param channel the channel to set the sampling interval for.
     * @param value the value for the sampling interval in seconds.
     * @return AisErrorCode::Success if the operation was set successfully.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::Unknown
     * - AisErrorCode::InvalidChannel
    */
    AisErrorCode setManualModeSamplingInterval(uint8_t channel, double value) const;

    /**
     * @brief set open-circuit potential mode.
     * 
     * To apply the set potential or current, leave the open circuit potential mode off.
     * This operation is reversed automatically when calling either setManualModeConstantVoltage() or setManualModeConstantCurrent()
     * @param channel a valid channel number to set open circuit mode on.
     * @return AisErrorCode::Success if turning on the open circuit mode was successful.
     * If not successful, possible returned errors are:
     * - AisErrorCode::ManualExperimentNotRunning
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::DeviceCommunicationFailed

    */
    AisErrorCode setManualModeOCP(uint8_t channel) const;

    /**
     * @brief set constant voltage for the manual experiment.
     * @param channel a valid channel number to set a constant voltage for.
     * @param value the value to set the constant voltage in volts.
     * @return AisErrorCode::Success if setting the constant voltage was successful.
     * If not successful, possible returned errors are:
     * - AisErrorCode::FailedToSetManualModeConstantVoltage
     * - AisErrorCode::ManualExperimentNotRunning
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
    */
    AisErrorCode setManualModeConstantVoltage(uint8_t channel, double value) const;

    /**
     * @brief set constant voltage for the manual experiment and also set a manual current range.
     * @param channel a valid channel number to set a constant voltage for.
     * @param value the value to set the constant voltage in volts.
     * @param currentRangeIndex the index of the desired current range.
     * @return AisErrorCode::Success if setting the constant voltage was successful.
     * You can get a list of the available ranges for your model by calling \ref getManualModeCurrentRangeList.
     * If not successful, possible returned errors are:
     * - AisErrorCode::FailedToSetManualModeConstantVoltage
     * - AisErrorCode::FailedToSetManualModeCurrentRange
     * - AisErrorCode::ManualExperimentNotRunning
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
    */
    AisErrorCode setManualModeConstantVoltage(uint8_t channel, double value, int currentRangeIndex) const;

    /**
     * @brief set constant current for the manual experiment.
     * @param channel a valid channel number to set a constant voltage for.
     * @param value the value to set the constant current in Amps.
     * @return AisErrorCode::Success if setting the constant current was successful.
     * If not successful, possible returned errors are:
     * - AisErrorCode::ManualExperimentNotRunning
     * - AisErrorCode::DeviceNotFound
     * - AisErrorCode::InvalidChannel
     * - AisErrorCode::DeviceCommunicationFailed

    */
    AisErrorCode setManualModeConstantCurrent(uint8_t channel, double value) const;

    /**
     * @brief get a list of the applicable current ranges to the given channel specific to your device.
     * 
     * The list is indexed, with each index containing a range with minimum and maximum current for the range.
     * You can pass the index of the desired current range to setManualModeConstantVoltage. 
     * @param channel a valid channel number for which to check the current range.
     * @return a list of the of the applicable current ranges to the given channel specific to your device.
    */
    std::vector<std::pair<double, double>> getManualModeCurrentRangeList(uint8_t channel) const;

signals:

    /**
     * @brief a signal that is emitted if the device associated with this handler has been disconnected.
     * 
    */
    void deviceDisconnected();

    /**
     * @brief a signal that is emitted when the floating ground connection state has changed.
     * @param grounded true if there is a connection to ground and false if the ground has disconnected.
    */
    void groundFloatStateChanged(bool grounded);

    /**
     * @brief a signal that is emitted whenever a new elemental experiment has started.
     * @param channel the channel number on which the experiment was started.
     * @param stepInfo information regarding the current step.
     * @see AisExperimentNode
    */
    void experimentNewElementStarting(uint8_t channel, const AisExperimentNode& stepInfo);

    /**
     * @brief a signal that is emitted whenever new DC data for an active experiment are ready.
     * @param channel the channel number from which the DC data arrived.
     * @param DCData the DC data that just arrived.
    */
    void activeDCDataReady(uint8_t channel, const AisDCData& DCData);

    /**
     * @brief a signal that is emitted whenever new DC data are ready when the device is in an idle state.
     * 
     * A manual experiment displays real time values. These values are displayed even if the channel does not have an experiment running on it. 
     * @param channel the channel number from which the DC data arrived.
     * @param DCData the DC data that just arrived.
    */
    void idleDCDataReady(uint8_t channel, const AisDCData& DCData);

    /**
     * @brief a signal that is emitted whenever new DC recovery data are ready.
     * @param channel the channel number from which the DC data are recovered from.
     * @param DCData the DC data that just arrived.
    */
    void recoveryDCDataReady(uint8_t channel, const AisDCData& DCData);

    /**
     * @brief a signal that is emitted whenever new AC data for an active experiment are ready.
     * @param channel the channel number from which the AC data arrived.
     * @param ACData the AC data that just arrived.
    */
    void activeACDataReady(uint8_t channel, const AisACData& ACData);

    /**
     * @brief a signal that is emitted whenever new AC recovery data are ready.
     * @param channel the channel number from which the AC data are recovered from.
     * @param ACData the AC data that just arrived.
    */
    void recoveryACDataReady(uint8_t channel, const AisACData& ACData);

    /**
     * @brief a signal that is emitted whenever an experiment was stopped manually or has completed.
     * @param channel the channel on which the experiment has stopped.
    */
    void experimentStopped(uint8_t channel);

    /**
     * @brief a signal that is emitted whenever an experiment was paused.
     * @param channel the channel on which the experiment was paused.
    */
    void experimentPaused(uint8_t channel);

    /**
     * @brief a signal that is emitted whenever an experiment was resumed.
     * @param channel the channel on which the experiment was resumed.
    */
    void experimentResumed(uint8_t channel);

    /**
     * @brief a signal that is emitted whenever data erase process is completed.
     * @param successful is true on erased correctly, and false on data is not erased.
    */
    void recoverDataErased(bool successful);

private slots:
    void onActiveExperimentNodeBeginning(uint8_t channel, const AisExperimentNode&);
    void onRecoveryExperimentNodeBeginning(uint8_t channel, const AisExperimentNode&);
    void onDeviceDisconnected();

private:
    void connectWithOperatorSignals();
};

#endif //SQUIDSTATLIBRARY_AISINSTRUMENTHANDLER_H
