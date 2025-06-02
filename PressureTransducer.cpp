#include <Arduino.h>
#include <math.h>
#include "PressureTransducer.h"

PressureTransducer::PressureTransducer(uint8_t pin, float minPressure, float maxPressure, float minVoltage, float maxVoltage, float vRef, int adcBits, float validVoltageThresholdFactor)
    : _pin(pin),
      _minPressure(minPressure),
      _maxPressure(maxPressure),
      _minVoltage(minVoltage),
      _maxVoltage(maxVoltage),
      _vRef(vRef),
      _adcResolution((1 << adcBits) - 1), // Calculate max ADC value from bits
      _validVoltageThresholdFactor(validVoltageThresholdFactor) {
    // Pre-calculate factors for performance optimization
    // For pressure conversion (slope and offset)
    _voltageToPressureSlope = (_maxPressure - _minPressure) / (_maxVoltage - _minVoltage);
    _voltageToPressureOffset = _minPressure - _minVoltage * _voltageToPressureSlope;
    // For range checking in readPressure()
    _minValidVoltageThreshold = _minVoltage * (1.0f - _validVoltageThresholdFactor);
    _maxValidVoltageThreshold = _maxVoltage * (1.0f - _validVoltageThresholdFactor) + _vRef * _validVoltageThresholdFactor;

    // Factor to convert raw ADC reading to voltage.
    _adcToVoltageFactor = _vRef / (float)_adcResolution;
}

void PressureTransducer::begin() {
    pinMode(_pin, INPUT);
}

unsigned int PressureTransducer::readRaw() const {
    return analogRead(_pin);
}

float PressureTransducer::readVoltage() const {
    unsigned int rawValue = readRaw();
    // Use pre-calculated factor for ADC to voltage conversion
    return (float)rawValue * _adcToVoltageFactor;
}

float PressureTransducer::readPressure() const {
    float voltage = readVoltage();

    // Check if the reading is significantly outside the specified voltage range.
    // If so return NAN, otherwise clamp the value to the range.
    if (voltage < _minVoltage) {
        if (voltage < _minValidVoltageThreshold) {
            return NAN;
        }
        voltage = _minVoltage;
    } else if (voltage > _maxVoltage) {
        if (voltage > _maxValidVoltageThreshold) {
            return NAN;
        }
        voltage = _maxVoltage;
    }

    // Use pre-calculated slope and offset for linear interpolation
    return voltage * _voltageToPressureSlope + _voltageToPressureOffset;
}