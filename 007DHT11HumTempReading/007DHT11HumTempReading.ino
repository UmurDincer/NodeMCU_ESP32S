//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//Humidity and temperature reading by using DHT11

#include "DHT.h"

#define DHT_PIN   23
#define SENSOR_TYPE  DHT11

DHT sensor(DHT_PIN, SENSOR_TYPE);

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  float temperature = sensor.readTemperature();
  float humidity    = sensor.readHumidity();

  if(isnan(temperature) || isnan(humidity)) Serial.print("DHT sensor cannot be read!\n");
  else{
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C\n");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%\n");
  }
  delay(2000);
}
