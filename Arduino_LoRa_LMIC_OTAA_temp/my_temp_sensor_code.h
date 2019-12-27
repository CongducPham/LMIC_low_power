#ifndef MY_TEMP_SENSOR_CODE
#define MY_TEMP_SENSOR_CODE

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

extern char nomenclature_str[4];
void sensor_Init();
double sensor_getValue();

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE READ PIN AND THE POWER PIN FOR THE TEMP. SENSOR
#define PIN_READ  A0
// use digital 6 to power the temperature sensor if needed
// actually you have to use a pin that is not used for the LMIC stack
// As the LMIC pins depends on the board you are using, you have to change according to your configuration.
#define PIN_POWER 6
///////////////////////////////////////////////////////////////////

#if defined ARDUINO_AVR_PRO || defined ARDUINO_AVR_MINI || defined ARDUINO_SAM_DUE || defined __MK20DX256__  || defined __MKL26Z64__ || defined __MK64FX512__ || defined __MK66FX1M0__ || defined __SAMD21G18A__
  // if you have a Pro Mini running at 5V, then change here
  // these boards work in 3.3V
  // Nexus board from Ideetron is a Mini
  // __MK66FX1M0__ is for Teensy36
  // __MK64FX512__  is for Teensy35
  // __MK20DX256__ is for Teensy31/32
  // __MKL26Z64__ is for TeensyLC
  // __SAMD21G18A__ is for Zero/M0 and FeatherM0 (Cortex-M0)
  #define VOLTAGE_SCALE  3300.0
#else // ARDUINO_AVR_NANO || defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_MEGA2560 
  // also for all other boards, so change here if required.
  #define VOLTAGE_SCALE  5000.0
#endif

#endif
