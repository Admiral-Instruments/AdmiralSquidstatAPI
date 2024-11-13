#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class DCPotentialSweepElement;

/**
 * @brief this experiment performs a DC potential sweep from the <strong>starting current</strong> to the <strong>ending current</strong>
 * which progresses linearly according to the <strong>scan rate</strong>.
 * 
 * <br>
 * @image html DCPotentialSweep.png
 * @image latex DCPotentialSweep.png
*/
class SQUIDSTATLIBRARY_EXPORT AisDCPotentialSweepElement final : public AisAbstractElement {
public:
    /**
     * @brief the potential sweep element constructor.
     * @param startPotential the value of the starting potential in volts
     * @param endPotential the value of the ending potential in volts
     * @param scanRate the voltage scan rate in V/s
     * @param samplingInterval how frequently we are sampling the data. 
    */
    explicit AisDCPotentialSweepElement(
        double startPotential,
        double endPotential,
        double scanRate,
        double samplingInterval);
    /**
    * @brief copy constructor for the AisDCPotentialSweepElement object.
   */
    explicit AisDCPotentialSweepElement(const AisDCPotentialSweepElement&);
    /**
   * @brief overload equal to operator for the AisDCPotentialSweepElement object.
   */
    AisDCPotentialSweepElement& operator=(const AisDCPotentialSweepElement&);

    ~AisDCPotentialSweepElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "DC Potential Linear Sweep".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Basic Experiments").
    */
    QStringList getCategory() const override;

    /**
    * @brief Gets the quiet time duration
    * @return The quiet time duration in seconds.
    */
    double getQuietTime() const;

    /**
    * @brief Sets the quiet time duration.
    * @param quietTime The quiet time duration to set in seconds.
    */
    void setQuietTime(double quietTime);

    /**
     * @brief gets the quiet time sampling interval.
     * @return samplingInterval The quiet time sampling interval to set in seconds.
     */
    double getQuietTimeSamplingInterval() const;

    /**
    * @brief Sets the quiet time sampling interval.
    * @param quietTimeSamplingInterval The quiet time sampling interval to set in seconds.
    */
    void setQuietTimeSamplingInterval(double quietTimeSamplingInterval);

    /**
     * @brief get the value set for the starting potential.
     * @return the value of the starting potential in volts.
    */
    double getStartingPot() const;

    /**
     * @brief set the value for the starting potential.
     * @param startingPotential the value of the starting potential in volts
    */
    void setStartingPot(double startingPotential);

    /**
     * @brief tells whether the starting potential is set against the open-circuit voltage or the reference terminal.
     * @return true if the starting potential is set against the open-circuit voltage and false if it is set against the reference terminal.
     * @see setStartVoltageVsOCP
    */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief set whether to reference the starting potential against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param startVoltageVsOCP  true to if the starting potential is set to reference the open-circuit voltage and false if set against the reference terminal.
     * @note by default, this is set to false.
    */
    void setStartVoltageVsOCP(bool startVoltageVsOCP);

    /**
     * @brief get the value set for the ending potential value.
     * 
     * This is the value of the voltage at which the experiment will stop.
     * @return the value set for the ending voltage in volts.
    */
    double getEndingPot() const;

    /**
     * @brief set the ending potential value.
     * 
     * This is the value of the voltage at which the experiment will stop.
     * @param endingPotential the value to set for the ending potential in volts.
    */
    void setEndingPot(double endingPotential);

    /**
     * @brief tells whether the end voltage is set with respect to the open circuit voltage or the reference terminal.
     * @return true if the end voltage is set with respect to the open-circuit voltage and false if set against the reference terminal.
     * @see setEndVoltageVsOCP
    */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief set whether to reference the end voltage against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param endVoltageVsOCP true to set the end voltage to be referenced against the open-circuit voltage and false if set against the reference terminal.
     * @note by default, this is set to false.
    */
    void setEndVoltageVsOCP(bool endVoltageVsOCP);

    /**
     * @brief get the value set for the voltage scan rate.
     * @return the value set for the voltage scan rate in V/s
     * @see setScanRate
    */
    double getScanRate() const;

    /**
     * @brief set the value for the voltage scan rate.
     * 
     * The scan rate represents the value of the discrete voltage step size in one second in the linear sweep.
     * @param scanRate the value to set for the scan rate.
    */
    void setScanRate(double scanRate);

    /**
     * @brief get how frequently we are sampling the data.
     * @return the data sampling interval value in seconds.
    */
    double getSamplingInterval() const;

    /**
     * @brief set how frequently we are sampling the data.
     * @param samplingInterval the data sampling interval value in seconds.
    */
    void setSamplingInterval(double samplingInterval);

    /**
     * @brief tells whether the current range is set to auto-select or not.
     * @return true if the current range is set to auto-select and false if a rage has been selected.
    */
    bool isAutoRange() const;

    /**
     * @brief set to auto-select the current range.
     * 
     * This option is set by default. There is no need to call this function to auto-select if the range was not manually set.
    */
    void setAutoRange();

    /**
     * @brief get the value set for the expected maximum current.
     * @return the value set for the expected maximum current in Amps.
     * @note if nothing was manually set, the device will auto-select the current range and the return value will be positive infinity.
    */
    double getApproxMaxCurrent() const;

    /**
     * @brief set maximum current expected, for manual current range selection.
     * 
     * The is an <strong>optional</strong> parameter. 
     * If nothing is set, the device will auto-select the current range.
     * @param approxMaxCurrent the value for the maximum current expected in Amps.
    */
    void setApproxMaxCurrent(double approxMaxCurrent);


     /**
     * @brief get the value set for the maximum Current.
     * The experiment will end when it reaches this value.
     * @return the value set for the maximum Current.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity
    */
    double getMaxAbsoluteCurrent() const;

    /**
     * @brief set a maximum Current to stop the experiment.
     * 
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an upper-limit Current value.
     * If a maximum Current is set, the experiment will continue to run as long as the measured Current is below that value with the harware current limitation.
     * @param maxCurrent the maximum Current value in volts at which the experiment will stop.
    */
    void setMaxAbsoluteCurrent(double maxCurrent);

    /**
     * @brief get the value set minimum for the Current in amps.
     * @return the value set for the minimum Current in amps.
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity
    */
    double getMinAbsoluteCurrent() const;

    /**
     * @brief set a minimum Current to stop the experiment.
     * 
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit Current value.
     * If a maximum Current is set, the experiment will continue to run as long as the measured voltage is above that value.
     * @param minCurrent the minimum Current value in volts at which the experiment will stop.
    */
    void setMinAbsoluteCurrent(double minCurrent);

    /**
     * @brief Get the value set for the alpha factor
     *
     * @return The value for the alpha factor is represented as a percent between 0 and 100.
     * @note If nothing is set, this function will return a default value of 75.
    */
    double getAlphaFactor() const;

    /**
     * @brief alpha factor controls the percentage of data sampled during a given interval. Data will be averaged over the last n% of the sampling interval.
     *
     * The is an <strong>optional</strong> parameter.
     * If nothing is set, the device will use the default value of 75.
     * @param alphaFactor the value for the alphaFactor ranges from 0 to 100.
    */
    void setAlphaFactor(double alphaFactor);

private:
    std::shared_ptr<DCPotentialSweepElement> m_dataDerived;
};