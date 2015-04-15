/*DC driver code
-PWM controls the speed/torque of the motor
-25 appears to be the "optimal" value, BUT may need to be adjusted
due to extra weight added

IMPORTANT: TESTED WITH AN ARDUINO UNO, PIN ASSIGNMENTS MAY VARY DEPENDING
ON BOARD
*/

//pin assignments
int pinD= 13;  //direction
int pinPWM= 10;//PWM pin

void setup(){
   pinMode(pinD, OUTPUT);
   pinMode(pinPWM, OUTPUT);
}

void loop(){
   digitalWrite(pinD, HIGH);
   /*for (int PWM= 0; PWM<=255; PWM+=5){
     analogWrite(pinPWM, PWM);
     delay(500);
   }*/
   
   analogWrite(pinPWM, 25);
   delay(100);   //short, brief burst
   analogWrite(pinPWM, 0);
   delay(250);
}

