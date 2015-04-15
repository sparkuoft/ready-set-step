

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

// include the library code:
#include <Wire.h>
#include <Adafruit_sMCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <EEPROM.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

const int TRAFFIC_LIGHT_DELAY = 1500;
const int STEPS_ADDR = 100;
const int RACES_ADDR = 200;

//constants:
int R1= 680;
int VCC= 5;
int stepResistance = 80;//15;//10;//50;
int nostepResistance= 100; //100;//30;//25;//300;
int aMotorSpeed = 30; //35; // pwm value
int bMotorSpeed = 35; //40; // pwm value
int aPulseDuration = 100; // ms
int bPulseDuration = 100; // ms
int FORWARD = LOW; // or whatever
int BACKWARD = HIGH; // or whatever
unsigned long timeout = 1000 * 30; // ms

//variables:
int isStepALeft= 0;
int isStepBLeft= 0;
int isStepARight= 0;
int isStepBRight= 0;
int numStepsA= 0;
int numStepsB= 0;
unsigned long aOnUntil = 0;
unsigned long bOnUntil = 0;
unsigned long totalSteps = 0;
unsigned long totalRaces = 0;
bool shouldRestartImmediately = false;
unsigned long timeOfLastAction = 0;

// b == blue == player 2

//pin assignments:s
//int fiveVolts= 14;
int inputPinALeft = A2;
int inputPinARight = A3;
int inputPinBLeft = A10;
int inputPinBRight = A11;

int aMotorPWM = 13;
int bMotorPWM = 11;
int aMotorDir = 12;
int bMotorDir = 10;

int aStartSwitch = A0;
int bStartSwitch = A8;
int aEndSwitch = A1;
int bEndSwitch = A9;

int green= 26;
int yellow= 24;
int red= 22;

int aWin = 53;
int bWin = 51;

int startButton = 5;

const int numT = 10;
int original[numT] = { 31, 33, 35, 37, 39, 47, 43, 45, 41, 49 };
int secondTry[numT] = { 35, 31, 33, 37, 39, 49, 45, 47, 41, 43 };
int thermometer[numT] = { 33, 35, 31, 37, 39, 45, 49, 43, 41, 47 };

void writeLongEEPROM(int address, unsigned long value)
{
    const byte* p = (const byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(address++, *p++);
}

bool readLongEEPROM(int address, unsigned long& value)
{
    bool success = false;
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++) {
          *p = EEPROM.read(address);
          if (*p != 255) success = true;
          p++;
          address++;
    }
    
    return success;
}

void resetMotors() {
  unsigned long shouldToggle = millis() + 750;
  digitalWrite(yellow, HIGH);
  int state = HIGH;
  
  digitalWrite(bMotorDir, !BACKWARD);
  digitalWrite(aMotorDir, BACKWARD);
  //Serial.println("A MOTOR DIR is LOW");
  
  while (digitalRead(bStartSwitch) == HIGH || digitalRead(aStartSwitch) == HIGH) {
    if (digitalRead(bStartSwitch) == HIGH) {
      //Serial.println("b is HIGH");
      analogWrite(bMotorPWM, bMotorSpeed);
    } else {
      //Serial.println("b is LOW");
      analogWrite(bMotorPWM, 0);
    }
    
    if (digitalRead(aStartSwitch) == HIGH) {
      analogWrite(aMotorPWM, aMotorSpeed);
      //Serial.println("a is HIGH");
    } else {
      analogWrite(aMotorPWM, 0);
      //Serial.println("a is LOW");
    }
    
    if (millis() > shouldToggle) {
      if (state == HIGH) state = LOW;
      else state = HIGH;
      
      shouldToggle = millis() + 750;
      digitalWrite(yellow, state);
    }
    
  }
  
  digitalWrite(yellow, LOW);
  analogWrite(aMotorPWM, 0);
  analogWrite(bMotorPWM, 0);
}

const double joulesPerStep = 175.0;
const char uJoules[] = "J";
const char uKila[] = "kJ";
const char uMega[] = "MJ";

void formatDouble(char* str, double d) {
  int roundDown = d;
  int hundreths = (d - roundDown) * 100;
  
  sprintf(str, "%01d.%02d", roundDown, hundreths);
}

void getJoulesString(char* str) {
  double totalJoules = totalSteps * joulesPerStep;
  const char* units = uJoules;
  
  if (totalJoules > 1000) {
    totalJoules /= 1000;
    units = uKila;
    
    if (totalJoules > 1000) {
      totalJoules /= 1000;
      units = uMega;
    }
  }
  
  char value[10];
  formatDouble(value, totalJoules);

  sprintf(str, "%s %s", value, units);
}

