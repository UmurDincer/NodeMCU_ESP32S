
//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//lighting up the led by pressing the button
#define BUTTON 21
#define LED    2
int button_state;

void setup() {
pinMode(BUTTON, INPUT_PULLUP);
pinMode(LED, OUTPUT);
}

void loop() {
button_state = digitalRead(BUTTON);

if(button_state == LOW)
  digitalWrite(LED, HIGH);
else
  digitalWrite(LED, LOW);

}
