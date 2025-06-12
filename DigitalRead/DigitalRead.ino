#define IR_Sensor_PIN 15

void setup() {
  Serial.begin(115200);
  pinMode(IR_Sensor_PIN,INPUT);
}

void loop() {
 int ir_state = digitalRead(IR_Sensor_PIN);
 if(ir_state == 1){
  Serial.print(ir_state);
  Serial.println(" | Detektujem nesto");
 } else {
  Serial.print(ir_state);
  Serial.println("  | Nista ne detektujem");
 }
 delay(3000);

}
