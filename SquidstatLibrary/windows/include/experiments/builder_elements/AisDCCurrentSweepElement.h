#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>



class DCCurrentSweepElement;

/**
 * @brief this experiment performs a DC current sweep from the <strong>starting current</strong> to the <strong>ending current</strong>
 * which progresses linearly according to the <strong>scan rate</strong>.
 * 
 * <br>
 * @image html DCCurrentSweep.png
 * @image latex DCCurrentSweep.png
*/
class SQUIDSTATLIBRARY_EXPORT AisDCCurrentSweepElement : public AisAbstractElement {
public:

    /**
     * @brief the DC current sweep element.
     * @param startCurrent the value for the starting current in Amps.
     * @param endCurrent the value for the ending current in Amps.
     * @param scanRate the value for the current scan rate in A/s.
     * @param samplingInterval how frequently we are sampling the data.
    */
    explicit AisDCCurrentSweepElement(
        double startCurrent,
        double endCurrent,
        double scanRate,
        double samplingInterval
    
    );
    explicit AisDCCurrentSweepElement(const AisDCCurrentSweepElement&);
    AisDCCurrentSweepElement& operator= (const AisDCCurrentSweepElement&);

    ~AisDCCurrentSweepElement() override;


    /**
     * @brief get the name of the element.
     * @return The name of the element: "DC Current Linear Sweep".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Galvanostatic Control", "Basic Voltammetry").
    */

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Galvanostatic Control", "Basic Voltammetry").
    */
    QStringList getCategory() const override;

    /**
     * @brief get the value set for the starting current.
     * @return the value set for the constant current in Amps.
    */
    double getStartingCurrent() const;

    /**
     * @brief set the value for the starting current.
     * @param startingCurrent the value to set for the starting current in Amps
    */
    void setStartingCurrent(double startingCurrent);

    /**
     * @brief get the value set for the ending current.
     * @return the value for the ending current in Amps.
    */
    double getEndingCurrent() const;

    /**
     * @brief set the value for the ending current.
     * @param endingCurrent the value for the ending current in Amps
    */
    void setEndingCurrent(double endingCurrent);

    /**
     * @brief get the value set for the scan rate.
     * @return the value set for the scan rate in A/s.
     * @see setScanRate
    */
    double getScanRate() const;

    /**
     * @brief set the value for the current scan rate.
     * 
     * The scan rate represents the value of the discrete current step size in one second in the linear sweep.
     * @param scanRate the value to set for the scan rate.
    */
    void setScanRate(double scanRate);

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
     * @brief get the value set for the maximum voltage.
     * The experiment will end when it reaches this value.
     * @return the value set for the maximum voltage.
     * @note this is an optional parameter. If no value has been set, the default value is positive infinity
    */
    double getMaxVoltage() const;

    /**
     * @brief set a maximum voltage to stop the experiment.
     * 
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an upper-limit voltage value.
     * If a maximum voltage is set, the experiment will continue to run as long as the measured voltage is below that value.
     * @param maxVoltage the maximum voltage value in volts at which the experiment will stop.
    */
    void setMaxVoltage(double maxVoltage);

    /**
     * @brief get the value set minimum for the voltage in volts.
     * @return the value set for the minimum voltage in volts.
     * @note this is an optional parameter. If no value has been set, the default value is negative infinity
    */
    double getMinVoltage() const;

    /**
     * @brief set a minimum voltage to stop the experiment.
     * 
     * The is an <strong>optional</strong> condition. 
     * If nothing is set, then the experiment will not stop based on an lower-limit voltage value.
     * If a maximum voltage is set, the experiment will continue to run as long as the measured voltage is above that value.
     * @param minVoltage the minimum voltage value in volts at which the experiment will stop.
    */
    void setMinVoltage(double minVoltage);

private:
    std::shared_ptr<DCCurrentSweepElement> m_dataDerived;
};