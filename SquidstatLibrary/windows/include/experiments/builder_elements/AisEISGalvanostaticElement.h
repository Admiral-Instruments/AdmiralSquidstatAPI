#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>

class EISGalvanostaticElement;

/**
 * @brief This experiment records the complex impedance of the experimental cell in galvanostatic mode,
 * starting from the <strong>start frequency</strong> and sweeping through towards the <strong>end frequency</strong>,
 * with a fixed number of frequency <strong>steps per decade</strong>. 
 * 
 * Important parameters include the <strong>DC bias</strong> and the <strong>AC excitation amplitude</strong>.
 * 
 * <br>
 * @image html EISGalvanostatic.png
 * @image latex EISGalvanostatic.png
*/
class SQUIDSTATLIBRARY_EXPORT AisEISGalvanostaticElement final : public AisAbstractElement {
public:
    /**
     * @brief the EIS galvanostatic element constructor.
     * @param startFrequency the value for the current starting frequency
     * @param endFrequency the value for the current ending frequency
     * @param stepsPerDecade the value for the current frequency steps per decade.
     * @param currentBias the value for the DC bias (DC offset).
     * @param currentAamplitude the AC current amplitude.
    */
    explicit AisEISGalvanostaticElement(
        double startFrequency,
        double endFrequency,
        double stepsPerDecade,
        double currentBias,
        double currentAamplitude);
    /**
  * @brief copy constructor for the AisEISGalvanostaticElement object.
 */
    explicit AisEISGalvanostaticElement(const AisEISGalvanostaticElement&);
    /**
* @brief overload equal to operator for the AisEISGalvanostaticElement object.
*/
    AisEISGalvanostaticElement& operator=(const AisEISGalvanostaticElement&);

    ~AisEISGalvanostaticElement() override;

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
     * @brief get the name of the element.
     * @return The name of the element: "Galvanostatic EIS".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Galvanostatic Control", "Impedance Methods", "Basic Experiments").
    */
    QStringList getCategory() const override;


    /**
     * @brief get the value set for the current starting frequency
     * @return the value set for the current start frequency in Hz?
    */
    double getStartFreq() const;

    /**
     * @brief set the value for the current starting frequency.
     * @param startFreq the value to set the current starting frequency in Hz
    */
    void setStartFreq(double startFreq);

    /**
     * @brief the value set for the current ending frequency.
     * @return the value set for the current end frequency in Hz
    */
    double getEndFreq() const;

    /**
     * @brief set the value for the current end frequency.
     * @param endFreq the value to set for the current end frequency in Hz
    */
    void setEndFreq(double endFreq);

    /**
     * @brief get the value set for the current frequency steps per decade.
     * @return the value set for the current frequency steps per decade. This is unit-less.
    */
    double getStepsPerDecade() const;

    /**
     * @brief set the number of the current frequency steps per decade.
     * @param stepsPerDecade the value to set for the number of steps per decade.
    */
    void setStepsPerDecade(double stepsPerDecade);

    /**
     * @brief get the value set for the DC bias (DC offset).
     * @return the value set for the DC bias in Amps.
    */
    double getBiasCurrent() const;

    /**
     * @brief set the value for the DC bias (DC offset).
     * @param biasCurrent the value to set for the DC bias in Amps.
    */
    void setBiasCurrent(double biasCurrent);

    /**
     * @brief the value to set for the AC current amplitude.
     * @return the value set for the AC current amplitude in Amps.
    */
    double getAmplitude() const;

    /**
     * @brief set the value for the AC current amplitude.
     * @param amplitude the value to set for the AC current amplitude in Amps.
    */
    void setAmplitude(double amplitude);

    /**
     * @brief set the minimum number of period of applied sinusoidal current at each frequency.
     * @param numberOfCycle set number of cycles Sampled.
    */
    void setMinimumCycles(int numberOfCycle);

    /**
     * @brief the value to setted for mimimum number of cycle Sampled.
     * @return the value set for the minimum number of cycles.
    */
    int getMinimumCycles() const;

private:
    std::shared_ptr<EISGalvanostaticElement> m_dataDerived;
};
