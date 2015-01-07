
const int analogInPin = 0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;       

void setup() {
  Serial.begin(9600); 
}

void loop() {
  // read the analog in value:
   
   sensorValue = analogRead(analogInPin);            
  // map it to the range of the analog out:

  // change the analog out value:


  // print the results to the serial monitor:
  Serial.print("p1" );                       
  Serial.println(sensorValue);
}
