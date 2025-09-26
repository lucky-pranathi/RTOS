/*author:Pranathi
date:01/09/25
topic:touch sensor testing in esp32 using super loop*/
int led=2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(touchRead(T0)<40){
    Serial.println(touchRead(T0));
    digitalWrite(led,HIGH);
  }else{
    digitalWrite(led,LOW);
  }
  delay(200);
}
