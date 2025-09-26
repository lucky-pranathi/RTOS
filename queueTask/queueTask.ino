/*author:Pranathi
date:06/09/25
topic:queue 2*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static const int q_len=5;
static QueueHandle_t q1;
static QueueHandle_t q2;
static volatile int led_delay;
static int led=2;

void taskA(void *parameters){
  char ch[50];   // buffer for queue receive
  while(1){
    // First check if something is received in queue q1
    if(xQueueReceive(q1, (void*)ch, 0) == pdTRUE){
      Serial.print("From q1: ");
      Serial.println(ch);
    } 
    else {
      // Check serial input
      if(Serial.available()){
        String input = Serial.readStringUntil('\n');  // Read full line

        input.trim(); // remove spaces/newline

        // Check if input is number
        bool isNumber = true;
        for (int i = 0; i < input.length(); i++) {
          if (!isDigit(input[i])) {
            isNumber = false;
            break;
          }
        }

        if (isNumber && input.length() > 0) {
          led_delay = input.toInt();
          xQueueSend(q2, (void*)&led_delay, 0);
        } else {
          // If string, just send to q1 or print directly
          strncpy(ch, input.c_str(), sizeof(ch));
          ch[sizeof(ch)-1] = '\0';  // safety null terminate
          xQueueSend(q1, (void*)ch, 0);
        }
      }
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}


void TaskB(void *parameters){
  int led_delay,count=0;
  while(1){
    if(xQueueReceive(q2,(void*)&led_delay,0)==pdTRUE){
      digitalWrite(led,HIGH);
      count++;
      vTaskDelay(led_delay / portTICK_PERIOD_MS);
      digitalWrite(led,LOW);
      vTaskDelay(led_delay / portTICK_PERIOD_MS);
    }
    if(count==100){
      xQueueSend(q2,(void*)&count,0);
      count=0;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  pinMode(led,OUTPUT);
  q1=xQueueCreate(q_len,sizeof(char));
  q2=xQueueCreate(q_len,sizeof(int));
  xTaskCreatePinnedToCore(
    taskA,
    "task A",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );
  xTaskCreatePinnedToCore(
    taskB,
    "task B",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

}

void loop() {
  // put your main code here, to run repeatedly:

}
