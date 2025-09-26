/*author:Pranathi
date:11/09/25
topic:counting semaphore example*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

typedef struct Message{
  char body[20];
  uint8_t len;
} Message;

static const int num_tasks=5;
static SemaphoreHandle_t semaphore;
static SemaphoreHandle_t mutex;

void printing(void *parameters){
  Message msg=*(Message *)parameters;

  xSemaphoreGive(semaphore);

  xSemaphoreTake(mutex,portMAX_DELAY);
  Serial.print("Message: ");
  Serial.print(msg.body);
  Serial.print(" |len: ");
  Serial.println(msg.len);
  xSemaphoreGive(mutex);

  vTaskDelay(1000/portTICK_PERIOD_MS);
  vTaskDelete(NULL);
}

void setup() {
  // put your setup code here, to run once:
  char task_name[20];
  Message msg;
  char text[20]="This is FREERTOS";

  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("FreeRTOS counting semaphores");

  semaphore=xSemaphoreCreateCounting(num_tasks,0);
  mutex=xSemaphoreCreateMutex();

  strcpy(msg.body,text);
  msg.len=strlen(text);

  for(int i=0;i<num_tasks;i++){
    sprintf(task_name,"Task %i",i);

    xTaskCreatePinnedToCore(printing,task_name,1024,(void *)&msg,1,NULL,app_cpu);
  }
  for(int i=0;i<num_tasks;i++){
    xSemaphoreTake(semaphore,portMAX_DELAY);
  }
  xSemaphoreTake(mutex,portMAX_DELAY);
  Serial.println("All tasks completed !");
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000/portTICK_PERIOD_MS);

}
