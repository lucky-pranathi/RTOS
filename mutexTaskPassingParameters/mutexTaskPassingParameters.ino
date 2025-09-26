/*author:Pranathi
date:11/09/25
topic:parameter passing to task*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static const int led=2;
// Globals
static SemaphoreHandle_t mutex;

void ledBlink(void *parameters){
  int num=*(int *)parameters;
  // Release the mutex so that the creating function can finish
  xSemaphoreGive(mutex);

  Serial.print("Received:");
  Serial.println(num);
  while(1){
    digitalWrite(led,HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(num/portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  int delay_arg;
  Serial.begin(115200);
  Serial.println("FreeRTOS mutex challenge");
  Serial.println("Enter a number for delay (milliseconds)");
  //wait for serial input
  while(Serial.available()<=0);
  delay_arg=Serial.parseInt();
  Serial.print("sending:");
  Serial.println(delay_arg);

  // Create mutex before starting tasks
  mutex = xSemaphoreCreateMutex();

  // Take the mutex
  xSemaphoreTake(mutex, portMAX_DELAY);

  xTaskCreatePinnedToCore(ledBlink,"Blinking led",1024,(void *)&delay_arg,1,NULL,app_cpu);
  
  // Do nothing until mutex has been returned (maximum delay)
  xSemaphoreTake(mutex, portMAX_DELAY);
  
  Serial.println("Done !");

}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000 / portTICK_PERIOD_MS);

}
