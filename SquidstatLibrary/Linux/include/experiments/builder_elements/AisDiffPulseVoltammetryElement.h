#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class DiffPulseVoltammetryElement;

/**
 * @brief In this experiment, the working electrode holds at a <strong>starting potential</strong> during the <strong>quiet time</strong>. 
 * Then it applies a train of pulses superimposed on a staircase waveform, with a uniform <strong>potential step</strong> size. 
 * The potential continues to step until the <strong>final potential</strong> is reached. 
 * 
 * The <strong>pulse width</strong> is the amount of time between the rising and falling edge of a pulse. 
 * The <strong>pulse period</strong> is the amount of time between the beginning of one pulse and the beginning of the next.
 * 
 * <br>
 * @image html DiffPulseVoltammetry.png
 * @image latex DiffPulseVoltammetry.png
 * 
 * Advanced control of data output for pulse experiments can be performed using the class @see AisDataManipulator
*/
class SQUIDSTATLIBRARY_EXPORT AisDiffPulseVoltammetryElement final : public AisAbstractElement {
public:
    /**
     * @brief the differential pulse element constructor.
     * @param startVoltage the value of the starting potential in volts
     * @param endVoltage the value of the ending potential in volts
     * @param voltageStep the value set for the voltage step in volts.
     * @param pulseHeight the value for the pulse height in volts.
     * @param pulseWidth the value for the pulse width in seconds.
     * @param pulsePeriod the value for the pulse period in seconds.
    */
    explicit AisDiffPulseVoltammetryElement(
        double startVoltage,
        double endVoltage,
        double voltageStep,
        double pulseHeight,
        double pulseWidth,
        double pulsePeriod);
    /**
   * @brief copy constructor for the AisDiffPulseVoltammetryElement object.
  */
    explicit AisDiffPulseVoltammetryElement(const AisDiffPulseVoltammetryElement&);
    /**
  * @brief overload equal to operator for the AisDiffPulseVoltammetryElement object.
  */
    AisDiffPulseVoltammetryElement& operator=(const AisDiffPulseVoltammetryElement&);

    ~AisDiffPulseVoltammetryElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Differential Pulse Potential Voltammetry".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Basic Voltammetry", "Pulse Voltammetry").
    */

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
     * @brief tells whether the starting potential is set against the open-circuit voltage or the reference terminal.
     * @return true if the starting potential is set against the open-circuit voltage and false if it is set against the reference terminal.
     * @see setStartVoltageVsOCP
    */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief set whether to reference the starting potential against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param startVoltageVsOCP  true to if the starting potential is set to reference the open-circuit voltage and false if set against the reference terminal.
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
     * @param endVoltage the value to set for the ending voltage in volts.
    */
    void setEndVoltage(double endVoltage);

    /**
     * @brief tells whether the end voltage is set with respect to the open circuit voltage or the reference terminal.
     * @return true if the end voltage is set with respect to the open-circuit voltage and false if set against the reference terminal.
     * @see setEndVoltageVsOCP
    */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief set whether to reference the end voltage against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param endVoltageVsOCP true to set the end voltage to be referenced against the open-circuit voltage and false if set against the reference terminal.
     * @note by default, this is set to false.
    */
    void setEndVoltageVsOCP(bool endVoltageVsOCP);

    /**
     * @brief get the value set for the potential step.
     * @return the value set for the potential step in volts.
     * @see setVStep
    */
    double getVStep() const;

    /**
     * @brief set the value for the potential step.
     * 
     * The potential step is the difference between the starting potential of two consecutive pulses.
     * @param vStep the value to set for the potential step in volts.
    */
    void setVStep(double vStep);

    /**
     * @brief get the value set for the pulse height.
     * @return the value set for the pulse height in volts.
     * @see setPulseHeight
    */
    double getPulseHeight() const;

    /**
     * @brief set the value for the pulse height.
     * 
     * For the first pulse, the pulse height is added to the starting potential. 
     * For the next pulse, the pulse height is added to the potential voltage and the potential step.
     * In general, the pulse height is added to the potential step and the starting voltage of the last pulse.
     * @param pulseHeight the value to set for the pulse height in volts.
    */
    void setPulseHeight(double pulseHeight);

    /**
     * @brief get the value set for the pulse width.
     * @return the value set for the pulse width in seconds.
     * @see setPulseWidth
    */
    double getPulseWidth() const;

    /**
     * @brief set the value for the pulse width.
     * 
     * The pulse width is the value in seconds for the time spent at the same voltage set for the pulse height.
     * @param pulseWidth the value to set for the pulse width in seconds.
     * @see setPulseHeight
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

    /**
    * @brief alphafactor controls the percentage of data sampled during a given interval. Data will be averaged over the last n% of the sampling interval.
    *
    * The is an <strong>optional</strong> parameter.
    * If nothing is set, the device will use the default value of 75.
    * @param alphafactor the value for the alphafactor ranges from 0 to 100.
   */
    void setAlphaFactor(double alphafactor);

private:
    std::shared_ptr<DiffPulseVoltammetryElement> m_dataDerived;
};