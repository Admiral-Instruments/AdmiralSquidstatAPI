#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class ConstantCurrentAdvElement;

/**
 *
 * @ingroup Elements
 * 
 * @brief an experiment that simulates a constant current flow with more advance options for stopping the experiment.
 * <br>
 * @image html ConstantCurrent.png
 * @image latex ConstantCurrent.png "ConstantCurrent"
*/
class SQUIDSTATLIBRARY_EXPORT AisConstantCurrentElement final : public AisAbstractElement {
public:
    /**
     * @brief the constant current element constructor.
     * @param current the value for the current in Amps.
     * @param samplingInterval the data sampling interval value in seconds.
     * @param duration the maximum duration for the experiment in seconds.
    */
    explicit AisConstantCurrentElement(
        double current,
        double samplingInterval,
        double duration);
    /**
* @brief copy constructor for the AisConstantCurrentElement object.
*/
    explicit AisConstantCurrentElement(const AisConstantCurrentElement&);
    /**
* @brief overload equal to operator for the AisConstantCurrentElement object.
*/
    AisConstantCurrentElement& operator=(const AisConstantCurrentElement&);

    ~AisConstantCurrentElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Constant Current, Advanced".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Galvanostatic Control", "Basic Experiments").
    */
    QStringList getCategory() const override;

    /**
     * @brief get the value set for the current.
     * @return the value for the current in Amps.
    */
    double getCurrent() const;

    /**
     * @brief set the value for the current.
     * @param current the value for the current in Amps.
    */
    void setCurrent(double current);

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
     * @brief get the minimum sampling voltage difference for reporting the data.
     *
     * get the value set for the minimum sampling voltage difference.
     * @return the value set for the minimum sampling voltage difference.
     * @see setMinSamplingVoltageDifference
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity.
    */
    double getMinSamplingVoltageDifference() const;

    /**
    * @brief set a minimum sampling voltage difference for reporting the voltage.
    *
    * This is an <strong>optional</strong> condition.
    * If nothing is set, then the experiment will report the data at time sampling interval.
    * When this is set, then the voltage is reported when there is a voltage difference of at least the given minimum sampling voltage difference.
    * So, when one voltage data point is reported (at the minimum possible time sampling interval), the next data point is not reported unless the difference
    * between the two voltage data points exceeds this given minimum sampling voltage difference value.

    * @note when this is set, this overrides the set value for the sampling interval.
    * @param minVoltageDifference the minimum sampling voltage difference value in volts.
   */
    void setMinSamplingVoltageDifference(double minVoltageDifference);

    /**
     * @brief get the value set for the maximum voltage.
     * The experiment will end when it reaches this value.
     * @return the value set for the maximum voltage.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity
    */
    double getMaxVoltage() const;

    /**
     * @brief set a maximum voltage to stop the experiment.
     * 
     * This is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an upper-limit voltage value.
     * If a maximum voltage is set, the experiment will continue to run as long as the measured voltage is below that value.
     * @param maxVoltage the maximum voltage value in volts at which the experiment will stop.
    */
    void setMaxVoltage(double maxVoltage);

    /**
     * @brief get the value set minimum for the voltage in volts.
     * @return the value set for the minimum voltage in volts.
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity
    */
    double getMinVoltage() const;

    /**
     * @brief set a minimum voltage to stop the experiment.
     * 
     * This is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit voltage value.
     * If a maximum voltage is set, the experiment will continue to run as long as the measured voltage is above that value.
     * @param minVoltage the minimum voltage value in volts at which the experiment will stop.
    */
    void setMinVoltage(double minVoltage);

    /**
     * @brief get the maximum duration set for the experiment.
     * The experiment will end when the duration of the experiment reaches this value.
     * @return the maximum duration for the experiment in seconds.
    */
    double getMaxDuration() const;

    /**
     * @brief set the maximum duration for the experiment.
     * 
     * The experiment will continue to run as long as the time passed is less than the value to set.
     * @param maxDuration the maximum duration for the experiment in seconds.
    */
    void setMaxDuration(double maxDuration);

    /**
     * @brief get the value set for the maximum capacity / cumulative charge.
     * @return the value set for the maximum capacity in Coulomb.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity.
    */
    double getMaxCapacity() const;

    /**
     * @brief set the value for the maximum capacity / cumulative charge in Coulomb.
     * 
     * This is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an upper-limit cumulative charge value.
     * If a maximum capacity is set, the experiment will continue to run as long as the cumulative charge is below that value.
     * @param maxCapacity the value to set for the cell maximum capacity.
    */
    void setMaxCapacity(double maxCapacity);

    /**
     * @brief tells whether the current range is set to auto-select or not.
     * @return true if the current range is set to auto-select and false if a range has been selected.
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
     * This is an <strong>optional</strong> parameter. 
     * If nothing is set, the device will auto-select the current range.
     * @param approxMaxCurrent the value for the maximum current expected in Amps.
    */
    void setApproxMaxCurrent(double approxMaxCurrent);

    /**
     * @brief tells whether the voltage range is set to auto-select or not.
     * @return true if the voltage range is set to auto-select and false if a range has been selected.
    */
    bool isAutoVoltageRange() const;

    /**
     * @brief set to auto-select the voltage range.
     *
     * This option is set by default. There is no need to call this function to auto-select if the range was not manually set.
    */
    void setAutoVoltageRange();


    /**
    * @brief get the value set for the expected maximum voltage.
    * @return the value set for the expected maximum Voltage in volt.
    * @note if nothing was manually set, the device will auto-select the voltage range and the return value will be positive infinity.
   */
    double getApproxMaxVoltage() const;

    /**
     * @brief set maximum voltage expected, for manual voltage range selection.
     *
     * This is an <strong>optional</strong> parameter.
     * If nothing is set, the device will auto-select the voltage range.
     * @param approxMaxVoltage the value for the maximum current expected in V.
    */
    void setApproxMaxVoltage(double approxMaxVoltage);

private:
    std::shared_ptr<ConstantCurrentAdvElement> m_dataDerived;
};