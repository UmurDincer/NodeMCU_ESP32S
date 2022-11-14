//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//Humidity and temperature reading by using DHT11
//https://github.com/UmurDincer/NodeMCU_ESP32S

#define BLYNK_TEMPLATE_ID           "TMPLMRJjO0K8"
#define BLYNK_DEVICE_NAME           "ESP32"
#define BLYNK_AUTH_TOKEN            "Lq0BtBkDv8EAysr91Ek7Of5LeEOZgcne"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TP-Link_8A42";
char pass[] = "96295267";

#define DHT_PIN 23
#define SENSOR_TYPE DHT11

DHT sensor(DHT_PIN, SENSOR_TYPE);
BlynkTimer timer;

void sensorVirtualPort()
{  
  float temperature = sensor.readTemperature();
  float humidity    = sensor.readHumidity();

  if(isnan(temperature) || isnan(humidity)) Serial.print("DHT sensor cannot be read!\n");
  else{
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C\n");
    Blynk.virtualWrite(V5, humidity);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%\n");
    Blynk.virtualWrite(V6, temperature);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  sensor.begin();
  timer.setInterval(20000L, sensorVirtualPort);
}

void loop() {
 Blynk.run();
 timer.run();

}
