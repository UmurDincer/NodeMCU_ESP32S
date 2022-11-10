//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//long press detection and led blink
#define BUTTON 21
#define LED    2
#define LONG_PRESS 1000
int lastState = LOW;
int currentState;
uint32_t pressedTime = 0;
uint32_t releasedTime = 0;
uint32_t duration = 0; 

int ledState = HIGH;
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentState = digitalRead(BUTTON);

  if(lastState == HIGH && currentState == LOW)
    pressedTime = millis();
  else if(lastState == LOW && currentState == HIGH)
    releasedTime = millis();

  duration = releasedTime - pressedTime;

  if(duration < LONG_PRESS){
    Serial.println("Standby");
    for(int i = 0; i < 10; i++){
      ledState = !ledState;
      digitalWrite(LED, ledState);
      delay(500);
    }
  }
  else{
    Serial.println("Long Press");
    for(int i = 0; i < 10; i++){
      ledState = !ledState;
      digitalWrite(LED, ledState);
      delay(100);
    }
  }

  digitalWrite(LED, LOW);
  delay(1000);
  releasedTime = 0;
  pressedTime = 0;
  lastState = currentState;

}
