/*author:Pranathi
date:31/08/25
topic:memory allocation2*/
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

static char *msg_ptr=NULL;
static volatile uint8_t msg_flag=0;
static const uint8_t buf_len=255;

void taskA(void *paramter){
  char c;
  char buf[buf_len];
  uint8_t idx=0;
  memset(buf,0,buf_len);
  while(1){
    if(Serial.available()>0){
      c=Serial.read();
      if(idx<buf_len-1){
        buf[idx]=c;
        idx++;
      }
      if(c=='\n'){
        buf[idx-1]='\0';
        if(msg_flag==0){
          msg_ptr=(char*) pvPortMalloc(idx*sizeof(char));
          configASSERT(msg_ptr);
          memcpy(msg_ptr,buf,idx);
          msg_flag=1;
        }
      memset(buf,0,buf_len);
      idx=0;
      }
    }
  }
}
void taskB(void *parameter){
  while(1){
    if(msg_flag==1){
      Serial.println(msg_ptr);
      Serial.print("Heap memory after allocating:");
      Serial.println(xPortGetFreeHeapSize());
      vPortFree(msg_ptr);
      msg_ptr=NULL;
      msg_flag=0;
      Serial.print("Heap memory after freeing:");
      Serial.println(xPortGetFreeHeapSize());
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Memoery allocation code setup");
  xTaskCreatePinnedToCore(
    taskA,
    "Task A",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );
  xTaskCreatePinnedToCore(
    taskB,
    "Task B",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
