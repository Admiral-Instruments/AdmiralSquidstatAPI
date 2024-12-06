#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class SteppedCurrent;

/**
 * @brief A class representing an experiment to apply the stepped current. 
 * <br>
 * @image html SteppedCurrent.png
 * @image latex SteppedCurrent.png
*/
class SQUIDSTATLIBRARY_EXPORT AisSteppedCurrentElement final : public AisAbstractElement {
public:
    /**
    * @brief Constructs a Stepped Current element.
    *
    * This constructor initializes the Stepped Current element with the specified parameters.
    *
    * @param startCurrent The initial current value in amperes.
    * @param endCurrent The final current value in amperes.
    * @param stepSize The size of each current step in amperes.
    * @param stepDuration The duration of each current step in seconds.
    * @param samplingInterval The data sampling interval value in seconds.
    */
    explicit AisSteppedCurrentElement(
        double startCurrent,
        double endCurrent,
        double stepSize,
        double stepDuration,
        double samplingInterval);
    /**
* @brief copy constructor for the AisSteppedCurrentElement object.
*/
    explicit AisSteppedCurrentElement(const AisSteppedCurrentElement&);
    /**
* @brief overload equal to operator for the AisSteppedCurrentElement object.
*/
    AisSteppedCurrentElement& operator=(const AisSteppedCurrentElement&);

    ~AisSteppedCurrentElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "SteppedCurrent".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Galvanostatic Control").
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
     * @brief Gets the ending current value for the stepped experiment.
    *
    * @return The ending current value in amperes.
    */
    double getEndCurrent() const;

    /**
    * @brief Gets the size of each current step in the stepped experiment.
    *
    * @return The size of each current step in amperes.
    */
    double getStepSize() const;

    /**
    * @brief Gets the starting current value for the stepped experiment.
    *
    * @return The starting current value in amperes.
    */
    double getStartCurrent() const;

    /**
    * @brief Gets the duration of each current step in the stepped experiment.
    *
    * @return The duration of each current step in seconds.
    */
    double getStepDuration() const;

    /**
    * @brief Sets the ending current value for the stepped experiment.
    *
    * @param iEnd The ending current value in amperes.
    */
    void setEndCurrent(double iEnd);

    /**
    * @brief Sets the size of each current step in the stepped experiment.
    *
    * @param iStep The size of each current step in amperes.
    * @note Regardless of iStep's sign, the device will determine the step direction based on the start and end current.
    */
    void setStepSize(double iStep);

    /**
    * @brief Sets the starting current value for the stepped experiment.
    *
    * @param iStart The starting current value in amperes.
    */
    void setStartCurrent(double iStart);

    /**
    * @brief Sets the duration of each current step in the stepped experiment.
    *
    * @param tStep The duration of each current step in seconds.
    */
    void setStepDuration(double tStep);

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

    /**
    * @brief get the value set for the expected minimum voltage.
    * @return the value set for the expected maximum Voltage in volt.
    * @note if nothing was manually set, the device will auto-select the voltage range and the return value will be positive infinity.
    */
    double getApproxMinVoltage() const;

    /**
     * @brief set minimum voltage expected, for manual voltage range selection.
     *
     * This is an <strong>optional</strong> parameter.
     * If nothing is set, the device will auto-select the voltage range.
     * @param approxMaxVoltage the value for the maximum current expected in V.
     */
    void setApproxMinVoltage(double approxMinVoltage);

private:
    std::shared_ptr<SteppedCurrent> m_dataDerived;
};