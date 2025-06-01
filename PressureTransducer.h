#ifndef PressureTransducer_h
#define PressureTransducer_h
#include "Arduino.h"

class PressureTransducer {
public:
    // Default values for constructor parameters
    static constexpr float DEFAULT_MIN_VOLTAGE = 0.5f;
    static constexpr float DEFAULT_MAX_VOLTAGE = 4.5f;
    static constexpr float DEFAULT_VREF = 5.0f;
    static constexpr int   DEFAULT_ADC_BITS = 10;
    static constexpr float DEFAULT_CRITICAL_VOLTAGE_THRESHOLD_FACTOR = 0.2f;

    // Constructor
    // pin: Analog pin connected to the transducer.
    // minVoltage: Voltage output by the transducer at minimum pressure (e.g., 0.5V).
    // maxVoltage: Voltage output by the transducer at maximum pressure (e.g., 4.5V).
    // minPressure: Minimum pressure the transducer can read (e.g., 0 PSI).
    // maxPressure: Maximum pressure the transducer can read (e.g., 100 PSI).
    // vRef: ADC reference voltage (default 5.0V for many Arduinos).
    // adcBits: Number of ADC resolution bits (e.g., 10 for 10-bit ADC, results in 0-1023 range).
    // criticalVoltageThresholdFactor: Factor to determine how far outside the calibrated voltage range a reading can be before it's considered invalid.
    PressureTransducer(
        uint8_t pin,
        float minPressure,
        float maxPressure,
        float minVoltage = DEFAULT_MIN_VOLTAGE,
        float maxVoltage = DEFAULT_MAX_VOLTAGE,
        float vRef = DEFAULT_VREF,
        int adcBits = DEFAULT_ADC_BITS,
        float criticalVoltageThresholdFactor = DEFAULT_CRITICAL_VOLTAGE_THRESHOLD_FACTOR);

    /**
     * @brief Initializes the analog input pin. Call this in your setup() function.
     */
    void begin();

    /**
     * @brief Reads the raw analog-to-digital converter (ADC) value from the sensor.
     * @return The raw ADC value (e.g., 0-1023 for a 10-bit ADC).
     */
    unsigned int readRaw();
    /**
     * @brief Reads the voltage from the sensor.
     * @return The voltage reading in Volts.
     */
    float readVoltage();
    /**
     * @brief Reads the pressure from the sensor.
     * @return The pressure reading in the units defined by minPressure/maxPressure. Returns NAN if the reading is considered invalid.
     */
    float readPressure();

private:
    uint8_t _pin;                       ///< Analog pin connected to the transducer.
    float _minVoltage;                  ///< Minimum voltage output of the transducer.
    float _maxVoltage;                  ///< Maximum voltage output of the transducer.
    float _minPressure;                 ///< Minimum pressure reading corresponding to _minVoltage.
    float _maxPressure;                 ///< Maximum pressure reading corresponding to _maxVoltage.
    float _vRef;                        ///< ADC reference voltage.
    int _adcResolution;                 ///< Maximum ADC value (e.g., 1023 for 10-bit).
    float _criticalVoltageThresholdFactor; ///< Factor for critical voltage threshold calculation.

    // Pre-calculated factors for performance optimization
    float _voltageToPressureSlope;      ///< Pre-calculated slope for voltage to pressure conversion.
    float _voltageToPressureOffset;     ///< Pre-calculated offset for voltage to pressure conversion.
    float _minCriticalVoltageThreshold; ///< Pre-calculated lower threshold for invalid voltage readings.
    float _maxCriticalVoltageThreshold; ///< Pre-calculated upper threshold for invalid voltage readings.
    float _adcToVoltageFactor;          ///< Pre-calculated factor for ADC raw value to voltage conversion.
};

#endif