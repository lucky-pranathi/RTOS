/*author:Pranathi
date:26/08/25
topic:task scheduling for led blink and serial read*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static const int led=2;
static int led_delay=500;

void toggleLed(void *parameter){
  while(1){
  digitalWrite(led,HIGH);
  vTaskDelay(led_delay / portTICK_PERIOD_MS);
  digitalWrite(led,LOW);
  vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}
void read(void *parameter){
  while(1){
    if(Serial.available()>0){
      String led_delay1=Serial.readStringUntil('\n');
      led_delay=led_delay1.toInt();
      Serial.print("Delay updated to: ");
      Serial.println(led_delay);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  Serial.println("Initial setup done");
  xTaskCreatePinnedToCore(
    toggleLed,
    "Led Toggling",
    1024,
    NULL,
    3,
    NULL,
    app_cpu
  );
  xTaskCreatePinnedToCore(
    read,
    "Serial input",
    2048,
    NULL,
    2,
    NULL,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
