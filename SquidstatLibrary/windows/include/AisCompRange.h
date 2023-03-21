#ifndef SQUIDSTATLIBRARY_AISCOMPRANGE_H
#define SQUIDSTATLIBRARY_AISCOMPRANGE_H

#include "AisSquidstatGlobal.h"
#include <QString>
#include <memory>

class AisCompRangePrivate;

/**
 * @brief This class has advanced options controlling the device stability including the bandwidth index and the stability factor.
 * @see setBandwidthIndex
 * @see setStabilityFactor
*/
class SQUIDSTATLIBRARY_EXPORT AisCompRange final {
public:
    /**
     * @brief constructor for the compensation-range object.
     * @param compRangeName a name to set for the compensation range for reference purposes.
     * @param bandwidthIndex the index value for the bandwidth.
     * @param stabilityFactor the factor value for the stability.
     * @see setBandwidthIndex
     * @see setStabilityFactor
    */
    explicit AisCompRange(const QString& compRangeName, uint8_t bandwidthIndex, uint8_t stabilityFactor);

    /**
     * @brief copy constructor for the compensation-range object.
    */
    AisCompRange(const AisCompRange&);

    
    /**
     * @brief get the value set for the bandwidth index.
     * @return the set value for the bandwidth index.
     * @see setBandwidthIndex
    */
    uint8_t getBandwidthIndex() const;

    /**
     * @brief set the index value for the bandwidth.
     * 
     * Usually, the device's default index value is optimal for running experiments. 
     * You may still increase the index within the range 0-10 as you run higher frequency experiments to see what best fits.
     * @param index the index value for the bandwidth (0-10).
    */
    void setBandwidthIndex(uint8_t index);

    /**
     * @brief get the value set for the stability factor.
     * @return the value set for the stability factor.
    */
    uint8_t getStabilityFactor() const;

    /**
     * @brief set a value for the stability factor.
     * 
     * Usually, the device's default factor value is optimal for running experiments. 
     * You may still increase the factor within the range 0-10 as you run experiments with more oscillations to see what best fits.
     * @param factor the stability-factor value (0-10) 
    */
    void setStabilityFactor(uint8_t factor);

    /**
     * @brief set a name for the compensation range for reference purposes.
     * @param compRangeName the name to set for the compensation range.
    */
    void setCompRangeName(const QString& compRangeName);

    /**
     * @brief get the name set for the compensation range.
     * @return the name set for the compensation range.
    */
    const QString& getCompRangeName() const;

private:
    std::shared_ptr<AisCompRangePrivate> m_data;
};

#endif
