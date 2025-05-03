/**
 * Outdoor Module - Environmental Monitoring System with RTC
 * 
 * This code reads data from:
 * - BMP280: Temperature, Pressure
 * - SCD30: CO2, Temperature, Humidity
 * - BH1750: Light Intensity
 * - DS3231: Real-time Clock (Date and Time)
 * 
 * Readings are taken every 60 seconds and printed to Serial with timestamp
 * 
 */

 #include <Wire.h>
 #include <Adafruit_Sensor.h>
 #include <Adafruit_BMP280.h>
 #include <SparkFun_SCD30_Arduino_Library.h>
 #include <BH1750.h>
 #include <RTClib.h>  // RTC library for DS3231
 
 // Sensor objects
 Adafruit_BMP280 bmp;
 SCD30 scd30;
 BH1750 lightMeter;
 RTC_DS3231 rtc; 
 
 // Timing variables
 const unsigned long MEASUREMENT_INTERVAL = 60000; // 60 seconds in milliseconds
 unsigned long lastMeasurementTime = 0;
 
 // I2C pins for ESP32-C6
 #define SDA_PIN 6  // GPIO6 for SDA
 #define SCL_PIN 7  // GPIO7 for SCL
 
 // Measurement status flags
 bool bmpMeasurementSuccess = false;
 bool scd30MeasurementSuccess = false;
 bool bh1750MeasurementSuccess = false;
 bool rtcMeasurementSuccess = false;
 
 // Function to print formatted date and time
 void printDateTime(DateTime dt) {
   char dateString[20];
   char timeString[20];
   
   sprintf(dateString, "%04d-%02d-%02d", dt.year(), dt.month(), dt.day());
   sprintf(timeString, "%02d:%02d:%02d", dt.hour(), dt.minute(), dt.second());
   
   Serial.print(dateString);
   Serial.print(" ");
   Serial.print(timeString);
 }
 
 void setup() {
   // Initialize serial communication
   Serial.begin(115200);
   while (!Serial) delay(100); // Wait for serial port to connect
   Serial.println("Outdoor Module - Environmental Monitoring System");
 
   // Initialize I2C
   Wire.begin(SDA_PIN, SCL_PIN);
   
   // Initialize RTC
   if (!rtc.begin()) {
     Serial.println("Couldn't find RTC! Check wiring.");
     while (1);
   }
   
   // Set RTC time if it was lost (battery disconnected or power failure)
   // Comment out after the first upload to keep the RTC time
   if (rtc.lostPower()) {
     Serial.println("RTC lost power, setting the time to compile time!");
     // Set the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
   
   Serial.println("RTC initialized!");
   
   // Initialize BMP280
   if (!bmp.begin(0x77)) {
     Serial.println("Could not find a valid BMP280 sensor, check wiring!");
     while (1);
   }
   Serial.println("BMP280 sensor initialized!");
   
   // Configure BMP280 settings
   bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode
                   Adafruit_BMP280::SAMPLING_X2,      // Temp. oversampling
                   Adafruit_BMP280::SAMPLING_X16,     // Pressure oversampling
                   Adafruit_BMP280::FILTER_X16,       // Filtering
                   Adafruit_BMP280::STANDBY_MS_500);  // Standby time
   
   // Initialize SCD30
   if (!scd30.begin()) {
     Serial.println("Could not find SCD30 sensor, check wiring!");
     while (1);
   }
   Serial.println("SCD30 sensor initialized!");
   
   // Configure SCD30 settings
   scd30.setMeasurementInterval(2); // Interval of measurement is [2, 1800] seconds
   scd30.setAutoSelfCalibration(true);
   
   // Start first measurement on SCD30 immediately
   scd30.beginMeasuring();
   
   // Initialize BH1750
   if (!lightMeter.begin()) {
     Serial.println("Could not find BH1750 sensor, check wiring!");
     while (1);
   }
   Serial.println("BH1750 sensor initialized!");
   
   Serial.println("All sensors ready!");
   Serial.println("Taking measurements every 60 seconds...");
   Serial.println();
 }
 
 void loop() {
   unsigned long currentTime = millis();
   
   // Check if it's time to take a new measurement
   if (currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL || lastMeasurementTime == 0) {
     // Start SCD30 measurement cycle early to ensure data will be ready
     // This will be done 5 seconds before the actual reading
     if (lastMeasurementTime != 0 && 
         currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL - 5000 && // Wait for 5 seconds if scd30 is ready
         currentTime - lastMeasurementTime < MEASUREMENT_INTERVAL) {
       scd30.beginMeasuring();
     }
     
     // Now that it's time for the full measurement, update the timestamp
     if (currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL || lastMeasurementTime == 0) {
       lastMeasurementTime = currentTime;
     
       Serial.println("==== New Measurement ====");
       
       // Get the current date and time from RTC
       DateTime now = rtc.now();
       
       // Print timestamp
       Serial.print("Timestamp: ");
       printDateTime(now);
       Serial.println();
     
       // Wait for SCD30 data to be available
       unsigned long waitStartTime = millis();
       while (!scd30.dataAvailable()) {
         // Add timeout in case sensor is disconnected
         if (millis() - waitStartTime > 5000) {  // 5 second timeout
           Serial.println("Timeout waiting for SCD30 data!");
           break;
         }
         delay(100);
       }
       
       // Start measure only when SCD30 is available to get all measures synchronized
       if (scd30.dataAvailable()) {
         
         // Read BMP280 data
         float bmp_temp = bmp.readTemperature();
         float pressure = bmp.readPressure() / 100.0F; // Convert Pa to hPa
         float altitude = bmp.readAltitude(1013.25); // Standard pressure at sea level level (1013.25 hPa)
 
         // Read SCD30 data
         float scd30_co2 = scd30.getCO2();
         float scd30_temp = scd30.getTemperature();
         float scd30_humidity = scd30.getHumidity();
         
         // Read BH1750 data
         float lux = lightMeter.readLightLevel();
 
         // Print data for BMP280
         Serial.println("--- BMP280 Data ---");
         Serial.print("Temperature: ");
         Serial.print(bmp_temp);
         Serial.println(" °C");
         Serial.print("Pressure: ");
         Serial.print(pressure);
         Serial.println(" hPa");
         Serial.print("Approx. Altitude: ");
         Serial.print(altitude);
         Serial.println(" m");
         
         // Print data for SCD30
         Serial.println("--- SCD30 Data ---");
         Serial.print("CO2: ");
         Serial.print(scd30_co2);
         Serial.println(" ppm");
         Serial.print("Temperature: ");
         Serial.print(scd30_temp);
         Serial.println(" °C");
         Serial.print("Humidity: ");
         Serial.print(scd30_humidity);
         Serial.println(" %");
         
         // Print data for BH1750
         Serial.println("--- BH1750 Data ---");
         Serial.print("Light: ");
         Serial.print(lux);
         Serial.println(" lux");
       
         // Get RTC temperature (DS3231 has an internal temperature sensor)
         Serial.println("--- RTC Data ---");
         Serial.print("RTC Temperature: ");
         Serial.print(rtc.getTemperature());
         Serial.println(" °C");
 
       } else {
         Serial.println("Failed to get SCD30 data after timeout");
       }
       
       Serial.println(); // Add empty line for better readability
     }
   }
   
   // Small delay to prevent consuming too much CPU
   delay(100);
 }
 