void getStepsString(char* str) {
  sprintf(str, "%lu steps", totalSteps);
}

void getRacesString(char* str) {
  sprintf(str, "%lu races", totalRaces);
}

int type = 0;

char* sparkString1 = "Like what you";
char* sparkString2 = "see?";
char* sparkString3 = "Join our mailing";
char* sparkString4 = "list!";
char* sparkString5 = "Subscribe at:";
char* sparkString6 = "spark.skule.ca";


void updateLCD() {
  type++;
  if (type == 6) type = 0;
  
  char buffer1[20];
  char buffer2[20];
  char* str1 = buffer1;
  char* str2 = buffer2;
  
  switch (type) {
    case 0:
      getJoulesString(str1);
      getStepsString(str2);
      break;
    case 1:
      getRacesString(str1);
      getJoulesString(str2);
      break;
    case 2:
      getStepsString(str1);
      getRacesString(str2);
      break;
    case 3:
      str1 = sparkString1;
      str2 = sparkString2;
      break;
    case 4:
      str1 = sparkString3;
      str2 = sparkString4;
      break;
    case 5:
      str1 = sparkString5;
      str2 = sparkString6;
      break;
  }
  
  lcd.clear();
  
  // (note: line 0 is the first row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(str1);
  
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print(str2);
}

void lcdPrint(const char* str, char color = 0x0) {
  if (color != 0x0) {
    lcd.setBacklight(color);
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  lcd.print(str);
}

void trafficLightSeq(){
   lcdPrint("Ready ...", RED);
   digitalWrite(red, HIGH);
   delay(TRAFFIC_LIGHT_DELAY);
   digitalWrite(red, LOW);
   lcdPrint("Set ...", YELLOW);
   digitalWrite(yellow, HIGH);
   delay(TRAFFIC_LIGHT_DELAY);
   digitalWrite(yellow, LOW);
   lcdPrint("Step!", GREEN);
   digitalWrite(green, HIGH);
   
   return;
}

void startRace() {
  resetMotors();
  trafficLightSeq();
}

void testThermometer() {

    for (int j = 0; j < numT; j++) {

        digitalWrite(thermometer[j], HIGH);
        
        delay(1000);

    }
    
     for (int j = 0; j < numT; j++) {
        digitalWrite(thermometer[j], LOW);
     }

}

void updateThermometer() {
  // THIS MANY STEPS IN A RACE!!
  const int total = 100;
  int maxSteps = max(numStepsA, numStepsB);
  
  for (int i = 0; i < numT; i++) {
    if (maxSteps >= total * (i + 1) / numT) {
      digitalWrite(thermometer[i], HIGH);
    } else {
      digitalWrite(thermometer[i], LOW);
    }
  }
}

void turnOnFullThermometer() {
  for (int i = 0; i < numT; i++) {
    digitalWrite(thermometer[i], HIGH);
  }
}

bool raceLoop() {
  timeOfLastAction = millis();
  
  while (digitalRead(bEndSwitch) == HIGH && digitalRead(aEndSwitch) == HIGH) {
    doMotorStuff();
    updateThermometer();
    
    if (digitalRead(startButton) == HIGH) {
      return true;
    }
    
    unsigned long now = millis();
    
    if (now - timeOfLastAction >= timeout) {
      return false;
    }
  }
  
  analogWrite(aMotorPWM, 0);
  analogWrite(bMotorPWM, 0);
  
  turnOnFullThermometer();
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  
  int pin = 0;
  if (digitalRead(bEndSwitch) == LOW) {
    pin = bWin;
    lcdPrint("Blue wins!", WHITE);
  } else {
    pin = aWin;
    lcdPrint("Red wins!", RED);
  }
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
  }
  
  return false;
}

void initializeVarsFromMemory() {
  
  bool stepsSuccess = readLongEEPROM(STEPS_ADDR, totalSteps);
  if (!stepsSuccess) {
    totalSteps = 0;
    writeLongEEPROM(STEPS_ADDR, totalSteps);
  }
  
  bool racesSuccess = readLongEEPROM(RACES_ADDR, totalRaces);
  if (!racesSuccess) {
    totalRaces = 0;
    writeLongEEPROM(RACES_ADDR, totalRaces);
  }
  
}

