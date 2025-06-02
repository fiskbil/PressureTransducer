#include <PressureTransducer.h>

// Analog pin the transducer is connected to (e.g., A0, A1, etc.)
const uint8_t SENSOR_PIN = A0;

// Define the pressure range for your specific transducer.
// For example, if your transducer reads from 0 to 100 PSI:
const float MIN_PRESSURE = 0.0f;   // Minimum pressure in PSI
const float MAX_PRESSURE = 100.0f; // Maximum pressure in PSI

PressureTransducer transducer(SENSOR_PIN, MIN_PRESSURE, MAX_PRESSURE);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB on some boards.
  }
  Serial.println("Pressure Transducer Example");

  transducer.begin(); 
}

void loop() {
  unsigned int rawValue = transducer.readRaw(); // Changed to unsigned int to match readRaw() return type
  float voltage = transducer.readVoltage();
  float pressure = transducer.readPressure();

  Serial.print("Raw ADC: "); Serial.print(rawValue);
  Serial.print("\t Voltage: "); Serial.print(voltage, 3); Serial.print(" V");
  Serial.print("\t Pressure: ");
  if (isnan(pressure)) {
    Serial.println("Invalid reading, sensor might be damaged.");
  } else {
    Serial.print(pressure, 2); Serial.println(" PSI");
  }
  delay(1000); // Wait for a second before the next reading
}