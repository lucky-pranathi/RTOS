/*author:Pranathi
date:25/08/25
topic:task scheduling*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

const char msg[]="Ballari institute";

static TaskHandle_t task1=NULL;
static TaskHandle_t task2=NULL;

void startTask1(void *parameter){
  int msglen=strlen(msg);
  while(1){
    Serial.println();
    for(int i=0;i<msglen;i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void startTask2(void *parameter){
  while(1){
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(300);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("---FreeRTOS task demo---");
  Serial.print("setup and loop task running on core");
  Serial.print(xPortGetCoreID());
  Serial.print("with priority");
  Serial.println(uxTaskPriorityGet(NULL));

  xTaskCreatePinnedToCore(
    startTask1,
    "Task1",
    2048,
    NULL,
    1,
    &task1,
    app_cpu
  );
  xTaskCreatePinnedToCore(
    startTask2,
    "Task 2",
    2048,
    NULL,
    2,
    &task2,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<3;i++){
    vTaskSuspend(task2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  if(task1!=NULL){
    vTaskDelete(task1);
    task1=NULL;
  }
}
