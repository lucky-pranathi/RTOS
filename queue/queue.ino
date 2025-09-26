/*author:Pranathi
date:01/09/25
topic:queue example*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static const uint8_t msg_q_len=5;
static QueueHandle_t msg_q;

void print(void *parameters){
  int item;
  while(1){
    if(xQueueReceive(msg_q,(void*)&item,0)==pdTRUE){
      // Serial.println(item);
    }
    Serial.println(item);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Initial setup for queue setup");

  msg_q=xQueueCreate(msg_q_len, sizeof(int));

  xTaskCreatePinnedToCore(
    print,
    "Print Message",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

}

void loop() {
  // put your main code here, to run repeatedly:
  static int num=0;
  if(xQueueSend(msg_q,(void*)&num,10)!=pdTRUE){
    Serial.println("Queue full");
  }
  num++;
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
