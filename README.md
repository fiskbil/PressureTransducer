# PressureTransducer Arduino Library

This Arduino library makes it easy to read pressure from common analog pressure transducers. It handles the conversion from raw ADC values to voltage, and then to your specified pressure units, taking into account the transducer's specific voltage output range and pressure range.

## Features

*   Simple conversion from raw ADC readings to pressure units.
*   Configurable voltage range (e.g., 0.5V - 4.5V).
*   Configurable pressure range (e.g., 0 PSI - 100 PSI, 0 Bar - 7 Bar).
*   Configurable ADC reference voltage (`Vref`).
*   Configurable ADC resolution (e.g., 10-bit, 12-bit).
*   Handles out-of-range readings gracefully by returning `NAN`.
*   Pre-calculates conversion factors for efficient readings.

## Installation

1.  **Download:** Click the "Code" button on the GitHub repository page and select "Download ZIP".
2.  **Install in Arduino IDE:**
    *   Open the Arduino IDE.
    *   Go to `Sketch` -> `Include Library` -> `Add .ZIP Library...`
    *   Navigate to the downloaded ZIP file and select it.

Alternatively, you can install it via the Arduino Library Manager:
1.  Open the Arduino IDE.
2.  Go to `Sketch` -> `Include Library` -> `Manage Libraries...`
3.  Search for "PressureTransducer" (once published).
4.  Click "Install".

## Basic Usage

Here's a simple example of how to use the library:

```cpp
#include <PressureTransducer.h>

// Define the analog pin your transducer is connected to
const uint8_t SENSOR_PIN = A0;

// Transducer specific parameters
// These are the values for a standard 100 PSI oil/fuel pressure sensor, like the AEM 30-2131-100
const float MIN_PRESSURE = 0.0f;   // PSI
const float MAX_PRESSURE = 100.0f; // PSI
const float MIN_VOLTAGE = 0.5f;    // Volts
const float MAX_VOLTAGE = 4.5f;    // Volts

// ADC specific parameters
const float V_REF = 5.0f;          // Volts
const int ADC_BITS = 10;           // Resolution

// Initialize the transducer object.
// You can use default settings or specify your transducer's parameters.
// This example explicitly sets the default parameters:
PressureTransducer transducer(SENSOR_PIN, MIN_PRESSURE, MAX_PRESSURE,
                              MIN_VOLTAGE, MAX_VOLTAGE, V_REF, ADC_BITS);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  Serial.println("Pressure Transducer Example");

  // Initialize the sensor pin
  transducer.begin();
}

void loop() {
  unsigned int rawValue = transducer.readRaw();
  float voltage = transducer.readVoltage();
  float pressure = transducer.readPressure();

  Serial.print("Raw ADC: "); Serial.print(rawValue);
  Serial.print("\t Voltage: "); Serial.print(voltage, 3); Serial.print(" V");
  Serial.print("\t Pressure: ");
  if (isnan(pressure)) {
    Serial.println("Invalid reading"); // Or "Sensor disconnected/damaged?"
  } else {
    Serial.print(pressure, 2); Serial.println(" PSI"); 
  }

  delay(1000);
}
```

## API Overview

*   `PressureTransducer(pin, minPressure, maxPressure, minVoltage, maxVoltage, vRef, adcBits)`: Constructor.
    *   `pin`: Analog pin.
    *   `minPressure`, `maxPressure`: Transducer's pressure range.
    *   `minVoltage`, `maxVoltage`: Transducer's voltage output range (optional, default to 0.5 - 4.5V).
    *   `vRef`: ADC reference voltage (optional, default to 5.0V).
    *   `adcBits`: ADC resolution in bits (optional, default to 10-bit).
    *   `criticalVoltageThresholdFactor`: Factor for determining invalid readings (optional, default to 0.2f).
*   `begin()`: Initializes the sensor pin. Call in `setup()`.
*   `readRaw()`: Returns the raw ADC value.
*   `readVoltage()`: Returns the calculated voltage.
*   `readPressure()`: Returns the calculated pressure. Returns `NAN` for invalid readings.

## Configuration

All configuration is done via the constructor when creating the `PressureTransducer` object.
```cpp
// Example: 0-150 PSI transducer, 0.5-4.5V output, 5.0V Vref, 12-bit ADC
PressureTransducer mySensor(A1, 0.0f, 150.0f, 0.5f, 4.5f, 5.0f, 12);
```

## License

This library is released under the MIT License.

---
Happy sensing!