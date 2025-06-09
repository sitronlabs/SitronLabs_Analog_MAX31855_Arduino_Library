#ifndef MAX31855_H
#define MAX31855_H

/* Arduino libraries */
#include <Arduino.h>
#include <SPI.h>

/* C/C++ library */
#include <errno.h>

/**
 * @brief MAX31855 thermocouple-to-digital converter driver class
 *
 * This class provides an interface to communicate with the MAX31855 cold-junction
 * compensated thermocouple-to-digital converter using SPI interface.
 */
class max31855 {
   public:
    //!@{
    //! Initialization and setup
    int setup(SPIClass &spi_library, const int spi_speed, const int pin_cs);
    //!@}

    //!@{
    //!  emperature reading and diagnostics
    int read(float &temperature_thermocouple_c, float &temperature_internal_c, bool &is_shorted_vcc, bool &is_shorted_gnd, bool &is_open);
    //!@}

   protected:
    SPIClass *m_spi_library = NULL;  //!< Pointer to SPI library instance
    SPISettings m_spi_settings;      //!< SPI communication settings
    int m_pin_cs = 0;                //!< Chip select pin number
};

#endif
