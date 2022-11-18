//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//LED blinking speed control using FreeRTOS

#include<stdlib.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t buffer_length = 20;
static const int     led_pin = LED_BUILTIN;
static int           led_blink_delay = 500;

void toggle_led(void *parameter){
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_blink_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_blink_delay / portTICK_PERIOD_MS);
  }
}

void takeInputfromSerialM(void *parameter){
  char value;
  char buf[buffer_length];
  uint8_t index = 0;

  memset(buf, 0, sizeof(buf));

  while(1){
    if(Serial.available() > 0){
        value = Serial.read();

        if(value == '\n'){
          led_blink_delay = atoi(buf);
          Serial.print("Updated LED delay is ");
          Serial.println(led_blink_delay);
          memset(buf, 0, sizeof(buf));
          index = 0;
        }else{
          if(index < buffer_length - 1){
            buf[index] = value;
            index++;
          }
        }
    }
  }
}

void setup() {
 pinMode(led_pin, OUTPUT);

 Serial.begin(115200);
 vTaskDelay(1000 / portTICK_PERIOD_MS);
 Serial.println("Enter delay value in milliseconds to change the LED blink speed.");

 xTaskCreatePinnedToCore(
                        toggle_led,     // function to be called
                        "Toggle LED",   // task's name
                        1024,           // stack size (words)
                        NULL,           // parameter
                        1,              //task priority
                        NULL,           // task handle
                        app_cpu         // run on one core 
 );

  xTaskCreatePinnedToCore(
                        takeInputfromSerialM,     // function to be called
                        "Serial read input",   // task's name
                        1024,           // stack size (words)
                        NULL,           // parameter
                        1,              //task priority
                        NULL,           // task handle
                        app_cpu         // run on one core 
 );

 vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
