void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    String data = Serial.readStringUntil('\n');
    int D = data.toInt();
    Serial.println(data);
    if(D == 1){
      digitalWrite(4, LOW);
      Serial.println("OFF");
    }
    else if(D == 2){
      digitalWrite(4, HIGH);
      Serial.println("ON");
    }
    delay(1000);
  }

}
