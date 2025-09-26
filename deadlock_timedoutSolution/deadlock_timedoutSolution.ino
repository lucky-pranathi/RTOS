/*author:Pranathi
date:14/09/25
topic:deadlock timedout solution*/
#if CONST_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static SemaphoreHandle_t mutex1;
static SemaphoreHandle_t mutex2;
static const int mutex_delay=1000/portTICK_PERIOD_MS;

void taskA(void *parameters){
  while(1){
    if(xSemaphoreTake(mutex1,mutex_delay)==pdTRUE){
      Serial.println("Task A takes mutex1");
      vTaskDelay(1/portTICK_PERIOD_MS);

      if(xSemaphoreTake(mutex2,mutex_delay)==pdTRUE){
        Serial.println("Task A takes mutex2");

        Serial.println("TaskA is doing some task");
        vTaskDelay(500/portTICK_PERIOD_MS);
        xSemaphoreGive(mutex2);
      }else{
        Serial.println("Task A timedout waiting for mutex2");
        // xSemaphoreGive(mutex1);
      }
      xSemaphoreGive(mutex1);
    }else{
      Serial.println("Task A timed out waiting for mutex1");
      // xSemaphoreGive(mutex1);
    }

    // xSemaphoreGive(mutex2);
    // xSemaphoreGive(mutex1);

    Serial.println("TaskA going to sleep");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
void taskB(void *parameters){
  while(1){
    if(xSemaphoreTake(mutex2,mutex_delay)==pdTRUE){
      Serial.println("Task B takes mutex2");
      vTaskDelay(1/portTICK_PERIOD_MS);

      if(xSemaphoreTake(mutex1,mutex_delay)==pdTRUE){
        Serial.println("Task B takes mutex1");

        Serial.println("TaskB is doing some task");
        vTaskDelay(500/portTICK_PERIOD_MS);

        xSemaphoreGive(mutex1);
      }else{
        Serial.println("Task B timedout waiting for mutex1");
      }
      xSemaphoreGive(mutex2);
    }else{
      Serial.println("Task B timed out waiting for mutex2");
    }
  
    // xSemaphoreGive(mutex1);
    // xSemaphoreGive(mutex2);

    Serial.println("Task B going to sleep");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("FreeRTOS deadlock solution demo...");
  mutex1=xSemaphoreCreateMutex();
  mutex2=xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(taskA,"Task A",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(taskB,"Task B",1024,NULL,1,NULL,app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:

}
