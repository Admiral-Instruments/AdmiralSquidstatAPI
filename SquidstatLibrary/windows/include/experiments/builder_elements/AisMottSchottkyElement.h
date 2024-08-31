#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class MottSchottkyElement;

/**
 * @brief This class performs Mott-Schottky analysis on the working electrode for a specified range of potentials.
 *
 * The Mott-Schottky experiment is used to analyze the electrochemical properties of a working electrode by sweeping
 * the potential from a starting potential to an ending potential in discrete steps. At each potential step,
 * Electrochemical Impedance Spectroscopy (EIS) is performed with varying frequencies to gather impedance data.
 *
 * The experiment can be configured with various parameters, including the starting and ending potentials,
 * voltage step size, starting and ending frequencies, steps per frequency decade, AC excitation amplitude,
 * and the minimum number of cycles per frequency.
 *
 * @note The potential sweep is done in a staircase manner, with each step consisting of a quiet time followed by an EIS measurement.
 * 
 * <br>
 * @image html MottSchottky.png
 * @image latex MottSchottky.png "MottSchottky"
 */
class SQUIDSTATLIBRARY_EXPORT AisMottSchottkyElement final : public AisAbstractElement {
public:
    /**
     * @brief Constructor for the Mott-Schottky experiment element.
     * @param startingPotential The starting potential (voltage) for the experiment.
     * @param endingPotential The ending potential (voltage) for the experiment.
     * @param voltageStep The voltage step size between each potential during the experiment.
     * @param startFrequency The starting frequency for the EIS measurement.
     * @param endFrequency The ending frequency for the EIS measurement.
     * @param stepsPerDecade The number of frequency steps per decade.
     * @param amplitude The amplitude of the AC signal used in the EIS measurement.
     * @param minCycles The minimum number of cycles per frequency step during the EIS measurement.
     */
    explicit AisMottSchottkyElement(double startingPotential, double endingPotential, double voltageStep,
                                    double startFrequency, double endFrequency, double stepsPerDecade,
                                    double amplitude, double minCycles);

    /**
     * @brief Copy constructor for the AisMottSchottkyElement object.
     * @param other The object to copy from.
     */
    explicit AisMottSchottkyElement(const AisMottSchottkyElement& other);

    /**
     * @brief Assignment operator for the AisMottSchottkyElement object.
     * @param other The object to assign from.
     * @return A reference to the assigned object.
     */
    AisMottSchottkyElement& operator=(const AisMottSchottkyElement& other);

    /**
     * @brief Destructor for the AisMottSchottkyElement object.
     */
    ~AisMottSchottkyElement() override;

    /**
     * @brief Get the name of the experiment element.
     * @return The name of the element, "Mott-Schottky".
     */
    QString getName() const override;

    /**
     * @brief Get a list of applicable categories of the experiment element.
     * @return A list of categories where the experiment is applicable, such as "Advanced Experiments".
     */
    QStringList getCategory() const override;

    // Getter and Setter methods

    /**
     * @brief Get the starting potential for the experiment.
     * @return The starting potential in volts.
     */
    double getStartingPotential() const;

    /**
     * @brief Set the starting potential for the experiment.
     * @param startingPotential The starting potential in volts.
     */
    void setStartingPotential(double startingPotential);

    /**
     * @brief Get the ending potential for the experiment.
     * @return The ending potential in volts.
     */
    double getEndingPotential() const;

    /**
     * @brief Set the ending potential for the experiment.
     * @param endingPotential The ending potential in volts.
     */
    void setEndingPotential(double endingPotential);

    /**
     * @brief Get the voltage step size between each potential.
     * @return The voltage step size in volts.
     */
    double getVoltageStep() const;

    /**
     * @brief Set the voltage step size between each potential.
     * @param voltageStep The voltage step size in volts.
     */
    void setVoltageStep(double voltageStep);

    /**
     * @brief Get the starting frequency for the EIS measurement.
     * @return The starting frequency in Hz.
     */
    double getStartFrequency() const;

