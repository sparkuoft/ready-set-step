/*impromptu multimeter to calculate resistance of pressure step due to 
multimeters running out of battery...lol

testing circuit: a voltage divider 
Vcc--R2--Vo--680--GND
*/

//constants
int R1= 680;
int VCC= 5;
int inputPin= A0;

void setup(){
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
}

void loop(){
  float inputReading=analogRead(inputPin);
  float Vo= inputReading/1024*5;
  Serial.print("Input voltage is ");
  Serial.println(Vo);
  Serial.println();
  
  float resistance= (R1/inputVoltage)*5 -R1;
  Serial.print("Resistance is ");
  Serial.println(resistance);
  delay(500);
}
