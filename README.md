# AutomatedGreenHouse

Automated Green House system


## Structure of the project

There are different modules
 - Outdoor module
 - Indoor module
 - Local module : inside the greenhouse, sensor local values and control actuators (water pump, heater, fan)


## Commponents used

- BMP280 :
    - Pressure sensor from 300 to 1 100 hPa  
    - Temperature measurement for more accurate results from -40 to + 85 °C
    - [Sensor technical sheet on manufacturer website](https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/)
- SCD30 :
    - NDIR technology for accurate CO2 readings from 400 to 10,000 ppm
    - Measure temperature and relative humidity for more accurate measure
    - Local altitude compensation in the setup function for more accurate measure
    - [Link to the manufacturer website](https://sensirion.com/products/catalog/SCD30)
- DS3231 GT584:
    - Give accurate date & time (secondes, minutes, hours) and date (day, month, year, day of the week)
    - Leap-Year Compensation Valid Up to 2100
    - Crystal oscillator is temperature compensated (TCXO) for more accurate measures
    - Battery is a common CR2032
    - [Link to the DS3231 Data Sheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ds3231.pdf)
- BH1750 (to come)
- ESP32-C6-DevKit C1
    - System on a Chip (SoC)
    - [Link to the manufacturer website](https://www.espressif.com/en/products/socs/esp32-c6)



## Outdoor Module

### Components

- ESP32-C6-DevKit C1
- SCD30
- BMP280
- BH1750
- DS3231

### Electronic diagram

Interface used : I²C

![Electronic diagram](Outdoor_module.png)

### IDE & Librairies

Developed on Arduino IDE 2.3.6

Librairies required : 
 - for I2C interface : Wire.h
 - for SCD30 : SparkFun_SCD30_Arduino_Library.h
 - for DS3231 : RTClib.h>
 - for BMP280 : Adafruit_BMP280.h
 - for BH1750 : BH1750.h and Adafruit_Sensor.h


### Example of output

```
23:14:03.443 -> ESP-ROM:esp32c6-20220919
23:14:03.443 -> Build:Sep 19 2022
23:14:03.443 -> rst:0x1 (POWERON),boot:0x2c (SPI_FAST_FLASH_BOOT)
23:14:03.443 -> SPIWP:0xee
23:14:03.443 -> mode:DIO, clock div:2
23:14:03.443 -> load:0x40875720,len:0x1228
23:14:03.474 -> load:0x4086c110,len:0xd9c
23:14:03.474 -> load:0x4086e610,len:0x2f74
23:14:03.474 -> entry 0x4086c110
23:14:03.538 -> Outdoor Module - Environmental Monitoring System
23:14:03.538 -> RTC initialized!
23:14:03.667 -> BMP280 sensor initialized!
23:14:03.667 -> SCD30 sensor initialized!
23:14:03.699 -> BH1750 sensor initialized!
23:14:03.699 -> All sensors ready!
23:14:03.699 -> Taking measurements every 60 seconds...
23:14:03.699 -> 
23:14:03.699 -> ==== New Measurement ====
23:14:03.699 -> Timestamp: 2025-05-03 23:13:42
23:14:03.730 -> --- BMP280 Data ---
23:14:03.730 -> Temperature: 26.85 °C
23:14:03.730 -> Pressure: 1003.60 hPa
23:14:03.730 -> Approx. Altitude: 80.64 m
23:14:03.730 -> --- SCD30 Data ---
23:14:03.730 -> CO2: 719.00 ppm
23:14:03.763 -> Temperature: 28.05 °C
23:14:03.763 -> Humidity: 52.11 %
23:14:03.763 -> --- BH1750 Data ---
23:14:03.763 -> Light: 45.83 lux
23:14:03.763 -> --- RTC Data ---
23:14:03.763 -> RTC Temperature: 26.00 °C
23:14:03.763 -> 
```


## Indoor module 

(to come)


## Local module

(to come)