    /**
     * @brief Set the starting frequency for the EIS measurement.
     * @param startFrequency The starting frequency in Hz.
     */
    void setStartFrequency(double startFrequency);

    /**
     * @brief Get the ending frequency for the EIS measurement.
     * @return The ending frequency in Hz.
     */
    double getEndFrequency() const;

    /**
     * @brief Set the ending frequency for the EIS measurement.
     * @param endFrequency The ending frequency in Hz.
     */
    void setEndFrequency(double endFrequency);

    /**
     * @brief Get the number of frequency steps per decade.
     * @return The number of steps per decade.
     */
    double getStepsPerDecade() const;

    /**
     * @brief Set the number of frequency steps per decade.
     * @param stepsPerDecade The number of steps per decade.
     */
    void setStepsPerDecade(double stepsPerDecade);

    /**
     * @brief Get the amplitude of the AC signal used in the EIS measurement.
     * @return The AC amplitude in volts.
     */
    double getAmplitude() const;

    /**
     * @brief Set the amplitude of the AC signal used in the EIS measurement.
     * @param amplitude The AC amplitude in volts.
     */
    void setAmplitude(double amplitude);

    /**
     * @brief Get the minimum number of cycles per frequency step.
     * @return The minimum number of cycles.
     */
    double getMinCycles() const;

    /**
     * @brief Set the minimum number of cycles per frequency step.
     * @param minCycles The minimum number of cycles.
     */
    void setMinCycles(double minCycles);

    /**
     * @brief Get the quiet time before starting the EIS measurement.
     * @return The quiet time in seconds.
     */
    double getQuietTime() const;

    /**
     * @brief Set the quiet time before starting the EIS measurement.
     * @param quietTime The quiet time in seconds.
     */
    void setQuietTime(double quietTime);

    /**
     * @brief Get the sampling interval during the quiet time.
     * @return The sampling interval in seconds.
     */
    double getQuietTimeSampInterval() const;

    /**
     * @brief Set the sampling interval during the quiet time.
     * @param quietTimeSampInterval The sampling interval in seconds.
     */
    void setQuietTimeSampInterval(double quietTimeSampInterval);

    /**
     * @brief Get the quiet time after each potential step before starting the EIS measurement.
     * @return The quiet time after each potential step in seconds.
     */
    double getStepQuietTime() const;

    /**
     * @brief Set the quiet time after each potential step before starting the EIS measurement.
     * @param stepQuietTime The quiet time after each potential step in seconds.
     */
    void setStepQuietTime(double stepQuietTime);

    /**
     * @brief Get the sampling interval during the quiet time after each potential step.
     * @return The sampling interval in seconds.
     */
    double getStepQuietSampInterval() const;

    /**
     * @brief Set the sampling interval during the quiet time after each potential step.
     * @param stepQuietTimeSampInterval The sampling interval in seconds.
     */
    void setStepQuietSampInterval(double stepQuietTimeSampInterval);

    /**
     * @brief Check if the starting voltage is measured versus the open circuit potential (OCP).
     * @return True if the starting voltage is measured versus OCP, false otherwise.
     */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief Set whether the starting voltage is measured versus the open circuit potential (OCP).
     * @param startVsOCP True if the starting voltage is measured versus OCP, false otherwise.
     */
    void setStartVoltageVsOCP(bool startVsOCP);

    /**
     * @brief Check if the ending voltage is measured versus the open circuit potential (OCP).
     * @return True if the ending voltage is measured versus OCP, false otherwise.
     */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief Set whether the ending voltage is measured versus the open circuit potential (OCP).
     * @param endVsOCP True if the ending voltage is measured versus OCP, false otherwise.
     */
    void setEndVoltageVsOCP(bool endVsOCP);

private:
    std::shared_ptr<MottSchottkyElement> m_dataDerived; ///< Pointer to the derived data element.
};
