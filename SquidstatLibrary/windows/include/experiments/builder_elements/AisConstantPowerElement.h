#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class ConstantPowerElement;

/**
 * @brief This experiment simulates a constant power, charge or discharge".
 * <br>
 * @image html ConstantPower.png
 * @image latex ConstantPower.png
*/
class SQUIDSTATLIBRARY_EXPORT AisConstantPowerElement final : public AisAbstractElement {
public:

    /**
     * @brief the constant power element constructor
     * @param isCharge true to set the experiment simulate charge and false to simulate discharge.
     * @param power the value set for the power in watts.
     * @param duration the maximum duration for the experiment in seconds.
     * @param smaplingInterval the data sampling interval value in seconds.
    */
    explicit AisConstantPowerElement(
        bool isCharge, 
        double power,
        double duration,
        double smaplingInterval
    );
    explicit AisConstantPowerElement(const AisConstantPowerElement&);
    AisConstantPowerElement& operator= (const AisConstantPowerElement&);

    ~AisConstantPowerElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Constant Power Charge/Discharge".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Energy Storage", "Charge/Discharge").
    */
    QStringList getCategory() const override;

    /**
     * @brief tells whether the experiment is set to simulate charge or discharge.
     * @return true if the experiment is set to simulate charge and false if it is set to simulate discharge.
    */
    bool isCharge() const;

    /**
     * @brief set whether the experiment is to simulate charge or discharge.
     * @param isCharge if the given argument is true, the experiment will simulate charge and discharge if given false.
    */
    void setCharge(bool isCharge);

    /**
     * @brief get the value set for the power.
     * @return the value set for the power in watts.
    */
    double getPower() const;

    /**
     * @brief set the value for the power.
     * @param power the value set for the power in watts.
    */
    void setPower(double power);

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
      * @brief get the value set for the maximum voltage.
      * The experiment will end when it reaches this value.
      * @return the value set for the maximum voltage.
      * @note this is an optional parameter. If no value has been set, the default value is positive infinity
     */
    double getMaxVoltage() const;

    /**
     * @brief set a maximum voltage to stop the experiment.
     *
     * The is an <strong>optional</strong> condition.
     * If nothing is set, then the experiment will not stop based on an upper-limit voltage value.
     * If a maximum voltage is set, the experiment will continue to run as long as the measured voltage is below that value.
     * @param maxVoltage the maximum voltage value in volts at which the experiment will stop.
    */
    void setMaxVoltage(double maxVoltage);

    /**
     * @brief get the minimum value set for the voltage in volts.
     * The experiment will end when it reaches down this value.
     * @return the minimum value set for the voltage in volts.
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity
    */
    double getMinVoltage() const;

    /**
     * @brief set a minimum value for the voltage.
     * The experiment will end when it reaches down this value.
     * @param minVoltage the value for the voltage in volts.
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity.
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
     * The experiment will continue to run as long as the passed time is less than that the set duration value.
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
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an upper-limit cumulative charge value.
     * If a maximum capacity is set, the experiment will continue to run as long as the cumulative charge is below that value.
     * @param maxCapacity the value to set for the cell maximum capacity.
    */
    void setMaxCapacity(double maxCapacity);

private:
    std::shared_ptr<ConstantPowerElement> m_dataDerived;
};