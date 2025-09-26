/*author:Pranathi
date:26/08/25
topic:creating tasks 2*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif
static const int led=2;
void toggleLED(void *parameter){
  while(1){
    digitalWrite(led,HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void toggleLED2(void *parameter){
  while(1){
    digitalWrite(led,HIGH);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  xTaskCreatePinnedToCore(
    toggleLED,
    "Toggle Led",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);
    xTaskCreatePinnedToCore(
    toggleLED2,
    "Toggle Led 2",
    1024,
    NULL,
    2,
    NULL,
    app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:

}
