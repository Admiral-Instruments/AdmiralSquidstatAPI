#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class OpenCircuitElement;

/**
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
        double samplingInterval
    );
    explicit AisOpenCircuitElement(const AisOpenCircuitElement&);
    AisOpenCircuitElement& operator= (const AisOpenCircuitElement&);

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
     * The is an <strong>optional</strong> condition. 
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
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit voltage value.
     * If a maximum voltage is set, the experiment will continue to run as long as the measured voltage is above that value.
     * @param minVoltage the minimum voltage value in volts at which the experiment will stop.
    */
    void setMinVoltage(double minVoltage);

    /**
     * @brief get the value set for the minimum voltage rate of change with respect to time (minimum dV/dt).
     * @return the value set for the minimum voltage rate of change with respect to time (minimum dV/dt).
     * @note this is an optional parameter. If no value has been set, the default value is zero

    */
    double getMindVdt() const;

    /**
     * @brief set the minimum value for the voltage rate of change with respect to time (minimum dV/dt).
     * 
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit rate of change value.
     * If a minimum value is set, the experiment will continue to run as long as the rage of change is above that value.
     * @param mindVdt the minimum value for the voltage rate of change with respect to time (minimum dV/dt).
    */
    void setMindVdt(double mindVdt);

private:
    std::shared_ptr<OpenCircuitElement> m_dataDerived;
};