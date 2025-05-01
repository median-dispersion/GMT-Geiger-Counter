#ifndef _GEIGER_COUNTER_H
#define _GEIGER_COUNTER_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "Tube.h"

class GeigerCounter {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Radiation rating enumerator
    enum RadiationRating {

      RATING_NORMAL,
      RATING_ELEVATED,
      RATING_MEDIUM,
      RATING_HIGH,
      RATING_EXTREME

    };

    // Measurement unit enumerator
    enum MeasurementUnit {

      SIEVERTS,
      REM,
      RONTGEN,
      GRAY

    };

    // Equivalent dose unit enumerator
    enum DoseUnit {

      MICRO_SIEVERTS_PER_HOUR,
      MILLI_SIEVERTS_PER_HOUR,
      BASE_SIEVERTS_PER_HOUR,
      MICRO_REM_PER_HOUR,
      MILLI_REM_PER_HOUR,
      BASE_REM_PER_HOUR,
      MICRO_RONTGEN_PER_HOUR,
      MILLI_RONTGEN_PER_HOUR,
      BASE_RONTGEN_PER_HOUR,
      MICRO_GRAY_PER_HOUR,
      MILLI_GRAY_PER_HOUR,
      BASE_GRAY_PER_HOUR

    };

    // Get the single instance of the class
    static GeigerCounter& getInstance();

    void            begin();                                            // Initialize everything
    void            enable();                                           // Enable the Geiger counter
    void            disable();                                          // Disable the Geiger counter
    void            setGeigerCounterState(const bool state);            // Set the Geiger counter state
    void            setIntegrationTime(const uint8_t timeSeconds);      // Set the integration time
    void            setAutoIntegrateState(const bool state);            // Set the state of the automatic integration time adjustment
    void            setAutoRangeState(const bool state);                // Set if the equivalent dose should auto range
    void            setMeasurementUnit(const MeasurementUnit unit);     // Set the measurement unit of the equivalent dose
    bool            getGeigerCounterState();                            // Get the Geiger counter state
    uint8_t         getIntegrationTime();                               // Get the set integration time
    bool            getAutoIntegrateState();                            // Get the state of the automatic integration time adjustment
    bool            getAutoRangeState();                                // Get the auto ranging state
    MeasurementUnit getMeasurementUnit();                               // Get the measurement unit
    uint64_t        getCounts();                                        // Get the total number of counts
    uint64_t        getMainTubeCounts();                                // Get the number of counts the main tube has recorded
    uint64_t        getFollowerTubeCounts();                            // Get the number of counts the follower tube has recorded
    double          getCountsPerMinute(const uint8_t timeSeconds);      // Get the counts per minute for a fixed integration time
    double          getCountsPerMinute();                               // Get the counts per minute
    double          getMicrosievertsPerHour(const uint8_t timeSeconds); // Get microsieverts per hour for a fixed integration time
    double          getMicrosievertsPerHour();                          // Get microsieverts per hour
    double          getEquivalentDose();                                // Get the equivalent dose in the selected measurement unit
    DoseUnit        getEquivalentDoseUnit();                            // Get the equivalent dose unit
    RadiationRating getRadiationRating();                               // Get the radiation rating
    uint32_t*       getHistory();                                       // Get a pointer to the radiation history array
    uint8_t         getHistoryIndex();                                  // Get the index of the radiation history array

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    GeigerCounter();
    GeigerCounter(const GeigerCounter&) = delete;
    GeigerCounter& operator=(const GeigerCounter&) = delete;

    // Metric prefix enumerator 
    enum MetricPrefix {

      PREFIX_MICRO,
      PREFIX_MILLI,
      PREFIX_BASE

    };

    bool              _initialized;                               // Flag for checking if the Geiger counter is initialized
    volatile uint16_t _movingAverage[60];                         // An array for storing counts per second for a duration of 60 seconds
    volatile uint8_t  _movingAverageIndex;                        // Index of the moving average array
    hw_timer_t        *_movingAverageTimer;                       // Hardware timer for advancing the moving average array
    Tube              _mainTube;                                  // Main Tube
    Tube              _followerTube;                              // Follower tube
    bool              _enabled;                                   // Flag for checking if Geiger counter is enabled
    uint8_t           _integrationTimeSeconds;                    // Number of seconds to use from the moving average array to average over 
    bool              _autoIntegrate;                             // Flag for enabling automatic adjustment of the integration time
    bool              _autoRange;                                 // Flag for enabling equivalent dose auto ranging
    MeasurementUnit   _measurementUnit;                           // Selected measurement unit
    MetricPrefix      _metricPrefix;                              // Current metric prefix
    uint32_t          _history[RADIATION_HISTORY_LENGTH_MINUTES]; // Radiation history array
    uint8_t           _historyIndex;                              // Index of the radiation history array
    uint8_t           _historyTimerSeconds;                       // Radiation history update timer

    // Interrupt service routine for updating the Geiger counter
    static void IRAM_ATTR _update(void *instancePointer);

};

// Global reference to the Geiger counter instance for easy access
extern GeigerCounter& geigerCounter;

#endif