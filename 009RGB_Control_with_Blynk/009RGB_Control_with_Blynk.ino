//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//RGB control using Blynk
//https://github.com/UmurDincer/NodeMCU_ESP32S
#define BLYNK_TEMPLATE_ID "TMPLMRJjO0K8"
#define BLYNK_DEVICE_NAME "RGB Control"
#define BLYNK_AUTH_TOKEN "OtFwVuFTWikSTe2vf3in0vItyfV_iJpm"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TP-Link_8A42";
char pass[] = "96295267";

#define red   25
#define green 26
#define blue  27

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);

}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V7)
{
  int red_value = param.asInt();

  analogWrite(red, red_value);
}

BLYNK_WRITE(V8)
{
  int green_value = param.asInt();

  analogWrite(green, green_value);
}

BLYNK_WRITE(V9)
{
  int blue_value = param.asInt();

  analogWrite(blue, blue_value);
}