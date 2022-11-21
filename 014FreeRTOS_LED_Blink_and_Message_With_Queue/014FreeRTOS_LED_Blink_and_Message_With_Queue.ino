//NodeMCU ESP32S ESP-WROOM-32
//Umur Dinçer
//FreeRTOS led blink delay and message echo with task queue 
//this code is taken from Shawn Hymel (digi-key) for learning purpose
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else 
  static const BaseType_t app_cpu = 1;
#endif

static const uint8_t buf_len = 255;
static const char command[] = "delay";
static const int delay_queue_len = 5;
static const int msg_queue_len = 5;
static const uint8_t blink_max = 100;
static const int led_pin = BUILTIN_LED;

typedef struct{
  char body[20];
  int count;
}message;

static QueueHandle_t delay_queue;
static QueueHandle_t msg_queue;

void command_line_interface(void *parmaeter)
{
  message rcv_msg;
  char c;
  char buf[buf_len];
  uint8_t index = 0;
  uint8_t cmd_len = strlen(command);
  int led_delay;

  memset(buf, 0, sizeof(buf));

  while(1){
    if(xQueueReceive(msg_queue, (void*)&rcv_msg, 0) == pdTRUE){
      Serial.print(rcv_msg.body);
      Serial.print(rcv_msg.count);
    }

    if(Serial.available() > 0){
      c = Serial.read();

      if(index < buf_len - 1){
        buf[index] = c;
        index++;
      }

      if((c == '\n') ||(c == '\r')){
        Serial.print("\r\n");

        if(memcmp(buf, command, cmd_len) == 0){

          char* tail = buf + cmd_len;
          led_delay = atoi(tail);
          led_delay = abs(led_delay);

          if(xQueueSend(delay_queue, (void*)&led_delay, 10) != pdTRUE){
            Serial.println("ERROR: Could not put item on delay queue.");
          }
        }

        memset(buf, 0, buf_len);
        index = 0;
        }
        else{
          Serial.print(c);
        }
    }
  }
}

void blinkLED(void *parameters) {

  message msg;
  int led_delay = 500;
  uint8_t counter = 0;

  pinMode(LED_BUILTIN, OUTPUT);

  while (1) {

    if (xQueueReceive(delay_queue, (void *)&led_delay, 0) == pdTRUE) {

      strcpy(msg.body, "Message received ");
      msg.count = 1;
      xQueueSend(msg_queue, (void *)&msg, 10);
    }

    // Blink
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);

    counter++;
    if (counter >= blink_max) {
      
      strcpy(msg.body, "Blinked: ");
      msg.count = counter;
      xQueueSend(msg_queue, (void *)&msg, 10);

      counter = 0;
    }
  }
}

void setup() {
  
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Solution---");
  Serial.println("Enter the command 'delay xxx' where xxx is your desired ");
  Serial.println("LED blink delay time in milliseconds");

  delay_queue = xQueueCreate(delay_queue_len, sizeof(int));
  msg_queue = xQueueCreate(msg_queue_len, sizeof(message));

  xTaskCreatePinnedToCore(command_line_interface,
                          "command_line_interface",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(blinkLED,
                          "Blink LED",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);
}

void loop() {

}
