#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class ConstantPotAdvElement;

/**
 * @brief an experiment that simulates a constant applied voltage.
 * <br>
 * @image html ConstantPot.png
 * @image latex ConstantPot.png
*/
class SQUIDSTATLIBRARY_EXPORT AisConstantPotElement final : public AisAbstractElement {
public:
    /**
     * @brief the constant potential element constructor.
     * @param voltage the value set for the voltage/potential in volts.
     * @param samplingInterval the data sampling interval value in seconds.
     * @param duration the maximum duration for the experiment in seconds.
    */
    explicit AisConstantPotElement(
        double voltage,
        double samplingInterval,
        double duration);
    /**
     * @brief copy constructor for the AisConstantPotElement object.
    */
    explicit AisConstantPotElement(const AisConstantPotElement&);

    /**
     * @brief overload equal to operator for the AisConstantPotElement object.
    */
    AisConstantPotElement& operator=(const AisConstantPotElement&);

    ~AisConstantPotElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Constant Potential, Advanced".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Basic Experiments")
    */
    QStringList getCategory() const override;

    /**
     * @brief get the value set for the potential in volts.
     * @return the value set for the potential in volts.
    */
    double getPotential() const;

    /**
     * @brief set the value for the potential in volts.
     * @param potential the value to set for the potential in volts.
    */
    void setPotential(double potential);

    /**
     * @brief tells whether the specified voltage is set against the open-circuit voltage or the reference terminal.
     * @return true if the specified voltage is set against the open-circuit voltage and false if it is set against the reference terminal.
     * @see setVsOcp
    */
    bool isVoltageVsOCP() const;

    /**
     * @brief set whether to reference the specified voltage against the open-circuit voltage or the reference terminal.
     *
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param vsOCP true to set the specified voltage to reference the open-circuit voltage and false to set against the reference terminal.
    */
    void setVoltageVsOCP(bool vsOCP);

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
     * @brief get the maximum duration set for the experiment.
     * The experiment will end when the duration of the experiment reaches this value.
     * @return the maximum duration for the experiment in seconds.
    */
    double getMaxDuration() const;

    /**
     * @brief set the maximum duration for the experiment.
     *
     * This is an <strong>optional</strong> condition.
     * If nothing is set, then the experiment will not stop based on an duration.
     * If a maximum duration is set, the experiment will continue to run as long as the passed time is less than that value.
     * @param maxDuration the maximum duration for the experiment in seconds.
    */
    void setMaxDuration(double maxDuration);

    /**
     * @brief get the maximum value set for the absolute current in Amps.
     * The experiment will end when the absolute current reaches this value.
     * @return the maximum absolute current value in Amps.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity.
     */
    double getMaxAbsoluteCurrent() const;

    /**
     * @brief set the maximum value for the absolute current in Amps.
     *
     * This is an <strong>optional</strong> condition.
     * If nothing is set, then the experiment will not stop based on an upper-limit current value.
     * If a maximum absolute current is set, the experiment will continue to run as long as the absolute measured current is below that value.
     * @param maxCurrent the maximum absolute current value in Amps.
    */
    void setMaxAbsoluteCurrent(double maxCurrent);

    /**
     * @brief get the maximum value set for the absolute current in Amps.
     * The experiment will end when the absolute current reaches this value.
     * @return the maximum current value in Amps.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity.
     * @attention Deprecation Warning: This function may be modified or changed in a future version. Use getMaxAbsoluteCurrent instead.
    */
    [[deprecated("getMaxCurrent has been renamed getMaxAbsoluteCurrent for description accuracy. In future versions this function may be removed or modified.")]]
    double getMaxCurrent() const;

    /**
     * @brief set the maximum value for the absolute current in Amps.
     *
     * This is an <strong>optional</strong> condition.
     * If nothing is set, then the experiment will not stop based on an upper-limit current value.
     * If a maximum current is set, the experiment will continue to run as long as the measured current is below that value.
     * @param maxCurrent the maximum current value in Amps.
     * @attention Deprecation Warning: This function may be modified or changed in a future version. Use setMaxAbsoluteCurrent instead.
    */
    [[deprecated("setMaxCurrent has been renamed setMaxAbsoluteCurrent for description accuracy. In future versions this function may be removed or modified.")]]
    void setMaxCurrent(double maxCurrent);

    /**
     * @brief get the minimum value set for the absolute current in Amps.
     * The experiment will end when the absolute current falls down to this value.
     * @return the minimum absolute current value in Amps.
     * @note this is an optional parameter. If no value has been set, the default value is zero.
    */
    double getMinAbsoluteCurrent() const;

    /**
     * @brief set the minimum value for the absolute current in Amps.
     *
     * This is an <strong>optional</strong> condition.
     * If nothing is set, then the experiment will not stop based on an lower-limit current value.
     * If a minimum absolute current is set, the experiment will continue to run as long as the measured absolute current is above that value.
     * @param minCurrent the value to set for the minimum absolute current.
    */
    void setMinAbsoluteCurrent(double minCurrent);

    /**
     * @brief get the minimum value set for the absolute current in Amps.
     * The experiment will end when the absolute current falls down to this value.
     * @return the minimum absolute current value in Amps.
     * @note this is an optional parameter. If no value has been set, the default value is zero.
     * @attention Deprecation Warning: This function may be modified or changed in a future version. Use getMinAbsoluteCurrent instead.
    */
    [[deprecated("getMinCurrent has been renamed getMinAbsoluteCurrent for description accuracy. In future versions this function may be removed or modified.")]]
    double getMinCurrent() const;

    /**
     * @brief set the minimum value for the absolute current in Amps.
     *
     * This is an <strong>optional</strong> condition.
     * If nothing is set, then the experiment will not stop based on an lower-limit current value.
     * If a minimum absolute current is set, the experiment will continue to run as long as the measured absolute current is above that value.
     * @param minCurrent the value to set for the minimum absolute current.
     * @attention Deprecation Warning: This function may be modified or changed in a future version. Use setMinAbsoluteCurrent instead.
    */
    [[deprecated("setMinCurrent has been renamed setMinAbsoluteCurrent for description accuracy. In future versions this function may be removed or modified.")]]
    void setMinCurrent(double minCurrent);

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
     * @brief get the value set for the minimum current rate of change with respect to time (minimum di/dt).
     * @return the value set for the minimum current rate of change with respect to time (minimum di/dt).
     * @note this is an optional parameter. If no value has been set, the default value is zero.
    */
    double getMindIdt() const;

    /**
     * @brief set the minimum value for the current rate of change with respect to time (minimum di/dt).
     * 
     * This is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit rate of change value.
     * If a minimum value is set, the experiment will continue to run as long as the rage of change is above that value.
     * @param mindIdt the minimum value for the current rate of change with respect to time (minimum di/dt).
    */
    void setMindIdt(double mindIdt);

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
     * This is an <strong>optional</strong> parameter. 
     * If nothing is set, the device will auto-select the current range.
     * @param approxMaxCurrent the value for the maximum current expected in Amps.
    */
    void setApproxMaxCurrent(double approxMaxCurrent);

    /**
     * @brief get the value set for the voltage range.
     * @return the index set for the voltage range. (see AisInstrumentHandler::getManualModeVoltageRangeList())
     */
    int getVoltageRange() const;

    /**
     * @brief manually set the voltage control range.
     *
     * This is an <strong>optional</strong> parameter.
     * If this function is not called, the device will auto-select the voltage range by default.
     * @param idx the corresponding voltage range index (see AisInstrumentHandler::getManualModeVoltageRangeList())
    */
    void setVoltageRange(int idx);

private:
    std::shared_ptr<ConstantPotAdvElement> m_dataDerived;
};
