#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class SquareWaveVoltammetryElement;

/**
 * @brief This experiment holds the working electrode at the <strong>starting potential</strong> during the <strong>quiet time</strong>. 
 * Then it applies a train of square pulses superimposed on a staircase waveform with a uniform <strong>potential step</strong> magnitude. 
 * 
 * The potential continues to step until the <strong>final potential</strong> is reached. 
 * Each square pulse consists of a forward pulse and a reverse pulse of equal in <strong>amplitude</strong> but opposite in direction. 
 * <strong>Frequency</strong> is the inverse of the total duration of a square pulse. 
 * Current responses are sampled at two points, one at the end of the forward pulse (if) and another at the end of the reverse pulse (ir). 
 * The difference in current sampled at these two points is plotted against the potential of the corresponding staircase tread.
 * 
 * <br>
 * @image html SqrWaveVoltammetry.png
 * @image latex SqrWaveVoltammetry.png
 * 
 * Advanced control of data output for pulse experiments can be performed using the class @see AisDataManipulator
*/
class SQUIDSTATLIBRARY_EXPORT AisSquareWaveVoltammetryElement final : public AisAbstractElement {
public:
    /**
     * @brief the square wave element constructor
     * @param startVoltage the value of the starting potential in volts
     * @param endVoltage the value of the ending potential in volts
     * @param voltageStep the value set for the voltage step in volts.
     * @param pulseAmp the value for the pulse amplitude in volts.
     * @param pulseFrequency the value for the pulse frequency in Hz.
    */
    explicit AisSquareWaveVoltammetryElement(
        double startVoltage,
        double endVoltage,
        double voltageStep,
        double pulseAmp,
        double pulseFrequency);

    /**
* @brief copy constructor for the AisSquareWaveVoltammetryElement object.
*/
    explicit AisSquareWaveVoltammetryElement(const AisSquareWaveVoltammetryElement&);
    /**
* @brief overload equal to operator for the AisSquareWaveVoltammetryElement object.
*/
    AisSquareWaveVoltammetryElement& operator=(const AisSquareWaveVoltammetryElement&);

    ~AisSquareWaveVoltammetryElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Square Wave Potential Voltammetry".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Pulse Voltammetry").
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
     * @param startVoltageVsOcp  true to if the start voltage is set to reference the open-circuit voltage and false if set against the reference terminal.
     * @note by default, this is set to false.
    */
    void setStartVoltageVsOCP(bool startVoltageVsOcp);

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
     * The voltage step is added to the value of the starting potential of the previous pulse to start the new pulse.
     * @param vStep the value for the voltage step in volts.
     * @note Regardless of vStep's sign, the device will determine the step direction based on the start and end voltage.
    */
    void setVStep(double vStep);

    /**
     * @brief get the value set for the pulse amplitude.
     * @return the value set for the pulse amplitude in volts.
     * @see setPulseAmp
    */
    double getPulseAmp() const;

    /**
     * @brief set the value for the pulse amplitude.
     * 
     * The voltage pulse goes up in hight by the given amplitude in addition to the starting potential (of the previous pulse). 
     * It then goes back down twice the amplitude to end up one amplitude below the starting potential (of the previous pulse).
     * @param pulseAmp the value to set for the pulse amplitude in volts.
    */
    void setPulseAmp(double pulseAmp);

    /**
     * @brief get the value set for the pulse frequency.
     * @return the value set for the frequency in Hz.
    */
    double getPulseFreq() const;

    /**
     * @brief set the value for the pulse frequency.
     * @param pulseFreq the value to set for the pulse frequency in Hz.
    */
    void setPulseFreq(double pulseFreq);

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
     * @brief Get the value set for the alpha factor
     *
     * @return The value for the alpha factor is represented as a percent between 0 and 100.
     * @note If nothing is set, this function will return a default value of 75.
    */
    double getAlphaFactor() const;

    /**
    * @brief alpha factor controls the percentage of data sampled during a given interval. Data will be averaged over the last n% of the sampling interval.
    *
    * The is an <strong>optional</strong> parameter.
    * If nothing is set, the device will use the default value of 75.
    * @param alphaFactor the value for the alphaFactor ranges from 0 to 100.
   */
    void setAlphaFactor(double alphaFactor);

private:
    std::shared_ptr<SquareWaveVoltammetryElement> m_dataDerived;
};