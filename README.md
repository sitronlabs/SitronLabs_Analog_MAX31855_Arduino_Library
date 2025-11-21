[![Designed by Sitron Labs](https://img.shields.io/badge/Designed_by-Sitron_Labs-FCE477)](https://www.sitronlabs.com/)
[![Join the Discord community](https://img.shields.io/discord/552242187665145866?logo=discord&logoColor=white&label=Discord&color=%237289da)](https://discord.gg/btnVDeWhfW)
![License](https://img.shields.io/github/license/sitronlabs/SitronLabs_Analog_MAX31855_Arduino_Library)
![Latest Release](https://img.shields.io/github/release/sitronlabs/SitronLabs_Analog_MAX31855_Arduino_Library)
[![Arduino Library Manager](https://www.ardu-badge.com/badge/Sitron%20Labs%20MAX31855%20Arduino%20Library.svg?)](https://www.ardu-badge.com/Sitron%20Labs%20MAX31855%20Arduino%20Library)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/sitronlabs/library/Sitron_Labs_MAX31855_Arduino_Library.svg)](https://registry.platformio.org/libraries/sitronlabs/Sitron_Labs_MAX31855_Arduino_Library)

# Sitron Labs MAX31855 Arduino Library

Arduino library for interfacing with the Analog Devices MAX31855 thermocouple-to-digital converter.

## Description

The MAX31855 is a cold-junction compensated thermocouple-to-digital converter. The MAX31855 is available in multiple variants, each designed for a specific thermocouple type (K, J, N, T, S, R, or E). This library provides a simple interface to read temperature data and diagnostic information from the device via SPI.

## Installation

### Arduino IDE

Install via the Arduino Library Manager by searching for "Sitron Labs MAX31855".

Alternatively, install manually:
1. Download or clone this repository
2. Place it in your Arduino `libraries` folder
3. Restart the Arduino IDE

### PlatformIO

Install via the PlatformIO Library Manager by searching for "Sitron Labs MAX31855".

Alternatively, add the library manually to your `platformio.ini` file:

```ini
lib_deps = 
    https://github.com/sitronlabs/SitronLabs_Analog_MAX31855_Arduino_Library.git
```

## Hardware Connections

Connect the MAX31855 to your Arduino using SPI:

- VCC → 3.3V (check your board's specifications)
- GND → GND
- SCK → SCLK (Serial Clock)
- CS → Any digital pin (configure in code)
- SO → MISO (Master In Slave Out)

## Usage

```cpp
#include <SPI.h>
#include <max31855.h>

// Create sensor object
max31855 sensor;

// Define chip select pin
const int CS_PIN = 10;

void setup() {
  Serial.begin(9600);
  
  // Initialize SPI
  SPI.begin();
  
  // Setup the MAX31855 (SPI instance, speed in Hz, CS pin)
  // Maximum SPI speed is 5MHz
  sensor.setup(SPI, 1000000, CS_PIN);
}

void loop() {
  float temp_thermocouple;
  float temp_internal;
  bool shorted_vcc;
  bool shorted_gnd;
  bool open_circuit;
  
  // Read temperature and diagnostics
  sensor.read(temp_thermocouple, temp_internal, shorted_vcc, shorted_gnd, open_circuit);
  
  // Check for faults
  if (open_circuit) {
    Serial.println("Error: Thermocouple open circuit");
  } else if (shorted_vcc) {
    Serial.println("Error: Thermocouple shorted to VCC");
  } else if (shorted_gnd) {
    Serial.println("Error: Thermocouple shorted to GND");
  } else {
    Serial.print("Thermocouple: ");
    Serial.print(temp_thermocouple);
    Serial.print(" C, Internal: ");
    Serial.print(temp_internal);
    Serial.println(" C");
  }
  
  delay(1000);
}
```

## API Reference

### setup(SPIClass &spi_library, const int spi_speed, const int pin_cs)

Initializes the MAX31855 sensor.

- `spi_library`: SPI instance to use (typically `SPI`)
- `spi_speed`: SPI clock speed in Hz (maximum 5MHz)
- `pin_cs`: Chip select pin number

Returns 0 on success, or -EINVAL if SPI speed exceeds 5MHz.

### read(float &temperature_thermocouple_c, float &temperature_internal_c, bool &is_shorted_vcc, bool &is_shorted_gnd, bool &is_open)

Reads temperature data and diagnostic flags from the sensor.

- `temperature_thermocouple_c`: Output parameter for thermocouple temperature in Celsius (0.25°C resolution)
- `temperature_internal_c`: Output parameter for internal reference junction temperature in Celsius (0.0625°C resolution)
- `is_shorted_vcc`: Output parameter indicating if thermocouple is shorted to VCC
- `is_shorted_gnd`: Output parameter indicating if thermocouple is shorted to GND
- `is_open`: Output parameter indicating if thermocouple connection is open

Returns 0 on success.

## Specifications

- Thermocouple temperature resolution: 0.25°C
- Internal temperature resolution: 0.0625°C
- Maximum SPI speed: 5MHz
- Supports K, J, N, T, S, R, and E type thermocouples (depending on MAX31855 variant used)
