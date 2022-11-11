//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//potentiometer led control

#define LED 2
#define POT_ADC 36
int previousValue;

void setup() {
Serial.begin(115200);
pinMode(LED, OUTPUT);

}

void loop() {
int potValue = analogRead(POT_ADC);
int brightness = map(potValue, 0 , 4095, 0, 255);

analogWrite(LED, brightness);

if(brightness != previousValue){
Serial.print("Potentiometer's value = ");
Serial.print(potValue);
Serial.print("\n");
Serial.print("Brightness Scale(0 to 255) = ");
Serial.print(brightness);
Serial.print("\n");
}
previousValue = brightness;
delay(100);
}
