//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//RFID reading by using PN532

#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

#elif 0
#include <PN532_HSU.h>
#include <PN532.h>
PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

#else*/
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
#endif

volatile bool connected = false;

void setup() {
Serial.begin(115200);

}

void loop() {
bool success;
uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;

while(!connected) connected = init_pn532();

success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

if(success){
  Serial.print("uid = ");
  for(int i = 0; i < uidLength; i++){
    Serial.print(" 0x"); 
    Serial.print(uid[i], HEX);
  }
  Serial.print("\n");
  delay(500);
  connected = init_pn532();
}

}

bool init_pn532(void)
{
  uint32_t versionControl;
  nfc.begin();
  versionControl = nfc.getFirmwareVersion();

  if(!versionControl){
    Serial.print("Firmware version cannot be found! Invalid version!\n");
    return false;
  }

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.print("PN532 initialized successfully!\n");
  
  return true;
}
