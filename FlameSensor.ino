void setup(){
  Serial.begin(9600);
  pinMode(A0,INPUT);
  
}

void loop(){
  long int flame;
  flame = analogRead(A0);
  
  if(flame == 1024){
    Serial.println("No fire");
  }
  else{
    Serial.println("Fire!");
  }
  delay(1000);
}
