//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//led brightness control by using adc
#define LED    2
int brightness = 0;
int brightAmount = 1;

void setup() {

pinMode(LED, OUTPUT);
}

void loop() {

analogWrite(LED, brightness);
brightness = brightness + brightAmount;

if(brightness <= 0 || brightness >= 255)
  brightAmount = -brightAmount;

delay(10);

}
