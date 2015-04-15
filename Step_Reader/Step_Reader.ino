/*Pulse generator for pedal 

A pulse is generated whenever the person lands on the step- this is caused
by a drastic decrease in resistance

IMPORTANT(1): modified code from "Multimeter.ino"
IMPORTANT(2): SANDRO DO NOT MODIFY WITHOUT CONSULTING ME, ELSE BUBBLE TEA!
    Sandro says: Hehehehe >:) http://www.reactiongifs.com/r/y23Q27K.gif

Circuit setup: a voltage divider 
Vcc--R2--Vo--680--GND

IMPORTANT(3): TESTED WITH AN ARDUINO UNO, PIN ASSIGNMENTS MAY VARY DEPENDING
ON BOARD
*/

//constants:
int R1= 680;
int VCC= 5;
int stepResistance = 30;
int nostepResistance= 100;

//variables:
int isStep= 0;
int numSteps= 0;

//pin assignments:
int led= 7;
int inputPin= A0;

void setup(){
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(led, OUTPUT);
}

void loop(){
  
  //------DEBUGGING MESSAGES----------
  float inputReading=analogRead(inputPin);
  float Vo= inputReading/1024*5;
  //Serial.print("Input voltage is ");
  //Serial.println(Vo);
  //Serial.println();
  
  float resistance= (R1/Vo)*5 -R1;
  Serial.print("Resistance is ");
  Serial.println(resistance);
  
  //Serial.print("Number of steps: ");
  //Serial.println(numSteps);
  
  if (resistance>= nostepResistance)
    isStep = 1;
  
  if (isStep == 1 && resistance <= stepResistance){
    isStep = 0;
    digitalWrite(led, HIGH); 
    numSteps++;
  } else {
    digitalWrite(led, LOW); 
  }
  
  delay(50);
}
