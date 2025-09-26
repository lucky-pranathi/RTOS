/*author:Pranathi
date:07/09/25
topic:mutex example*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static int shared_var=0;
static SemaphoreHandle_t mutex;

void IncTask(void *parameters){
  int locVar;
  while(1){
    if(xSemaphoreTake(mutex,0)==pdTRUE){
      locVar=shared_var;
      locVar++;
      vTaskDelay(random(100,500)/portTICK_PERIOD_MS);
      shared_var=locVar;

      //return the mutex
      xSemaphoreGive(mutex);

      Serial.println(shared_var);
    }else{
      //Do some another work
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  //Create mutex
  mutex=xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(IncTask,"Task A",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(IncTask,"Task B",1024,NULL,1,NULL,app_cpu);
  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
