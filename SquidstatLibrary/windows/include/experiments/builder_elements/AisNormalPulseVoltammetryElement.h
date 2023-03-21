#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class NormalPulseVoltammetryElement;

/**
 * @brief This experiment holds the working electrode at a <strong>baseline potential</strong> during the <strong>quiet time</strong>,
 * then applies a train of pulses, which increase in amplitude until the <strong>final potential</strong> is reached. 
 * 
 * The <strong>potential step</strong> is the magnitude of this incremental increase. 
 * The <strong>pulse width</strong> is the amount of time between the rising and falling edge of a pulse. 
 * The <strong>pulse period</strong> is the amount of time between the beginning of one pulse and the beginning of the next.
 * 
 * <br>
 * @image html NormalPulseVoltammetry.png
 * @image latex NormalPulseVoltammetry.png
*/
class SQUIDSTATLIBRARY_EXPORT AisNormalPulseVoltammetryElement final : public AisAbstractElement {
public:
    /**
     * @brief the normal-pulse-voltammetry element constructor
     * @param startVoltage the value of the starting potential in volts
     * @param endVoltage the value of the ending potential in volts
     * @param voltageStep the value set for the voltage step in volts.
     * @param pulseWidth the value for the pulse width in volts.
     * @param pulsePeriod the value for the pulse period in volts.
    */
    explicit AisNormalPulseVoltammetryElement(
        double startVoltage,
        double endVoltage,
        double voltageStep,
        double pulseWidth,
        double pulsePeriod);
    /**
* @brief copy constructor for the AisNormalPulseVoltammetryElement object.
*/
    explicit AisNormalPulseVoltammetryElement(const AisNormalPulseVoltammetryElement&);
    /**
* @brief overload equal to operator for the AisNormalPulseVoltammetryElement object.
*/
    AisNormalPulseVoltammetryElement& operator=(const AisNormalPulseVoltammetryElement&);

    ~AisNormalPulseVoltammetryElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Normal Pulse Potential Voltammetry".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Basic Voltammetry", "Pulse Voltammetry").
    */
    QStringList getCategory() const override;

    /**
     * @brief get the value set for the start voltage.
     * @return the value of the start voltage in volts.
    */
    double getStartVoltage() const;

    /**
     * @brief set the value for the start voltage.
     * @param startVoltage the value of the start voltage in volts
    */
    void setStartVoltage(double startVoltage);

    /**
     * @brief tells whether the start voltage is set against the open-circuit voltage or the reference terminal.
     * @return true if the start voltage is set against the open-circuit voltage and false if it is set against the reference terminal.
     * @note if nothing is set, the default is false.
     * @see setStartVoltageVsOCP
    */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief set whether to reference the start voltage against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param startVoltageVsOCP  true to if the start voltage is set to reference the open-circuit voltage and false if set against the reference terminal.
     * @note by default, this is set to false.
    */
    void setStartVoltageVsOCP(bool startVoltageVsOCP);

    /**
     * @brief get the value set for the ending potential value.
     * 
     * This is the value of the voltage at which the experiment will stop.
     * @return the value set for the ending voltage in volts.
    */
    double getEndVoltage() const;

    /**
     * @brief set the ending potential value.
     * 
     * This is the value of the voltage at which the experiment will stop.
     * @param endVoltage the value to set for the ending potential in volts.
    */
    void setEndVoltage(double endVoltage);

    /**
     * @brief tells whether the end voltage is set with respect to the open circuit voltage or the reference terminal.
     * @return true if the end voltage is set with respect to the open-circuit voltage and false if set against the reference terminal.
     * @note if nothing is set, the default is false.
    */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief set whether to reference the end voltage against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param endVoltageVsOcp true to set the end voltage to be referenced against the open-circuit voltage and false if set against the reference terminal.
     * @note by default, this is set to false.
    */
    void setEndVoltageVsOCP(bool endVoltageVsOcp);

    /**
     * @brief get the value set for the voltage step.
     * @return the value set for the voltage step in volts.
     * @see setVStep
    */
    double getVStep() const;

    /**
     * @brief set the value for the voltage step.
     * 
     * The voltage step is the voltage difference between the heights of two consecutive pulses.
     * @param vStep the value for the voltage step in volts.
    */
    void setVStep(double vStep);

    /**
     * @brief get the value for the voltage pulse width.
     * @return the value for the voltage pulse width in seconds.
     * @see setPulseWidth
     * 
    */
    double getPulseWidth() const;

    /**
     * @brief set the value for the voltage pulse width.
     * 
     * The pulse width is the value in seconds for the time spent at the same voltage set for the pulse height.
     * @param pulseWidth the value to set for the pulse width in seconds.
    */
    void setPulseWidth(double pulseWidth);

    /**
     * @brief get the value set for the pulse period.
     * @return the value set for the pulse period in seconds.
     * @see setPulsePeriod
    */
    double getPulsePeriod() const;

    /**
     * @brief set the value for the pulse period.
     * 
     * The pulse period is the time spent between the starts of two consecutive pulses.
     * @param pulsePeriod the value to set for the pulse period in seconds.
    */
    void setPulsePeriod(double pulsePeriod);

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
     * The is an <strong>optional</strong> parameter. 
     * If nothing is set, the device will auto-select the current range.
     * @param approxMaxCurrent the value for the maximum current expected in Amps.
    */
    void setApproxMaxCurrent(double approxMaxCurrent);

private:
    std::shared_ptr<NormalPulseVoltammetryElement> m_dataDerived;
};