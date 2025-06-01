#include "PressureTransducer.h"
#include "Arduino.h" 
#include <math.h>

PressureTransducer::PressureTransducer(uint8_t pin, float minPressure, float maxPressure, float minVoltage, float maxVoltage, float vRef, int adcBits, float criticalVoltageThresholdFactor)
    : _pin(pin),
      _minPressure(minPressure),
      _maxPressure(maxPressure),
      _minVoltage(minVoltage),
      _maxVoltage(maxVoltage),
      _vRef(vRef),
      _adcResolution((1 << adcBits) - 1), // Calculate max ADC value from bits
      _criticalVoltageThresholdFactor(criticalVoltageThresholdFactor) {
    // Pre-calculate factors for performance optimization
    // For pressure conversion (slope and offset)
    _voltageToPressureSlope = (_maxPressure - _minPressure) / (_maxVoltage - _minVoltage);
    _voltageToPressureOffset = _minPressure - _minVoltage * _voltageToPressureSlope;
    // For range checking in readPressure()
    _minCriticalVoltageThreshold = _minVoltage * (1.0f - _criticalVoltageThresholdFactor);
    _maxCriticalVoltageThreshold = _maxVoltage * (1.0f - _criticalVoltageThresholdFactor) + _vRef * _criticalVoltageThresholdFactor;

    // Factor to convert raw ADC reading to voltage.
    _adcToVoltageFactor = _vRef / (float)_adcResolution;
}

void PressureTransducer::begin() {
    pinMode(_pin, INPUT);
}

unsigned int PressureTransducer::readRaw() {
    return analogRead(_pin);
}

float PressureTransducer::readVoltage() {
    unsigned int rawValue = readRaw();
    // Use pre-calculated factor for ADC to voltage conversion
    return (float)rawValue * _adcToVoltageFactor;
}

float PressureTransducer::readPressure() {
    float voltage = readVoltage();

    // Check if the reading is significantly outside the specified voltage range.
    // If so return NAN, otherwise clamp the value to the range.
    if (voltage < _minVoltage) {
        if (voltage < _minCriticalVoltageThreshold) {
            return NAN;
        }
        voltage = _minVoltage;
    } else if (voltage > _maxVoltage) {
        if (voltage > _maxCriticalVoltageThreshold) {
            return NAN;
        }
        voltage = _maxVoltage;
    }

    // Use pre-calculated slope and offset for linear interpolation
    return voltage * _voltageToPressureSlope + _voltageToPressureOffset;
}