/*author:Pranathi
date:01/09/25
topic:inbuilt touch sensor testing in esp32 using rtos*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static const int led=2;
static volatile uint8_t flag=0;

void readSensor(void *parameters){
  while(1){
    if(touchRead(T0)<40){
      flag=1;
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void glowLed(void *parameters){
  while(1){
    if(flag==1){
      digitalWrite(led,HIGH);
      flag=0;
    }else{
      digitalWrite(led,LOW);
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("FreeRtos prgram to demonstrate touch sensor");
  xTaskCreatePinnedToCore(
    readSensor,
    "Reading from touch sensor",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );
  xTaskCreatePinnedToCore(
    glowLed,
    "Glowing led",
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
