
#ifndef AIS_ERROR_CODE_H
#define AIS_ERROR_CODE_H

#include "AisSquidstatGlobal.h"
#include <qstring.h>

/**
 * @brief This class contains the possible error codes returned to the user when working with the API.
 *
 * If a function has an AisErrorCode return type, then it needs to be checked for possible failures.
 * The object of this class returned will contain an error code that can be accessed by calling \ref value() member function
 * and an error message that can be accessed by calling @see message.
*/
class SQUIDSTATLIBRARY_EXPORT AisErrorCode {

public:
    enum ErrorCode : uint8_t {
        Unknown = 255, /*!< indicates that the command failed for an unknown reason.*/
        Success = 0, /*!< indicates success.*/
        ConnectionFailed = 1, /*!< indicates failure connecting the plugged in device when calling AisDeviceTracker::connectToDeviceOnComPort.*/
        FirmwareNotSupported = 2, /*!< indicates failure connecting the plugged in device when calling AisDeviceTracker::connectToDeviceOnComPort because firmware update require.*/
        FirmwareFileNotFound = 3,
        FirmwareUptodate = 4,

        InvalidChannel = 10, /*!< indicates that the given channel number is not valid.*/
        BusyChannel = 11, /*!< indicates that the failure was due to the channel being busy.*/
        DeviceNotFound = 13, /*!< indicates that no device was detected to be connected.*/

        ManualExperimentNotRunning = 51, /*!< indicates that the given command applies when there is a manual experiment running on the channel but there is none.*/
        ExperimentNotUploaded = 52, /*!< indicates that the given command applies when an experiment has already been uploaded to the channel but there is none.*/
        ExperimentIsEmpty = 53, /*!< indicates that the given experiment has no elements. It need to contain at least one.*/
        InvalidParameters = 54, /*!< indicates that a given parameter is invalid. For example, it is out of the allowed range.*/
        ChannelNotBusy = 55, /*!< indicates that the given command applies when there is an experiment running or paused on the channel but there is none.*/
        ExperimentUploaded = 56, /*!< indicates that the given command could not be completed because an experiment is already uploaded to the channel.*/

        DeviceCommunicationFailed = 100, /*!< indicates that there was failure in communication with the device.*/

        FailedToSetManualModeCurrentRange = 101, /*!< indicates failure to set manual mode current range due to a possible communication failure with the device.*/
        FailedToSetManualModeConstantVoltage = 102, /*!< indicates failure to set manual mode constant voltage due to a possible communication failure with the device*/
        FailedToPauseExperiment = 103, /*!< indicates that pausing the experiment failed because either there is no active experiment or due to a possible communication failure with the device.*/
        FailedToResumeExperiment = 104, /*!< indicates that resuming the experiment failed because either there is no paused experiment or due to a possible communication failure with the device.*/
        FailedToStopExperiment = 105, /*!< indicates that stopping the experiment failed because either there is no experiment running, the experiment is paused, or due to a possible communication failure with the device.*/
        FailedToUploadExperiment = 106, /*!< indicates failure to communicate with the device to upload the experiment.*/
        ExperimentAlreadyPaused = 107, /*!< indicates that pausing the experiment failed because the experiment is already paused.*/
        ExperimentAlreadyRun = 108, /*!< indicates that resuming the experiment failed because an experiment is already running.*/
        FailedToSetManualModeVoltageRange = 109, /* !< indicates failure to set manual mode voltage range due to a possible communication failure with the device.*/
        FailedToSetManualModeConstantCurrent = 110, /*!< indicates failure to set manual mode constant current due to a possible communication failure with the device.*/
        FailedToSetManualModeInOCP = 111, /*!< indicates failure of setting manual mode in open circuit mode for possible communication failure with the device.*/
        FailedToSetManualModeSamplingInterval = 112, /*!< indicates failure of setting manual mode sampling interval. possible communication failure with the device.*/
        FailedToSetIRComp = 113, /*!< indicates failure of setting IR Compensation. Possible communication failure with the device.*/
        FailedToSetCompRange = 114, /*!< indicates failure of setting Compensation Range. Possible communication failure with the device.*/

        FailedRequest = 254 /* !< indicates a failed request to the device. */
    };

    AisErrorCode();
    AisErrorCode(ErrorCode error);
    AisErrorCode(ErrorCode error, QString message);

    /**
     * @brief a function to get a message explaining the error.
     * @return a message that explains the error.
    */
    QString message() const;

    /**
     * @brief a function to get the error code.
     * @return the error code
    */
    int value() const;

    operator ErrorCode() const;

private:
    ErrorCode code;
    QString errorMessage;
};

#endif // ! AIS_ERROR_CODE_H
