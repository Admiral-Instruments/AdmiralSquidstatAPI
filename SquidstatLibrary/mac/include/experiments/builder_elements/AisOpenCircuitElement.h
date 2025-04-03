#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class OpenCircuitElement;

/**
 *
 * @ingroup Elements
 * 
 * @brief This experiment observes the <strong>open circuit potential</strong> of the working electrode for a specific period of time.
 * <br>
 * @image html OpenCircuitPotential.png
 * @image latex OpenCircuitPotential.png
*/
class SQUIDSTATLIBRARY_EXPORT AisOpenCircuitElement final : public AisAbstractElement {
public:
    /**
     * @brief the open-circuit element constructor.
     * @param duration the maximum duration for the experiment in seconds.
     * @param samplingInterval the data sampling interval value in seconds.
    */
    explicit AisOpenCircuitElement(
        double duration,
        double samplingInterval);
    /**
* @brief copy constructor for the AisOpenCircuitElement object.
*/
    explicit AisOpenCircuitElement(const AisOpenCircuitElement&);
    /**
* @brief overload equal to operator for the AisOpenCircuitElement object.
*/
    AisOpenCircuitElement& operator=(const AisOpenCircuitElement&);

    ~AisOpenCircuitElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Open Circuit Potential".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Basic Experiments").
    */
    QStringList getCategory() const override;

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
     * @brief get the value set for the duration of the experiment.
     * @return the value set for the duration of the experiment in seconds.
    */
    double getMaxDuration() const;

    /**
     * @brief set the value set for the duration of the experiment.
     * @param maxDuration the value to set for the duration of the experiment in seconds.
    */
    void setMaxDuration(double maxDuration);

    /**
     * @brief get the value set for the maximum voltage.
     * The experiment will end when it reaches this value.
     * @return the value set for the maximum voltage.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity.
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
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity.
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
     * @brief get the value set for the minimum voltage rate of change with respect to time (minimum dV/dt).
     * @return the value set for the minimum voltage rate of change with respect to time (minimum dV/dt).
     * @note this is an optional parameter. If no value has been set, the default value is zero.
    */
    double getMindVdt() const;

    /**
     * @brief set the minimum value for the voltage rate of change with respect to time (minimum dV/dt).
     * 
     * This is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit rate of change value.
     * If a minimum value is set, the experiment will continue to run as long as the rage of change is above that value.
     * @param mindVdt the minimum value for the voltage rate of change with respect to time (minimum dV/dt).
    */
    void setMindVdt(double mindVdt);

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
     * @param approxMaxVoltage the value for the maximum voltage expected in V.
    */
    void setApproxMaxVoltage(double approxMaxVoltage);

private:
    std::shared_ptr<OpenCircuitElement> m_dataDerived;
};