#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class StaircasePotentialVoltammetryElement;

/**
 * @brief AisStaircasePotentialVoltammetryElement class represents an element for staircase potential voltammetry experiments.
 * It inherits from AisAbstractElement.
 * 
 * <br>
 * @image html StaircasePotentialVoltammetry.png
 * @image latex StaircasePotentialVoltammetry.png
 */
class SQUIDSTATLIBRARY_EXPORT AisStaircasePotentialVoltammetryElement final : public AisAbstractElement {
public:
    /**
     * @brief Constructs an AisStaircasePotentialVoltammetryElement with specified parameters.
     * @param startVoltage The starting voltage in volts.
     * @param firstVoltageLimit The first voltage limit in volts.
     * @param secondVoltageLimit The second voltage limit in volts.
     * @param endVoltage The ending voltage in volts.
     * @param stepSize The potential step size in volts.
     * @param stepDuration The potential step duration in seconds.
     * @param samplingInterval The potential sampling interval in seconds.
     */
    AisStaircasePotentialVoltammetryElement(double startVoltage,
        double firstVoltageLimit,
        double secondVoltageLimit,
        double endVoltage,
        double stepSize,
        double stepDuration,
        double samplingInterval);

    /**
     * @brief Copy constructor for AisStaircasePotentialVoltammetryElement.
     * @param other The AisStaircasePotentialVoltammetryElement to copy.
     */
    AisStaircasePotentialVoltammetryElement(const AisStaircasePotentialVoltammetryElement& other);

    /**
     * @brief Assignment operator for AisStaircasePotentialVoltammetryElement.
     * @param other The AisStaircasePotentialVoltammetryElement to assign.
     * @return Reference to this AisStaircasePotentialVoltammetryElement.
     */
    AisStaircasePotentialVoltammetryElement& operator=(const AisStaircasePotentialVoltammetryElement& other);

    /**
     * @brief Destructor for AisStaircasePotentialVoltammetryElement.
     */
    ~AisStaircasePotentialVoltammetryElement() override;

    /**
     * @brief Gets the name of the element.
     * @return The name of the element.
     */
    QString getName() const override;

    /**
     * @brief Gets the category of the element.
     * @return The category of the element.
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
     * @brief Gets the starting voltage.
     * @return The starting voltage in volts.
     */
    double getStartVoltage() const;

    /**
     * @brief Sets the starting voltage.
     * @param startVoltage The starting voltage to set in volts.
     */
    void setStartVoltage(double startVoltage);

    /**
     * @brief Checks if the starting voltage is with respect to the open circuit mode.
     * @return True if the starting voltage is with respect to the open circuit mode, false otherwise.
     */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief Sets whether the starting voltage is with respect to the open circuit mode.
     * @param startVsOCP True to set the starting voltage with respect to the open circuit mode, false otherwise.
     */
    void setStartVoltageVsOCP(bool startVsOCP);

    /**
     * @brief Gets the ending voltage.
     * @return The ending voltage in volts.
     */
    double getEndVoltage() const;

    /**
     * @brief Sets the ending voltage.
     * @param endVoltage The ending voltage to set in volts.
     */
    void setEndVoltage(double endVoltage);

    /**
     * @brief Checks if the ending voltage is with respect to the open circuit mode.
     * @return True if the ending voltage is with respect to the open circuit mode, false otherwise.
     */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief Sets whether the ending voltage is with respect to the open circuit mode.
     * @param endVsOCP True to set the ending voltage with respect to the open circuit mode, false otherwise.
     */
    void setEndVoltageVsOCP(bool endVsOCP);

    /**
     * @brief Gets the first voltage limit.
     * @return The first voltage limit in volts.
     */
    double getFirstVoltageLimit() const;

    /**
     * @brief Sets the first voltage limit.
     * @param firstVoltageLimit The first voltage limit to set in volts.
     */
    void setFirstVoltageLimit(double firstVoltageLimit);

    /**
     * @brief Checks if the first voltage limit is with respect to the open circuit mode.
     * @return True if the first voltage limit is with respect to the open circuit mode, false otherwise.
     */
    bool isFirstVoltageLimitVsOCP() const;

    /**
     * @brief Sets whether the first voltage limit is with respect to the open circuit mode.
     * @param firstVoltageLimitVsOCP True to set the first voltage limit with respect to the open circuit mode, false otherwise.
     */
    void setFirstVoltageLimitVsOCP(bool firstVoltageLimitVsOCP);

    /**
     * @brief Gets the second voltage limit.
     * @return The second voltage limit in volts.
     */
    double getSecondVoltageLimit() const;

    /**
     * @brief Sets the second voltage limit.
     * @param secondVoltageLimit The second voltage limit to set in volts.
     */
    void setSecondVoltageLimit(double secondVoltageLimit);

    /**
     * @brief Checks if the second voltage limit is with respect to the open circuit mode.
     * @return True if the second voltage limit is with respect to the open circuit mode, false otherwise.
     */
    bool isSecondVoltageLimitVsOCP() const;

    /**
     * @brief Sets whether the second voltage limit is with respect to the open circuit mode.
     * @param secondVoltageLimitVsOCP True to set the second voltage limit with respect to the open circuit mode, false otherwise.
     */
    void setSecondVoltageLimitVsOCP(bool secondVoltageLimitVsOCP);

    /**
     * @brief Gets the potential step size.
     * @return The potential step size in volts.
     */
    double getStepSize() const;

    /**
     * @brief Sets the potential step size.
     * @param stepSize The potential step size to set in volts.
     */
    void setStepSize(double stepSize);

    /**
     * @brief Gets the potential step duration.
     * @return The potential step duration in seconds.
     */
    double getStepDuration() const;

    /**
     * @brief Sets the potential step duration.
     * @param stepDuration The potential step duration to set in seconds.
     */
    void setStepDuration(double stepDuration);

    /**
     * @brief Gets the potential sampling interval.
     * @return The potential sampling interval in seconds.
     */
    double getSamplingInterval() const;

    /**
     * @brief Sets the potential sampling interval.
     * @param samplingInterval The potential sampling interval to set in seconds.
     */
    void setSamplingInterval(double samplingInterval);

    /**
     * @brief Checks if the experiment should autorange the current.
     * @return True if autorange is enabled, false otherwise.
     */
    bool isAutorange() const;

    /**
     * @brief Enables autorange for the experiment.
     */
    void setAutorange();

    /**
     * @brief Gets the approximate maximum current.
     * @return The approximate maximum current.
     */
    double getApproxMaxCurrent() const;

    /**
     * @brief Sets the approximate maximum current.
     * @param approxMaxCurrent The approximate maximum current to set.
     */
    void setApproxMaxCurrent(double approxMaxCurrent);

    /**
     * @brief get the value set for the number of cycles
     * @return the number of cycles set.
    */
    double getNumberOfCycles();

    /**
     * @brief set the number of cycles to oscillate between the first voltage-limit and the second voltage-limit.
     * @param cycles the number of cycles to set
    */
    void setNumberOfCycles(int cycles);

private:
    std::shared_ptr<StaircasePotentialVoltammetryElement> m_dataDerived; ///< Shared pointer to the StaircasePotentialVoltammetryElement data.
};
