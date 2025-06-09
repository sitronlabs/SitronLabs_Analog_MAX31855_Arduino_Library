/* Self header */
#include "max31855.h"

/**
 * @brief Initialize the MAX31855 device
 *
 * This function configures the SPI interface and GPIO pins for communication
 * with the MAX31855 thermocouple-to-digital converter. It validates the SPI
 * speed to ensure it's within the device's specifications and sets up the
 * chip select pin.
 *
 * @param spi_library Reference to the SPI library instance to use
 * @param spi_speed SPI clock speed in Hz (must not exceed 5MHz)
 * @param pin_cs Chip select pin number
 * @return 0 on success, -EINVAL if spi_speed exceeds 5MHz
 */
int max31855::setup(SPIClass &spi_library, const int spi_speed, const int pin_cs) {

    /* Ensure spi speed is within supported range */
    if (spi_speed > 5000000) {
        return -EINVAL;
    }

    /* Save parameters */
    m_spi_library = &spi_library;
    m_spi_settings = SPISettings(spi_speed, MSBFIRST, SPI_MODE0);
    m_pin_cs = pin_cs;

    /* Configure gpios */
    pinMode(m_pin_cs, OUTPUT);
    digitalWrite(m_pin_cs, HIGH);

    /* Return success */
    return 0;
}

/**
 * @brief Read temperature and diagnostic data from the MAX31855
 *
 * This function performs a complete read operation from the MAX31855 device.
 * It reads the 32-bit register that contains both temperature measurements
 * (thermocouple and internal reference junction) as well as fault detection flags.
 * The temperature values are converted from the raw register format to floating
 * point Celsius values.
 *
 * The thermocouple temperature has a resolution of 0.25°C.
 * The internal temperature has a resolution of 0.0625°C.
 *
 * @param[out] temperature_thermocouple_c Thermocouple temperature in Celsius
 * @param[out] temperature_internal_c Internal reference junction temperature in Celsius
 * @param[out] is_shorted_vcc True if thermocouple is shorted to VCC
 * @param[out] is_shorted_gnd True if thermocouple is shorted to GND
 * @param[out] is_open True if thermocouple connection is open
 * @return 0 on success
 */
int max31855::read(float &temperature_thermocouple_c, float &temperature_internal_c, bool &is_shorted_vcc, bool &is_shorted_gnd, bool &is_open) {

    /* Read register from device */
    uint32_t reg = 0;
    m_spi_library->beginTransaction(m_spi_settings);
    digitalWrite(m_pin_cs, LOW);
    reg |= m_spi_library->transfer(0x00);
    reg <<= 8;
    reg |= m_spi_library->transfer(0x00);
    reg <<= 8;
    reg |= m_spi_library->transfer(0x00);
    reg <<= 8;
    reg |= m_spi_library->transfer(0x00);
    digitalWrite(m_pin_cs, HIGH);
    m_spi_library->endTransaction();

    /* Extract temperatures */
    temperature_thermocouple_c = ((reg & 0x80000000) ? -.25 : +.25) * ((reg >> 18) & 0x00003FFFF);
    temperature_internal_c = ((reg & 0x8000) ? -.0625 : +.0625) * ((reg >> 4) & 0x7FF);

    /* Extract error flags */
    is_shorted_vcc = (reg & 0x04) ? 1 : 0;
    is_shorted_gnd = (reg & 0x02) ? 1 : 0;
    is_open = (reg & 0x01) ? 1 : 0;

    /* Return success */
    return 0;
}
