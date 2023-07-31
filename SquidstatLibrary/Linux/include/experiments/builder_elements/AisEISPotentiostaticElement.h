#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class EISPotentiostaticElement;

/**
 * @brief This experiment records the complex impedance of the experimental cell in potentiostatic mode,
 * starting from the <strong>start frequency</strong> and sweeping through towards the <strong>end frequency</strong>,
 * with a fixed number of frequency <strong>steps per decade</strong>. 
 * 
 * Important parameters include the <strong>DC bias</strong> and the <strong>AC excitation amplitude</strong>.
 * 
 * <br>
 * @image html EISPotentiostatic.png
 * @image latex EISPotentiostatic.png
 */
class SQUIDSTATLIBRARY_EXPORT AisEISPotentiostaticElement final : public AisAbstractElement {
public:
    /**
     * @brief the EIS potentiostatic element
     * @param startFrequency the value for the voltage starting frequency
     * @param endFrequency the value for the voltage ending frequency
     * @param stepsPerDecade the value for the voltage frequency steps per decade.
     * @param voltageBias the value for the DC bias (DC offset).
     * @param voltageAamplitude the AC voltage amplitude.
    */
    explicit AisEISPotentiostaticElement(
        double startFrequency,
        double endFrequency,
        double stepsPerDecade,
        double voltageBias,
        double voltageAamplitude);
    /**
 * @brief copy constructor for the AisEISPotentiostaticElement object.
*/
    explicit AisEISPotentiostaticElement(const AisEISPotentiostaticElement&);
    /**
* @brief overload equal to operator for the AisEISPotentiostaticElement object.
*/
    AisEISPotentiostaticElement& operator=(const AisEISPotentiostaticElement&);

    ~AisEISPotentiostaticElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Potentiostatic EIS".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Impedance Methods", "Basic Experiments").
    */
    QStringList getCategory() const override;

    /**
     * @brief get the value set for the voltage starting frequency
     * @return the value set for the start frequency in Hz
    */
    double getStartFreq() const;

    /**
     * @brief set the value for the voltage starting frequency.
     * @param startFreq the value to set the starting frequency Hz
    */
    void setStartFreq(double startFreq);

    /**
     * @brief the value set for the voltage ending frequency.
     * @return the value set for the voltage end frequency in Hz
    */
    double getEndFreq() const;

    /**
     * @brief set the value for the voltage end frequency.
     * @param endFreq the value to set for the voltage end frequency in Hz
    */
    void setEndFreq(double endFreq);

    /**
     * @brief get the value set for the voltage frequency steps per decade.
     * @return the value set for the frequency steps per decade. This is unit-less.
    */
    double getStepsPerDecade() const;

    /**
     * @brief set the number of the voltage frequency steps per decade.
     * @param stepsPerDecade the value to set for the number of steps per decade.
    */
    void setStepsPerDecade(double stepsPerDecade);

    /**
     * @brief get the value set for the DC bias (DC offset).
     * @return the value set for the DC bias in volts.
    */
    double getBiasVoltage() const;

    /**
     * @brief set the value for the DC bias (DC offset).
     * @param biasVoltage the value to set for the DC bias in volts.
    */
    void setBiasVoltage(double biasVoltage);

    /**
     * @brief tells whether the DC-bias voltage is referenced against the open-circuit voltage or the reference cable.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @return true if the DC-bias voltage is set against the open-circuit voltage and false if it is set against the reference terminal.
    */
    bool isBiasVoltageVsOCP() const;

    /**
     * @brief set whether to reference the DC-bias voltage against the open-circuit voltage or the reference terminal.
     * @param biasVsOCP true to if the DC-bias voltage is set to reference the open-circuit voltage and false if set against the reference terminal.
    */
    void setBiasVoltageVsOCP(bool biasVsOCP);

    /**
     * @brief the value to set for the AC voltage amplitude.
     * @return the value set for the AC voltage amplitude in volts.
    */
    double getAmplitude() const;

    /**
     * @brief set the value for the AC voltage amplitude.
     * @param amplitude the value to set for the AC voltage amplitude in volts.
    */
    void setAmplitude(double amplitude);

private:
    std::shared_ptr<EISPotentiostaticElement> m_dataDerived;
};