void setup(){
  Serial.begin(9600);
  pinMode(inputPinALeft, INPUT);
  pinMode(inputPinBLeft, INPUT);
  pinMode(inputPinARight, INPUT);
  pinMode(inputPinBRight, INPUT);
  //pinMode(fiveVolts, OUTPUT);
  
  pinMode(aMotorPWM, OUTPUT);
  pinMode(bMotorPWM, OUTPUT);
  pinMode(aMotorDir, OUTPUT);
  pinMode(bMotorDir, OUTPUT);
  
  digitalWrite(aMotorDir, HIGH);
  //Serial.println("A MOTOR DIR is HIGH");
  digitalWrite(bMotorDir, HIGH);
  
  analogWrite(aMotorPWM, 0);
  analogWrite(bMotorPWM, 0);
  
  // traffic light setup
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT); 

  //pinMode(13, OUTPUT);
  
  pinMode(aStartSwitch, INPUT_PULLUP);
  pinMode(bStartSwitch, INPUT_PULLUP);
  pinMode(aEndSwitch, INPUT_PULLUP);
  pinMode(bEndSwitch, INPUT_PULLUP);
  
  pinMode(aWin, OUTPUT);
  pinMode(bWin, OUTPUT);
  
  pinMode(startButton, INPUT_PULLUP);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setBacklight(GREEN);
  
  // thermometer pins
  for (int i = 0; i < numT; i++) {
    pinMode(thermometer[i], OUTPUT);
  }
  
  initializeVarsFromMemory();
  
  //turnOnFullThermometer();
  
  //while (true);
}

void turnOnMotorA(int dir) {
  digitalWrite(aMotorDir, dir);
  //Serial.print("A MOTOR DIR is ");
  //Serial.println(dir == HIGH ? "HIGH" : "LOW");
  analogWrite(aMotorPWM, aMotorSpeed);
  aOnUntil = millis() + aPulseDuration;
  //Serial.println("Turn motor A on!");
}

void turnOnMotorB(int dir) {
  digitalWrite(bMotorDir, !dir);
  analogWrite(bMotorPWM, bMotorSpeed);
  bOnUntil = millis() + bPulseDuration;
  //Serial.println("Turn motor B on!");
}

void turnOffMotorsIfNecessary() {
  if (millis() > aOnUntil) {
    analogWrite(aMotorPWM, 0);
    Serial.println("Turn motor A off!");
    Serial.print("Now is: ");
    Serial.println(millis());
    Serial.print("aOnUntil: ");
    Serial.println(aOnUntil);
  }
  
  if (millis() > bOnUntil) {
    analogWrite(bMotorPWM, 0);
    Serial.println("Turn motor B off!");
    Serial.print("Now is: ");
    Serial.println(millis());
    Serial.print("bOnUntil: ");
    Serial.println(bOnUntil);
    //Serial.println("Turn motor B off!");
  }
}

int mostRecentStep = 0;
  
