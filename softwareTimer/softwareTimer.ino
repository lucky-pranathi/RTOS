/*author:Pranathi
date:12/09/25
topic:software timer*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static TimerHandle_t one_shot=NULL;
static TimerHandle_t auto_load=NULL;

void myCallback(TimerHandle_t timer){
  if((uint32_t)pvTimerGetTimerID(timer)==0){
    Serial.println("One shot timer expired");
  }else if((uint32_t)pvTimerGetTimerID(timer)==1){
    Serial.println("Auto load timer expired.");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("FreeRTOS timer demo...");

  one_shot=xTimerCreate("One shot timer",2000/portTICK_PERIOD_MS,pdFALSE,(void *)0,myCallback);
  auto_load=xTimerCreate("Auto load timer",1000/portTICK_PERIOD_MS,pdTRUE,(void *)1,myCallback);

  if(one_shot == NULL || auto_load==NULL){
    Serial.println("Timer is not created.");
  }else{

    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.println("Starting timer...");
    xTimerStart(one_shot,portMAX_DELAY);
    xTimerStart(auto_load,portMAX_DELAY);
  }
  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
