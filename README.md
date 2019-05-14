LMIC_low_power
==============

This is a first attempt to add low-power mode to the [Arduino LMIC port](https://github.com/matthijskooijman/arduino-lmic). The `lmic` folder contains the modified version of the LMIC port based on version `1.5.0+arduino-2`. Modified files are:

- `lmic/config.h`
- `lmic/hal.h`
- `hal/hal.cpp`

`Arduino_LoRa_LMIC_ABP_temp` is the LMIC-based Arduino example similar to the `Arduino_LoRa_temp` [example](https://github.com/CongducPham/LowCostLoRaGw/tree/master/Arduino) of the our [low-cost LoRa IoT framework](https://github.com/CongducPham/LowCostLoRaGw). Its structure and features are the same (simple temperature sensor, keep track of sequence number on reset, low-power consumption down to 5uA in sleep mode) except that it uses the LMIC port to build a LoRaWAN device. For smallest power consumption, please look at our YouTube video tutorial [Extreme low-cost & low-power LoRa IoT for real-world deployment](https://www.youtube.com/watch?v=2_VQpcCwdd8).

The main reason of this contribution is because low-power mode on AVR microcontrollers makes both `millis()` and `micros()` to be not updated, causing timing issues in the LMIC stack, especially the time to complete the EV_TXCOMPLETE event. The idea is to store the amount of sleep time in order to add it to the value returned by `hal_ticks()`. A new function `hal_sleep_lowpower(u1_t sleepval)` is introduced to allow the main low-power loop to indicate to the LMIC stack how much sleep time (in seconds) it has to take into account when returning `hal_ticks()`.

Installation
------------ 

- copy `Arduino_LoRa_LMIC_ABP_temp` into your Arduino sketch folder
- copy `lmic` into your Arduino `libraries` folder

Flash a device
--------------

You will need an Arduino device such as the Arduino Pro Mini in 8MHz and 3.3v version. Connect a LoRa radio module and change the DIO mapping in `Arduino_LoRa_LMIC_ABP_temp` according to your configuration. 

You have several options of PCBs:

- our open source [ProMini PCB](https://github.com/CongducPham/LowCostLoRaGw#pcbs): DIO0, DIO1, RST and DIO2 are connected to D2, D3, D4 and D5 respectively if the corresponding solder pads are soldered together.

- the open-source [UCA Pro Mini breakout](https://github.com/FabienFerrero/UCA_Board) from my colleague Fabien Ferrero that uses D2, D7, D9 and D8 for DIO0, DIO1, DIO2 and RST respectively.

- the open source []Mini-Lora](https://github.com/hallard/Mini-LoRa) from C. Hallard that uses D2, D7, D8 and A0 for DIO0, DIO1, DIO2 and RST respectively.

In order to use `Arduino_LoRa_LMIC_ABP_temp`, you need to create a device for instance on TTN to get the device short address (32 bits) and, if you want, both NwkSKey and AppSKey. These information have to be filled in the example code. A nice tutorial can be found on https://medium.com/kkbankol-events/tutorial-build-a-open-source-smart-city-based-on-lora-7ca76b9a098. 

The default sleep time is 5min, `unsigned int TX_INTERVAL = 5*60;`. In a real-world application, set it to more than 10mins or higher.

Enjoy! C. Pham