void doMotorStuff(){
  
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
  //Serial.print("Left: ");
  //Serial.println(RALeft);
  float RBLeft= (R1/VoBLeft)*5 -R1;
  float RARight= (R1/VoARight)*5 -R1;
  //Serial.print("Right: ");
  
  float RBRight= (R1/VoBRight)*5 -R1;
  
//  switch (mostRecentStep) {
//    case 0:
//      Serial.print("RALeft: ");
//      Serial.println(RALeft);
//      break;
//    case 1:
//      Serial.print("RARight: ");
//      Serial.println(RARight);
//      break;
//    case 2:
//      Serial.print("RBLeft: ");
//      Serial.println(RBLeft);
//      break;
//    case 3:
//      Serial.print("RBRight: ");
//      Serial.println(RBRight);
//      break;
//  }
  
  //Serial.print("Resistance is ");
  //Serial.println(resistance);
  
  //Serial.print("Number of steps: ");
  //Serial.println(numSteps);
  
  //Player A
  if (RALeft>=nostepResistance)
  {
    isStepALeft = 1;
    //digitalWrite(13, LOW);
  }
    
  if (isStepALeft == 1 && RALeft <= stepResistance){
   isStepALeft = 0;
   numStepsA++;  
   totalSteps++;
   writeLongEEPROM(STEPS_ADDR, totalSteps);
   //Serial.print("SHOULD BE SETTING MOTOR DIR to ");
   //Serial.println(FORWARD == HIGH ? "HIGH" : "LOW");
   Serial.print("Turning motor a on, time is: ");
   Serial.println(millis());
   turnOnMotorA(FORWARD);
   timeOfLastAction = millis();
   mostRecentStep = 0;
   //digitalWrite(13, HIGH);
  }
  
  if (RARight>=nostepResistance)
    isStepARight = 1;
    
  if (isStepARight == 1 && RARight <= stepResistance){
   isStepARight = 0;
   numStepsA++;  
   totalSteps++;
   writeLongEEPROM(STEPS_ADDR, totalSteps);
   //Serial.print("SHOULD BE SETTING MOTOR DIR to ");
   //Serial.println(FORWARD == HIGH ? "HIGH" : "LOW");
   Serial.print("Turning motor a on, time is: ");
   Serial.println(millis());
   turnOnMotorA(FORWARD);
   timeOfLastAction = millis();
   mostRecentStep = 1;
  }
    
  //Player B
  if (RBLeft>=nostepResistance)
    isStepBLeft = 1;
  
  if (isStepBLeft == 1 && RBLeft <= stepResistance){
    isStepBLeft = 0; 
    numStepsB++;
    totalSteps++;
    writeLongEEPROM(STEPS_ADDR, totalSteps);
    Serial.print("Turning motor b on, time is: ");
    Serial.println(millis());
    turnOnMotorB(FORWARD);
    timeOfLastAction = millis();
    mostRecentStep = 2;
  }
  
  if (RBRight>=nostepResistance)
    isStepBRight = 1;
  
  if (isStepBRight == 1 && RBRight <= stepResistance){
    isStepBRight = 0; 
    numStepsB++;
    totalSteps++;
    writeLongEEPROM(STEPS_ADDR, totalSteps);
    Serial.print("Turning motor b on, time is: ");
    Serial.println(millis());
    turnOnMotorB(FORWARD);
    timeOfLastAction = millis();
    mostRecentStep = 3;
  }
  
  turnOffMotorsIfNecessary();
}

void resetThermometer() {
  for (int i = 0; i < numT; i++) {
    digitalWrite(thermometer[i], LOW);
  }
}

void resetVars() {
  isStepALeft= 0;
  isStepBLeft= 0;
  isStepARight= 0;
  isStepBRight= 0;
  numStepsA= 0;
  numStepsB= 0;
  aOnUntil = 0;
  bOnUntil = 0;
}

void resetEverything() {
  //Serial.println("Resetting vars ...");
  resetVars();
  
  //Serial.println("Resetting thermometer...");
  resetThermometer();
  
  digitalWrite(aWin, LOW);
  digitalWrite(bWin, LOW);
  
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  
  resetMotors();
}

int lightNumber = 0;

void updateLightShow() {
  resetThermometer();
  
  if (lightNumber < 10) {
    for (int i = 0; i < lightNumber && i < numT; i++) {
      digitalWrite(thermometer[i], HIGH);
    }
  } else {
    for (int i = 19; i > lightNumber; i--) {
      digitalWrite(thermometer[i - 10], HIGH);
    }
  }
    
  lightNumber++;
  if (lightNumber > 19) lightNumber = 0;
}
    

void waitForStartButtonAndShowStats() {
  lightNumber = 0;
  unsigned long nextUpdate = millis() + 3000;
  unsigned long nextLightUpdate = millis() + 150;
  updateLightShow();
  updateLCD();
  
  while (digitalRead(startButton) == LOW) {
    if (millis() > nextUpdate) {
      updateLCD();
      nextUpdate = millis() + 3000;
    }
    if (millis() > nextLightUpdate) {
      updateLightShow();
      nextLightUpdate = millis() + 150;
    }
  }
  
  resetThermometer();
}

//void loop() {
//  for (int i = 0; i < 100; i++) {
//    digitalWrite(aMotorDir, FORWARD);
//    digitalWrite(bMotorDir, FORWARD);
//    
//    analogWrite(aMotorPWM, i);
//    analogWrite(bMotorPWM, i);
//    
//    Serial.println(i);
//    
//    delay(1000);
//  }
//}

void loop() {
  lcdPrint("Resetting...", YELLOW);
  Serial.println("Resetting ...");
  resetEverything();
  if (!shouldRestartImmediately) waitForStartButtonAndShowStats();
  startRace();
  shouldRestartImmediately = raceLoop();
  totalRaces++;
  writeLongEEPROM(RACES_ADDR, totalRaces);
  //testThermometer();
}

//void loop() {
//  //digitalWrite(35, HIGH);
//  //digitalWrite(37, HIGH);
//  //digitalWrite(33, HIGH); <- Broken
//  //Serial.println("DID A THING!");
//  
//  testThermometer();
//}

