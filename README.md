LMIC_low_power
==============

This is a first attempt to add low-power mode to the [Arduino LMIC port](https://github.com/matthijskooijman/arduino-lmic). The `lmic` folder contains the modified version of the LMIC port based on version `1.5.0+arduino-2`. Modified files are:

- `lmic/config.h`: has an additional `#define LMIC_LOWPOWER` statement
- `lmic/hal.h`: defines `hal_sleep_lowpower(u1_t sleepval)` function
- `hal/hal.cpp`: adds `u4_t os_cumulated_sleep_time_in_seconds`, implements `hal_sleep_lowpower(u1_t sleepval)` function and modifies `hal_ticks()`

`Arduino_LoRa_LMIC_ABP_temp` is the LMIC-based Arduino example similar to the `Arduino_LoRa_temp` [example](https://github.com/CongducPham/LowCostLoRaGw/tree/master/Arduino) of the our [low-cost LoRa IoT framework](https://github.com/CongducPham/LowCostLoRaGw). Its structure and features are the same (simple temperature sensor, keep track of sequence number on reset, low-power consumption down to 5uA in sleep mode) except that it uses the LMIC port to build a LoRaWAN device. For smallest power consumption, please look at our YouTube video tutorial [Extreme low-cost & low-power LoRa IoT for real-world deployment](https://www.youtube.com/watch?v=2_VQpcCwdd8).

`Arduino_LoRa_LMIC_OTAA_temp` is the OTAA version of `Arduino_LoRa_LMIC_ABP_temp`. This example is for demonstrating the usage of an SX1301-based concentrator hat (e.g. RAK831/2245) with our gateway framework. Please refer to this [README](https://github.com/CongducPham/LowCostLoRaGw/blob/master/gw_full_latest/scripts/rak2245-rak831/README.md) for more details.

The main reason of this contribution is because low-power mode on AVR microcontrollers makes both `millis()` and `micros()` to be not updated, causing timing issues in the LMIC stack, especially the time to complete the EV_TXCOMPLETE event. The idea is to store the amount of sleep time in order to add it to the value returned by `hal_ticks()`. A new function `hal_sleep_lowpower(u1_t sleepval)` is introduced to allow the main low-power loop to indicate to the LMIC stack how much sleep time (in seconds) it has to take into account when returning `hal_ticks()`.

Installation
------------ 

- copy both `Arduino_LoRa_LMIC_ABP_temp` and `Arduino_LoRa_LMIC_OTAA_temp` into your Arduino sketch folder
- copy `lmic` into your Arduino `libraries` folder

Flash a device
--------------

You will need an Arduino device such as the Arduino Pro Mini in 8MHz and 3.3v version. Connect a LoRa radio module and change the DIO mapping in `Arduino_LoRa_LMIC_ABP_temp` and `Arduino_LoRa_LMIC_OTAA_temp` according to your configuration. The example uses DR_SF12.

You have several options of PCBs:

- our open source [ProMini PCB](https://github.com/CongducPham/LowCostLoRaGw#pcbs) that uses D2, D3, D5 and D4 for DIO0, DIO1, DIO2 and RST respectively (provided that the corresponding solder pads are soldered together).

- the open-source [UCA Pro Mini breakout](https://github.com/FabienFerrero/UCA_Board) from my colleague Fabien Ferrero that uses D2, D7, D9 and D8 for DIO0, DIO1, DIO2 and RST respectively.

- the open source [Mini-Lora](https://github.com/hallard/Mini-LoRa) from C. Hallard that uses D2, D7, D8 and A0 for DIO0, DIO1, DIO2 and RST respectively.

- the [LoRa Radio node](https://www.tindie.com/products/IOTMCU/lora-radio-node-v10/) from IOTMCU that uses D2 for DIO0 and D9 for RST. Then you can use jumper wires to connect DIO1 and DIO2 (these are available on a pin header) to some available pins (such as D3 and D4 for instance).

- the [LoRa Nexus](https://webshop.ideetron.nl/Nexus) from Ideetron that uses D4, D5, D7 and Arduino's RST for DIO0, DIO1, DIO2 and RST respectively.

In order to use `Arduino_LoRa_LMIC_ABP_temp`, you need to create an ABP device on TTN for instance to get the device short address (32 bits) and, if you want, both NwkSKey and AppSKey. These information have to be filled in the example code. A nice tutorial can be found on https://medium.com/kkbankol-events/tutorial-build-a-open-source-smart-city-based-on-lora-7ca76b9a098.

In order to use `Arduino_LoRa_LMIC_ABP_temp`, you need to create an OTAA device on TTN.

The default sleep time is 5min, `unsigned int TX_INTERVAL = 5*60;`. In a real-world application, set it to more than 10mins or higher.

Output from Serial Monitor
--------------------------

Here is a typical output from Arduino IDE's Serial Monitor with both `#define SHOW_LOW_POWER_CYCLE` and `#define SHOW_LMIC_LOWPOWER_TIMING` uncommented. To deploy a sensor device for a real-world scenario, comment these lines.

```
LoRa temperature sensor, LMIC extended version
Arduino Pro Mini detected
ATmega328P detected
Get back previous sx1272 config
Using seqnoUp of 1
Forced to use default parameters
Using idle period of 300
Reading 244
Reading 238
Reading 235
Reading 233
Reading 230
Mean temp is 22.25
Sending \!TC/22.25
Real payload size is 10
234974: Packet queued
454645: EV_TXCOMPLETE (includes waiting for RX windows)
diff in ticks: 219671
diff in seconds: 3
now micros: 7287424
cumulated sleep: 0
now ticks: 455464
now ticks from os_getTime(): 455464
Switch to power saving mode
8888888888888888888888888888888888881D[860]
Reading 305
Reading 302
Reading 299
Reading 296
Reading 292
Mean temp is 22.25
Sending \!TC/22.25
Real payload size is 10
18623475: Packet queued
18843145: EV_TXCOMPLETE (includes waiting for RX windows)
diff in ticks: 219670
diff in seconds: 3
now micros: 12504112
cumulated sleep: 289
now ticks: 18844007
now ticks from os_getTime(): 18844007
Switch to power saving mode
888888888888
```

As you can see, after the wake up from the sleep period of 5mins, `EV_TXCOMPLETE` happens after about 3s ("`diff in seconds: 3`") which means that the transmission happen almost immediately. This is actually the normal behavior as a sleep time of 5mins is more than the minimum off-time imposed by the LMIC's duty-cycle rule (about 146s for DR_SF12). Therefore the transmission can be scheduled to happen immediately, validating the correct timing of the LMIC stack after a sleep period.

If you compared to the output below when there is no specific low-power management (unmodified LMIC stack) then you can see that in addition to sleeping for 5mins, on wake up LMIC schedules the next transmission to be 146s in the future ("`diff in seconds: 146`") as its internal clock based on `micros()` has not advanced while the microcontroller was put in sleep mode.

```
LoRa temperature sensor, LMIC extended version
Arduino Pro Mini detected
ATmega328P detected
Get back previous sx1272 config
Using seqnoUp of 1
Forced to use default parameters
Using idle period of 300
Reading 232
Reading 226
Reading 222
Reading 219
Reading 216
Mean temp is 22.25
Sending \!TC/22.25
Real payload size is 10
235132: Packet queued
454801: EV_TXCOMPLETE (includes waiting for RX windows)
diff in ticks: 219669
diff in seconds: 3
now micros: 7289896
now ticks: 455618
now ticks from os_getTime(): 455619
Switch to power saving mode
8888888888888888888888888888888888881D[866]
Reading 291
Reading 287
Reading 283
Reading 279
Reading 275
Mean temp is 22.25
Sending \!TC/22.25
Real payload size is 10
560285: Packet queued
9722398: EV_TXCOMPLETE (includes waiting for RX windows)
diff in ticks: 9162113
diff in seconds: 146
now micros: 155572568
now ticks: 9723285
now ticks from os_getTime(): 9723286
Switch to power saving mode
88888888888888888841D[484]
```

Enjoy! C. Pham