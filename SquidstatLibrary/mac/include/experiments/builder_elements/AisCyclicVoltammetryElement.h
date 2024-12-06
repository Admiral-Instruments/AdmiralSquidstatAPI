#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class CyclicVoltammetryElement;

/**
 * @brief This experiment sweeps the potential of the working electrode back and forth between the 
 * <strong>first voltage-limit</strong> and the <strong>second voltage-limit</strong> at a constant
 * <strong>scan rate (dE/dt)</strong> for a specified number of <strong>cycles</strong>.
 * 
 * The scan will always start from the <strong>start voltage</strong> towards the <strong>first voltage-limit</strong>. 
 * The experiment will continue to cycle between the <strong>first voltage-limit</strong> and the <strong>second voltage-limit</strong>
 * according to the number of cycles.
 * The cycling scheme is as follow: 
 * <strong>start voltage</strong> &rarr; [<strong>first voltage-limit</strong> &rarr; <strong>first voltage-limit</strong>]n &rarr; <strong>Ending potential</strong>, 
 * where &ldquo;n&rdquo; is number of cycles.
 * <br>
*/
class SQUIDSTATLIBRARY_EXPORT AisCyclicVoltammetryElement final : public AisAbstractElement {
public:
    /**
     * @brief constructor of the cyclic voltammetry element.
     * @param startVoltage the value of the start voltage in volts
     * @param firstVoltageLimit the value of the first voltage-limit in volts
     * @param secondVoltageLimit the value of the second voltage-limit in volts
     * @param endVoltage the value of the end voltage in volts
     * @param dEdt the constant scan rate dE/dt in V/s.
     * @param samplingInterval the data sampling interval value in seconds.
    */
    explicit AisCyclicVoltammetryElement(
        double startVoltage,
        double firstVoltageLimit,
        double secondVoltageLimit,
        double endVoltage,
        double dEdt,
        double samplingInterval);

    /**
    * @brief copy constructor for the AisCyclicVoltammetryElement object.
   */
    explicit AisCyclicVoltammetryElement(const AisCyclicVoltammetryElement&);

    /**
    * @brief overload equal to operator for the AisCyclicVoltammetryElement object.
    */
    AisCyclicVoltammetryElement& operator=(const AisCyclicVoltammetryElement&);

    ~AisCyclicVoltammetryElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Cyclic Voltammetry".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Potentiostatic Control", "Basic Experiments").
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
     * @brief gets the potential sampling interval.
     * @return samplingInterval The quiet time sampling interval to set in seconds.
     */
    double getQuietTimeSamplingInterval() const;

    /**
    * @brief Sets the quiet time sampling interval.
    * @param quietTimeSamplingInterval The quiet time sampling interval to set in seconds.
    */
    void setQuietTimeSamplingInterval(double quietTimeSamplingInterval);

    /**
     * @brief get the value set for the start voltage
     * @return the value of the start voltage in volts
    */
    double getStartVoltage() const;

    /**
     * @brief set the value for the start voltage.
     * @param startVoltage the value of the start voltage in volts
    */
    void setStartVoltage(double startVoltage);

    /**
     * @brief tells whether the start voltage is set with respect to the open circuit voltage or not.
     * @return true if the start voltage is set with respect to the open-circuit voltage and false if not.
    */
    bool isStartVoltageVsOCP() const;

    /**
     * @brief set whether to reference the start voltage against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param startVoltageVsOCP  true to if the start voltage is set to reference the open-circuit voltage and false if set against the reference terminal.
    */
    void setStartVoltageVsOCP(bool startVoltageVsOCP);

    /**
     * @brief get the value set for the first voltage-limit.
     * 
     * After the starting voltage, the scan will go to the first voltage-limit. This could result in either upward scan first if 
     * the first voltage-limit is higher than the start voltage or downward scan first if the first voltage-limit is lower than the start voltage.
     * @return the first voltage-limit value in volts.
    */
    double getFirstVoltageLimit() const;

    /**
     * @brief set the first voltage-limit
     * 
     * After the starting voltage, the scan will go to the first voltage-limit. This could result in either upward scan first if 
     * the first voltage-limit is higher than the start voltage or downward scan first if the first voltage-limit is lower than the start voltage.
     * @param v1 first voltage-limit value in volts
    */
    void setFirstVoltageLimit(double v1);

    /**
     * @brief tells whether the first voltage-limit is set with respect to the open circuit voltage or not.
     * @return true if the first voltage-limit is set with respect to the open-circuit voltage and false if not.
     * @note if no value was set, the default is false.
    */
    bool isFirstVoltageLimitVsOCP() const;

    /**
     * @brief set whether to reference the first voltage-limit against the open-circuit voltage or not.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param firstVoltageLimitVsOCP true to set the upper voltage to be referenced against the open-circuit voltage and false otherwise.
    */
    void setFirstVoltageLimitVsOCP(bool firstVoltageLimitVsOCP);

    /**
     * @brief get the value set for the second voltage-limit
     * 
     * After starting from the start-voltage and reaching the first voltage-limit, the scan will go to the second voltage limit.
     * The scan will continue to oscillate between the first and second voltage-limits according to the number of cycles.
     * @return the second voltage-limit value in volts.
    */
    double getSecondVoltageLimit() const;

    /**
     * @brief set the second voltage-limit
     * 
     * After starting from the start-voltage and reaching the first voltage-limit, the scan will go to the second voltage limit.
     * The scan will continue to oscillate between the first and second voltage-limits according to the number of cycles.
     * @param v2 the second voltage-limit value in volts
    */
    void setSecondVoltageLimit(double v2);

    /**
     * @brief tells whether the second voltage-limit is set with respect to the open circuit voltage or not.
     * @return true if the second voltage-limit is set with respect to the open-circuit voltage and false if not.
     * @note if no value was set, the default is false.
    */
    bool isSecondVoltageLimitVsOCP() const;

    /**
     * @brief set whether to reference the second voltage-limit against the open-circuit voltage or not.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param secondVoltageLimitVsOCP true to set the second voltage-limit to be referenced against the open-circuit voltage and false otherwise.
    */
    void setSecondVoltageLimitVsOCP(bool secondVoltageLimitVsOCP);

    /**
     * @brief get the value set for the number of cycles
     * @return the number of cycles set.
    */
    unsigned int getNumberOfCycles();

    /**
     * @brief set the number of cycles to oscillate between the first voltage-limit and the second voltage-limit.
     * @param cycles the number of cycles to set
    */
    void setNumberOfCycles(unsigned int cycles);

    /**
     * @brief get the value set for the ending potential value.
     * 
     * This is the value of the voltage at which the experiment will stop.
     * After the last cycle, the experiment will do one last sweep towards this value.
     * @return the value set for the ending voltage in volts.
    */
    double getEndVoltage() const;

    /**
     * @brief set the ending potential value.
     * 
     * This is the value of the voltage at which the experiment will stop.
     * After the last cycle, the experiment will do one last sweep towards this value.
     * @param endVoltage the value to set for the ending potential in volts.
    */
    void setEndVoltage(double endVoltage);

    /**
     * @brief tells whether the end voltage is set with respect to the open circuit voltage or the reference terminal.
     * @return true if the end voltage is set with respect to the open-circuit voltage and false if set against the reference terminal.
     * @note if no value was set, the default is false
    */
    bool isEndVoltageVsOCP() const;

    /**
     * @brief set whether to reference the end voltage against the open-circuit voltage or the reference terminal.
     * 
     * The reference terminal is for you to connect to any reference point you like. Connect it to the working electrode to reference ground.
     * @param endVoltageVsOCP true to set the end voltage to be referenced against the open-circuit voltage and false if set against the reference terminal.
    */
    void setEndVoltageVsOCP(bool endVoltageVsOCP);

    /**
     * @brief get the value set for the constant scan rate dE/dt.
     * @return the value set for the constant scan rate dE/dt in V/s.
    */
    double getdEdt() const;

    /**
     * @brief set the value for the constant scan rate dE/dt.
     * @param dEdt the value set for the constant scan rate dE/dt in V/s.
    */
    void setdEdt(double dEdt);

    /**
     * @brief get how frequently we are sampling the data.
     * @return the data sampling interval value in seconds.
    */
    double getSamplingInterval() const;

    /**
     * @brief set how frequently we are sampling the data.
     * @param sampInterval the data sampling interval value in seconds.
    */
    void setSamplingInterval(double sampInterval);

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
     * This is an <strong>optional</strong> parameter. 
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
    * This is an <strong>optional</strong> parameter.
    * If nothing is set, the device will use the default value of 75.
    * @param alphaFactor the value for the alphaFactor ranges from 0 to 100.
   */
    void setAlphaFactor(double alphaFactor);

private:
    std::shared_ptr<CyclicVoltammetryElement> m_dataDerived;
};