#define pin 2

const int pwmFreq = 5000;
const int pwmResolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(pin,pwmFreq,pwmResolution);
}

void loop() {
for(auto i=0;i<255;i=i+10){
ledcWrite(pin,i);
Serial.print(i);
Serial.println("|");
delay(100);
}
delay(1000);
for(auto i=255;i>0;i=i-10){
ledcWrite(pin,i);
Serial.print(i);
Serial.println("|");
delay(100);
}
delay(1000);
}
