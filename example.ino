#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

RTC_DATA_ATTR int recordCounter = 0;

typedef struct {
  float Temp;
  float Pressure;
} bmp180Records;

#define maxRecords 5 
#define sleepTime  10 

RTC_DATA_ATTR bmp180Records Records[maxRecords];

Adafruit_BMP085 bmp;

#define LED_PIN   32
#define LED_DELAY 500  
#define I2C_SDA   19   
#define I2C_SCL   22   
void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);

  Wire.begin(I2C_SDA, I2C_SCL);

  bool status = bmp.begin();

  if ( status ) {
    Serial.println("Record new values");
    digitalWrite (LED_PIN, HIGH);
    Records[recordCounter].Temp     = bmp.readTemperature();       // Units °C
    Records[recordCounter].Pressure = bmp.readPressure() / 100.0F; // Units hPa
    recordCounter++;
    delay(LED_DELAY);
    if ( recordCounter == 1 ) {
      Serial.println(String(recordCounter) + " record is stored in RTC memory");
    } else {
      Serial.println(String(recordCounter) + " records are stored in RTC memory");
    }
    

    if (recordCounter >= maxRecords) {
      for (int i = 0; i < maxRecords; i++){
        Serial.println(String(i)+","+String(Records[i].Temp)+","+String(Records[i].Pressure));
      }
      recordCounter = 0;
    }  
  } else {
    Serial.println("Sorry but BMP180 did not respond !");
  }

  esp_sleep_enable_timer_wakeup(sleepTime * 1000000);
  
  esp_deep_sleep_start();
}

void loop() {
}