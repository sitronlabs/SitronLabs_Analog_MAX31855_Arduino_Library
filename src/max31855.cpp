/* Self header */
#include "max31855.h"

/**
 *
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
 *
 * @param[out] temperature_thermocouple_c
 * @param[out] temperature_internal_c
 * @param[out] is_shorted_vcc
 * @param[out] is_shorted_gnd
 * @param[out] is_open
 * @return 0 in case of success or a negative error code otherwise.
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
