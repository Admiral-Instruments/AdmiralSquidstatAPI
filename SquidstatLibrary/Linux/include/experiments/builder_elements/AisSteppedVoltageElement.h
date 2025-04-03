#pragma once

#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class SteppedVoltage;

/**
 *
 * @ingroup Elements
 * 
 * @brief A class representing an experiment to apply the stepped volatge. 
 *
 * This class inherits from AisAbstractElement and is designed for Stepped Voltage experiments.
 * 
 *
 * @image html SteppedPotential.png
 * @image latex SteppedPotential.png
 */

class SQUIDSTATLIBRARY_EXPORT AisSteppedVoltageElement final : public AisAbstractElement {
public:
    /**
    * @brief Constructor for the AisSteppedVoltageElement element.
    *
    * This constructor initializes the AisSteppedVoltageElement element with the specified parameters.
    *
    * @param startVoltage The initial voltage value in volts.
    * @param endVoltage The final voltage value in volts.
    * @param voltageStep The size of each voltage step in volts.
    * @param voltageStepDuration The duration of each voltage step in seconds.
    * @param samplingInterval The data sampling interval value in seconds.
    */
    explicit AisSteppedVoltageElement(double startVoltage,
        double endVoltage,
        double voltageStep,
        double voltageStepDuration,
        double samplingInterval);

    /**
     * @brief Copy constructor for the AisSteppedVoltageElement object.
     * @param other The AisSteppedVoltageElement object to be copied.
     */
    explicit AisSteppedVoltageElement(const AisSteppedVoltageElement& other);

    /**
     * @brief Overloaded assignment operator for the AisSteppedVoltageElement object.
     * @param other The AisSteppedVoltageElement object to be assigned.
     * @return A reference to the assigned AisSteppedVoltageElement object.
     */
    AisSteppedVoltageElement& operator=(const AisSteppedVoltageElement& other);

    /**
     * @brief Destructor for the AisSteppedVoltageElement object.
     */
    ~AisSteppedVoltageElement() override;

    /**
     * @brief Get the name of the element.
     * @return The name of the element: "Stepped Voltage".
     */
    QString getName() const override;

    /**
     * @brief Get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control").
     */
    QStringList getCategory() const override;

    /**
     * @brief Get the starting voltage for the experiment.
     * @return The starting voltage in volts.
     */
    double getStartVoltage() const;

    /**
     * @brief Set the starting voltage for the experiment.
     * @param vStart The starting voltage in volts.
     */
    void setStartVoltage(double vStart);

    /**
     * @brief Get the ending voltage for the experiment.
     * @return The ending voltage in volts.
     */
    double getEndVoltage() const;

    /**
     * @brief Set the ending voltage for the experiment.
     * @param vEnd The ending voltage in volts.
     */
    void setEndVoltage(double vEnd);

    /**
     * @brief Get the voltage step for each iteration.
     * @return The voltage step in volts.
     */
    double getStepSize() const;
    
    /**
     * @brief Set the voltage step for each iteration.
     * @param vStep The voltage step in volts.
     * @note Regardless of vStep's sign, the device will determine the step direction based on the start and end voltage.
     */
    void setStepSize(double vStep);

    /**
     * @brief Get the time step for each iteration.
     * @return The time step in seconds.
     */
    double getStepDuration() const;

    /**
     * @brief Set the duration of each step.
     * @param duration The step duration in seconds.
     */
    void setStepDuration(double duration);

    /**
     * @brief Get the data sampling interval.
     * @return The data sampling interval in seconds.
     */
    double getSamplingInterval() const;

    /**
     * @brief Set the data sampling interval.
     * @param samplingInterval The data sampling interval in seconds.
     */
    void setSamplingInterval(double samplingInterval);

    /**
     * @brief Get the approximate maximum current.
     * @return The approximate maximum current in Amps.
     */
    double getApproxMaxCurrent() const;

    /**
     * @brief Set the approximate maximum current.
     * @param approxMaxCurrent The approximate maximum current in Amps.
     */
    void setApproxMaxCurrent(double approxMaxCurrent);

    /**
     * @brief Check if the experiment starts with the open circuit potential.
     * @return True if the experiment starts with open circuit potential, false otherwise.
     */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief Set whether the experiment starts with the open circuit potential.
     * @param startVsOCP True to start with open circuit potential, false otherwise.
     */
    void setStartVoltageVsOCP(bool startVsOCP);

    /**
     * @brief Check if the experiment ends with the open circuit potential.
     * @return True if the experiment ends with open circuit potential, false otherwise.
     */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief Set whether the experiment ends with the open circuit potential.
     * @param endVsOCP True to end with open circuit potential, false otherwise.
     */
    void setEndVoltageVsOCP(bool endVsOCP);

    /**
     * @brief Check if current autoranging is enabled.
     * @return True if current autoranging is enabled, false otherwise.
     */
    bool isAutoRange() const;
    
    /**
     * @brief Enable current autoranging for the experiment.
     */
    void setCurrentAutorange();

private:
    std::shared_ptr<SteppedVoltage> m_dataDerived; ///< Shared pointer to the SteppedVoltage data.
};

