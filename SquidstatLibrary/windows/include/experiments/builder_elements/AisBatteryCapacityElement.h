#pragma once

#include "AisAbstractElement.h"
#include "AisSquidstatGlobal.h"
#include <QString>
#include <system_error>


class BatteryCapacityElement;

/**
 * @brief This test determines the capacity of a battery. A battery is charged then discharged according to constant current-constant voltage (CC-CV) charging/discharging method. 
 * The constant current stage ends after reaching the <b>Voltage limit</b> and the constant voltage stage ends after reaching the <b>Current limit</b>.
 * 
 * <br>
 * @image html BatteryCapacity.png
 * @image latex BatteryCapacity.png
 * 
 * During the charging phase, we have two stages, constant current (CC) and constant voltage (CV). 
 * During the CC stage, the current flows into the battery until the battery reaches the charge <b>Voltage limit</b> where, the CV stage starts.
 * The current then continues to flow into the battery but at a decreasing rate, keeping the battery voltage at the same level. 
 * The CV stage stops and thus charging stops when the current decreases to the minimum charge current limit.
 * 
 * Similarly during the discharging phase, we have CC and CV.
 * During the CC stage, the current flows out of the battery until the battery voltage falls down to the discharge <b>Voltage limit</b> where, the CV stage starts.
 * The current then continues to flow out of the battery but at a decreasing rate, keeping the battery voltage at the same level. 
 * The CV stage stops and thus the discharging stops when the absolute current decreases to the minimum discharge current limit.
 * 
*/
class SQUIDSTATLIBRARY_EXPORT AisBatteryCapacityElement final : public AisAbstractElement {
public:

    /**
     * @brief this is the battery capacity element constructor. 
     * @param chargeCurrent the value of the charge-phase constant current in Amps
     * @param dischargeCurrent the value of the discharge-phase constant current in Amps.
     * @param chargeVoltageLimit the value representing the charge-phase voltage limit in volts
     * @param dischargeVoltageLimit the value representing the discharge-phase voltage limit in volts.
     * @param minChargeCurrent the value representing the charge-phase minimum current limit in Amp.
     * @param minDischargeCurrent the value representing the discharge-phase minimum current limit in Amp.
     * @param chargeSampInterval the data sampling interval value in seconds during the charge phase.
     * @param dischargeSampInterval the data sampling interval value in seconds during the discharge phase.
    */
    explicit AisBatteryCapacityElement(
        double chargeCurrent,
        double dischargeCurrent,
        double chargeVoltageLimit,
        double dischargeVoltageLimit,
        double minChargeCurrent,
        double minDischargeCurrent,
        double chargeSampInterval,
        double dischargeSampInterval
    );

    explicit AisBatteryCapacityElement(const AisBatteryCapacityElement&);
    AisBatteryCapacityElement& operator= (const AisBatteryCapacityElement&);

    
    ~AisBatteryCapacityElement() override;

    /**
     * @brief get the name of the element.
     * @return The name of the element: "Battery Capacity".
    */
    QString getName() const override;

    /**
     * @brief get a list of applicable categories of the element.
     * @return A list of applicable categories: ("Energy Storage", "Charge/Discharge").
    */
    QStringList getCategory() const override;

    /**
     * @brief tells whether or not the constant voltage steps/stages are set to be skipped.
     * @return true if the constant voltage steps are set to be skipped and false otherwise.
     * @note if nothing is set, the default is false i.e. the CV step is included.
    */
    bool isSkipCVsteps() const;

    /**
     * @brief set whether to skip the constant voltage steps/stages or not.
     * @param skipCVSteps the constant voltage steps will be skipped if the given argument is true and not skipped if given false.
     * @note by this is an optional parameter and by default is set to false i.e. the CV step is included.
    */
    void setSkipCVsteps(bool skipCVSteps);

    /**
     * @brief get the current value set for the CC stage of the charging phase.
     * 
     * In the charge phase, we have two stages: constant current with voltage going up followed by constant voltage with current going down.
     * Here, we are getting the value of the current at the constant current stage.
     * @return the value of the charge-phase constant current in Amps.
     * @see AisBatteryCapacityElement
    */
    double getChargeCurrent() const;

    /**
     * @brief set current value for the CC stage of the charging phase. 
     * 
     * In the charge phase, we have two stages: constant current with voltage going up followed by constant voltage with current going down.
     * Here, we are setting the value of the current at the constant current stage.
     * @param chargeCurrent the value of the charge-phase constant current in Amps.
    */
    void setChargeCurrent(double chargeCurrent);

    /**
     * @brief get the voltage limit that the battery is set to charge up to.
     * This is the value at which CC stops and CV starts during the charging phase.
     * @return the value representing the charge-phase voltage limit
     * @see AisBatteryCapacityElement
    */
    double getChargeVoltageLimit() const;

    /**
     * @brief set the voltage limit to charge the battery up to.
     * This is the value at which CC stops and CV starts during the charging phase.
     * @param chargeVoltageLimit the value representing the charge-phase voltage limit in volts
     * @see AisBatteryCapacityElement
    */
    void setChargeVoltageLimit(double chargeVoltageLimit);

