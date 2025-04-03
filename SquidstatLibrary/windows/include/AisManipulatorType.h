#ifndef AISMANIPULATORTYPE_H
#define AISMANIPULATORTYPE_H

/** 
 * @ingroup Helpers
 * 
 * @brief This enum represents different pulse types, it is intended to tell the AisDataManipulator class how to calculate various pulse experiment parameters.
*/ 
enum AisPulseType {
    DifferentialPulse = 0,  //! Corresponds to AisDiffPulseVoltammetryElement
    NormalPulse  = 1 ,      //! Corresponds to AisNormPulseVoltammetryElement
    SquarewavePulse = 2     //! Corresponds to AisSquareWaveVoltammetryElement
};

#endif // AISMANIPULATORTYPE_H
