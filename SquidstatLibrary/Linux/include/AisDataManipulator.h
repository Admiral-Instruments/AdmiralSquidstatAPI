#ifndef AISDATAMANIPULATOR_H
#define AISDATAMANIPULATOR_H

#include "AisDataPoints.h"
#include "AisErrorCode.h"
#include "AisManipulatorType.h"
#include "AisSquidstatGlobal.h"
#include <iostream>
#include <string>
#include <memory>

class AisDataManipulatorPrivate;
class AisDiffPulseVoltammetryElement;
class AisNormalPulseVoltammetryElement;
class AisSquareWaveVoltammetryElement;

/**
 *
 * @ingroup Helpers
 *
 * @brief This class offers advanced control over pulse data collection and manipulation. It provides methods to manipulate AIS primary data for all three pulse voltammetry experiments types, namely Differential Pulse Voltammetry (DPV), Square Wave Voltammetry (SWV), and Normal Pulse Voltammetry (NPV). 
 */
class SQUIDSTATLIBRARY_EXPORT AisDataManipulator
{
public:
    /**
     * @brief Default constructor for AisDataManipulator class.
     */
    AisDataManipulator();

    /**
     * @brief AisDiffPulseVoltammetryElement constructor for AisDataManipulator class.
     * @param other The AisDiffPulseVoltammetryElement tile to be executed
     */
    AisDataManipulator(const AisDiffPulseVoltammetryElement& other);

    /**
     * @brief AisNormalPulseVoltammetryElement constructor for AisDataManipulator class.
     * @param other the AisNormalPulseVoltammetryElement tile to be executed
     */
    AisDataManipulator(const AisNormalPulseVoltammetryElement& other);

    /**
     * @brief AisSquareWavePulseVoltammetryElement constructor for AisDataManipulator class.
     * @param other AisSquareWaveVoltammetryElement tile to be executed
     */
    AisDataManipulator(const AisSquareWaveVoltammetryElement& other);

    /**
     * @brief Set pulse type with pulse width and pulse period.
     * @note This function is usefull only for DifferentialPulse and NormalPulse.
     * @param type The type of pulse.
     * @param pulseWidth The width of the pulse.
     * @param pulsePeriod The period of the pulse.
     * @return AisErrorCode::Success if pulse setting was successful.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedRequest
     */
    AisErrorCode setPulseType(AisPulseType type, double pulseWidth, double pulsePeriod);

    /**
     * @brief Set pulse type with frequency.
     * @note This function is usefull only for SquarewavePulse.
     * @param type The type of pulse.
     * @param frequency The frequency of the pulse.
     * @return AisErrorCode::Success if pulse setting was successful.
     * If not successful, possible returned errors are: 
     * - AisErrorCode::FailedRequest
     */
    AisErrorCode setPulseType(AisPulseType type, double frequency);

    /**
     * @brief Get the pulse width.
     * @return The pulse width.
     */
    double getPulseWidth() const;

    /**
     * @brief Get the pulse period.
     * @return The pulse period.
     */
    double getPulsePeriod() const;

    /**
     * @brief Get the pulse frequency.
     * @return The pulse frequency.
     */
    double getFrequency() const;

    /**
     * @brief Check if the pulse is completed.
     * @return True if the pulse is completed, false otherwise.
     */
    bool isPulseCompleted() const;

    /**
     * @brief Get the base current.
     * @return The base current.
     */
    double getBaseCurrent() const;

    /**
     * @brief Get the pulse current.
     * @return The pulse current.
     */
    double getPulseCurrent() const;

    /**
     * @brief Get the base voltage.
     * @return The base voltage.
     */
    double getBaseVoltage() const;

    /**
     * @brief Get the pulse voltage.
     * @return The pulse voltage.
     */
    double getPulseVoltage() const;

    /**
     * @brief Load primary data from AisDCData object.
     * @param data The AisDCData object containing primary data.
     */
    void loadPrimaryData(const AisDCData& data);

private:
    std::shared_ptr<AisDataManipulatorPrivate> m_data; ///< Private implementation object.
};

#endif // AISDATAMANIPULATOR_H
