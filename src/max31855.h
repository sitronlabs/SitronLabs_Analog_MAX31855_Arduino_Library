#ifndef MAX31855_H
#define MAX31855_H

/* Arduino libraries */
#include <Arduino.h>
#include <SPI.h>

/* C/C++ library */
#include <errno.h>

/**
 *
 */
class max31855 {
   public:
    /* Setup */
    int setup(SPIClass &spi_library, const int spi_speed, const int pin_cs);

    /* Read */
    int read(float &temperature_thermocouple_c, float &temperature_internal_c, bool &is_shorted_vcc, bool &is_shorted_gnd, bool &is_open);

   protected:
    SPIClass *m_spi_library = NULL;
    SPISettings m_spi_settings;
    int m_pin_cs = 0;
};

#endif
