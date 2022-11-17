#include <Arduino_GFX_Library.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

Arduino_DataBus *bus = new Arduino_ESP32SPI(33 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, 12 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, -1 /* RST */, 1 /* rotation */);

#define WIFI_SSID "TP-Link_8A42"
#define WIFI_PASSWORD "96295267"
#define API_KEY "AIzaSyAX69YDrBXmBky7o01C5ysS1XmeApLjIgY"
#define DATABASE_URL "https://esp32-bc5c6-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
int read_data;
int count = 0;
bool signupSuccess = false;

void setup(void)
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    
    while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);

    gfx->begin();
    gfx->fillScreen(PURPLE);
    gfx->setCursor(50, 80);
    gfx->setTextColor(BLACK);
    gfx->setTextSize(5);
    gfx->println("Hello World!");

    delay(5000); 

    Serial.println();
    Serial.print("Connected to... ");
    Serial.println(WiFi.localIP());
    Serial.println();
    
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", "")) {

      Serial.println("ok");
      signupSuccess = true;
  }
    else {
      Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop()
{   

    if (Firebase.ready() && signupSuccess && (millis() - sendDataPrevMillis > 8000 || sendDataPrevMillis == 0)) 
    {
      sendDataPrevMillis = millis();
      if (Firebase.RTDB.getInt(&fbdo, "/test/int"))
      { 
       if (fbdo.dataType() == "int")
       {
          read_data = fbdo.intData();
          Serial.print("Data received: ");
          Serial.println(read_data); 
          gfx->setCursor(220, 150);
          gfx->setTextColor(BLACK);
          gfx->setTextSize(4);
          gfx->println(read_data);
          delay(200);
        }
      }
      else
      {
          Serial.println(fbdo.errorReason()); 
      }

      delay(500);
      gfx->fillRect(0, 0, 480, 320, WHITE);
      if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      count++;
      delay(100);
      }

}
