/*author:Pranathi
date:25/08/25
topic:creating tasks*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif
static const int led=2;
void toggleLED(void *parameter){
  while(1){
    digitalWrite(led,HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
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
}

void loop() {
  // put your main code here, to run repeatedly:

}
