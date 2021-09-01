#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class ConstantResistanceElement;

/**
 * @brief This element/experiment simulates a constant resistance load.
 * <br>
 * @image html ConstantResistance.png
 * @image latex ConstantResistance.png
*/
class SQUIDSTATLIBRARY_EXPORT AisConstantResistanceElement final : public AisAbstractElement {
public:

    /**
     * @brief the constant resistance element constructor.
     * @param resistance the value in ohm of the load resistance
     * @param duration the maximum duration for the experiment in seconds.
     * @param samplingInterval the data sampling interval value in seconds.
    */
    explicit AisConstantResistanceElement(
        double resistance,
        double duration,
        double samplingInterval
    );
    explicit AisConstantResistanceElement(const AisConstantResistanceElement&);
    AisConstantResistanceElement& operator= (const AisConstantResistanceElement&);

    ~AisConstantResistanceElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Constant Resistance".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Energy Storage", "Charge/Discharge").
    */
    QStringList getCategory() const override;

    /**
     * @brief get the value set for the resistance as a load.
     * @return the value in ohm of the load resistance.
    */
    double getResistance() const;

    /**
     * @brief set the value for the resistance as a load
     * @param resistance the value in ohm of the load resistance.
    */
    void setResistance(double resistance);

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
     * @brief get the value set minimum for the voltage in volts.
     * @return the value set for the minimum voltage in volts.
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity
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
    std::shared_ptr<ConstantResistanceElement> m_dataDerived;
};