#ifndef SQUIDSTATLIBRARY_AISDATAPOINTS_H
#define SQUIDSTATLIBRARY_AISDATAPOINTS_H

#include <qstring.h>

/**
 * @brief a structure containing DC data information.
*/
struct AisDCData {

    /**
     * @brief the time at which the DC data arrived.
    */
    double timestamp;

    /**
     * @brief the measured working electrode voltage in volts.
    */
    double workingElectrodeVoltage;

    /**
     * @brief the measured counter electrode voltage in volts.
    */
    double counterElectrodeVoltage;

    /**
     * @brief the measured electric current value in Amps
    */
    double current;

    /**
     * @brief the measured temperature in Celsius.
    */
    double temperature;
};

/**
 * @brief a structure containing AC data information.
*/
struct AisACData {

    /**
     * @brief the time at which the AC data arrived.
    */
    double timestamp;

    /**
     * @brief the applied frequency in Hz.
    */
    double frequency;

    /**
     * @brief the magnitude of the complex impedance.
    */
    double absoluteImpedance;

    /**
     * @brief the real part of the complex impedance.
    */
    double realImpedance;

    /**
     * @brief the imaginary part of the complex impedance.
    */
    double imagImpedance;

    /**
     * @brief the phase angle between the real and the imaginary parts of the impedance. 
    */
    double phaseAngle;

    /**
     * @brief the percentage of the total harmonic distortion in the AC signal.
    */
    double totalHarmonicDistortion;

    /**
     * @brief the number of cycles specific to the reported frequency.
     * 
     * In EIS, we run a range of frequencies. For each frequency, a specific number of cycles are run. 
     * The higher the frequency, the more number of cycles.
    */
    double numberOfCycles;

    /**
     * @brief the DC working electrode voltage in volts.
    */
    double workingElectrodeDCVoltage;

    /**
     * @brief the DC electric current value in Amps
    */
    double DCCurrent;

    /**
     * @brief the amplitude of the AC current.
    */
    double currentAmplitude;

    /**
     * @brief the amplitude of the AC voltage.
    */
    double voltageAmplitude;
};

/**
 * @brief a structure containing some information regarding the running element.
*/
struct AisExperimentNode {

    /**
     * @brief This is the name of the current element running.
    */
    QString stepName;

    /**
     * @brief this number is the order of the element within the custom experiment.
    */
    int stepNumber;

    /**
     * @brief this number is the order of the step within the element.
    */
    int substepNumber;
};

#endif //SQUIDSTATLIBRARY_AISDATAPOINTS_H
