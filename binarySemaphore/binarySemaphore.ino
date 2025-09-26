/* author:Pranathi A
date: 11/09/2025
topic:binary semaphore*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static const int led=2;
// Globals
static SemaphoreHandle_t bin_sem;

void ledBlink(void *parameters){
  int num=*(int *)parameters;
  // Release the mutex so that the creating function can finish
  xSemaphoreGive(bin_sem);

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
  bin_sem = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(ledBlink,"Blinking led",1024,(void *)&delay_arg,1,NULL,app_cpu);
  
  // Do nothing until mutex has been returned (maximum delay)
  xSemaphoreTake(bin_sem, portMAX_DELAY);
  
  Serial.println("Done !");

}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000 / portTICK_PERIOD_MS);
u
}