    /**
     * @brief get the current limit value for the CV stage of the charging phase.
     *
     * In the charge phase, we have two stages: constant current with voltage going up followed by constant voltage with current going down.
     * Here, we are getting the minimum current value at which the CV step stops and therefore charge phase stops too.
     * @return the value representing the charge-phase current limit in Amp.
    */
    double getMinChargeCurrent() const;

    /**
     * @brief set the current limit value for the CV stage of the charging phase.
     * 
     * In the charge phase, we have two stages: constant current with voltage going up followed by constant voltage with current going down.
     * Here, we are setting the minimum value for the current to reach in the CV stage of the charging phase. 
     * CV stops and therefore charging stops when this current limit is reached.
     * @param minChargeCurrent the value representing the charge-phase minimum current limit in Amp.
    */
    void setMinChargeCurrent(double minChargeCurrent);

    /**
     * @brief get how frequently we are sampling the data during the charge phase.
     * @return the data sampling interval value in seconds during the charge phase.
    */
    double getChargeSamplingInterval() const;

    /**
     * @brief set how frequently we are sampling the data during the charge phase.
     * @param chargeSampInterval the data sampling interval value in seconds during the charge phase.
    */
    void setChargeSamplingInterval(double chargeSampInterval);

    /**
     * @brief get current value set for the CC stage of the discharging phase.
     * 
     * In the discharge phase, we have two stages: constant current with voltage going down followed by constant voltage with absolute current going down.
     * Here, we are getting the constant current value.
     * @return the value of the discharge-phase constant current in Amps.
    */
    double getDischargeCurrent() const;

    /**
     * @brief set current value for the CC stage of the discharging phase.
     * 
     * In the discharge phase, we have two stages: constant current with voltage going down followed by constant voltage with absolute current going down.
     * Here, we are setting the constant current value.
     * @param dischargeCurrent the value of the discharge-phase constant current in Amps.
    */
    void setDischargeCurrent(double dischargeCurrent);

    /**
     * @brief get the voltage limit that the battery is set to discharge down to.
     * This is the value at which CC stops and CV starts during the discharging phase.
     * @return the value representing the discharge-phase voltage limit in volts.
    */
    double getDischargeVoltageLimit() const;

    /**
     * @brief set the voltage limit to discharge the battery down to. 
     * This is the value at which CC stops and CV starts during the discharging phase.
     * @param dischargeVoltageLimit the value representing the discharge-phase voltage limit in volts.
    */
    void setDischargeVoltageLimit(double dischargeVoltageLimit);

    /**
     * @brief get the current limit value for the CV stage of the discharging phase.
     * 
     * In the discharge phase, we have two stages: constant current with voltage going down followed by constant voltage with absolute current going down.
     * Here, we are getting the minimum current value at which CV stops and therefore discharging stops too.
     * @return the value representing the discharge-phase current limit in Amp.
    */
    double getMinDischargeCurrent() const;

    /**
     * @brief set the current limit value for the CV stage of the discharging phase.
     * 
     * In the discharge phase, we have two stages: constant current with voltage going down followed by constant voltage with absolute current going down.
     * Here, we are setting the minimum current value at which the CV stops and therefore discharging stops too.
     * @param minDischargeCurrent the value representing the discharge-phase minimum current limit in Amp.
    */
    void setMinDischargeCurrent(double minDischargeCurrent);

    /**
     * @brief get how frequently we are sampling the data during the discharge phase.
     * @return the data sampling interval value in seconds during the discharge phase.
    */
    double getDischargeSamplingInterval() const;

    /**
     * @brief set how frequently we are sampling the data during the discharge phase.
     * @param dischargeSampInterval the data sampling interval value in seconds during the discharge phase.
    */
    void setDischargeSamplingInterval(double dischargeSampInterval);

    /**
     * @brief get the currently set rest time between the charging and the discharging phases. 
     * 
     * At rest time, no current flows in or out of the battery.
     * @return rest time in seconds.
     * @note by this is an optional parameter and by default is set to 0s
    */
    double getRestPeriodDuration() const;

    /**
     * @brief set a rest time between the charging and the discharging phases. 
     *
     * At rest time, no current flows in or out of the battery.
     * @param restPeriodDuration rest time in seconds.
     * @note by this is an optional parameter and by default is set to 0s
    */
    void setRestPeriodDuration(double restPeriodDuration);

    /**
     * @brief get how frequently, we are sampling (voltage) in rest time.
     * 
     * At rest time, no current flows in or out of the battery.
     * @return the sampling interval value in seconds.
     * @note by this is an optional parameter and by default is set to 1s
    */
    double getRestSamplingInterval() const;

    /**
     * @brief set how frequently, we are sampling (voltage) in rest time.
     * 
     * At rest time, no current flows in or out of the battery.
     * @param restSampInterval the sampling interval value in seconds.
     * @note by this is an optional parameter and by default is set to 1s
    */
    void setRestSamplingInterval(double restSampInterval);
    
private:
    std::shared_ptr<BatteryCapacityElement> m_dataDerived;
};