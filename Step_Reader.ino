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
int stepResistance = 50;
int nostepResistance= 300;

//variables:
int isStepALeft= 0;
int isStepBLeft= 0;
int isStepARight= 0;
int isStepBRight= 0;
int numStepsA= 0;
int numStepsB= 0;

//pin assignments:s
int fiveVolts= 14;
int inputPinALeft = A2;
int inputPinARight = A3;
int inputPinBLeft = A10;
int inputPinBRight = A11;

void setup(){
  //Serial.begin(9600);
  pinMode(inputPinALeft, INPUT);
  pinMode(inputPinBLeft, INPUT);
  pinMode(inputPinARight, INPUT);
  pinMode(inputPinBRight, INPUT);
  pinMode(fiveVolts, OUTPUT);
}

void loop(){
  
  //------DEBUGGING MESSAGES----------
  float ALeft=analogRead(inputPinALeft);
  float BLeft=analogRead(inputPinBLeft);
  float ARight=analogRead(inputPinARight);
  float BRight=analogRead(inputPinBRight);
  float VoALeft= ALeft/1024*5;
  float VoBLeft= BLeft/1024*5;
  float VoARight= ARight/1024*5;
  float VoBRight= BRight/1024*5;
  
  //Serial.print("Input voltage is ");
  //Serial.println(Vo);
  //Serial.println();
  
  float RALeft= (R1/VoALeft)*5 -R1;
  float RBLeft= (R1/VoBLeft)*5 -R1;
  float RARight= (R1/VoARight)*5 -R1;
  float RBRight= (R1/VoBRight)*5 -R1;
  
  //Serial.print("Resistance is ");
  //Serial.println(resistance);
  
  //Serial.print("Number of steps: ");
  //Serial.println(numSteps);
  
  //Player A
  if (RALeft>=nostepResistance)
    isStepALeft = 1;
    
  if (isStepALeft == 1 && RALeft <= stepResistance){
   isStepALeft = 0;
   numStepsA++;  
  }
  
  if (RARight>=nostepResistance)
    isStepARight = 1;
    
  if (isStepARight == 1 && RARight <= stepResistance){
   isStepARight = 0;
   numStepsA++;  
  }
    
  //Player B
  if (RBLeft>=nostepResistance)
    isStepBLeft = 1;
  
  if (isStepBLeft == 1 && RBLeft <= stepResistance){
    isStepBLeft = 0; 
    numStepsB++;
  }
  
  if (RBRight>=nostepResistance)
    isStepBRight = 1;
  
  if (isStepBRight == 1 && RBRight <= stepResistance){
    isStepBRight = 0; 
    numStepsB++;
  }
  
  delay(50);
}
