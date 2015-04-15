/*Traffic Lights (?)
After much debate, the Spark Team has decided we're sticking with the (British
/European) traffic light convention 

IMPORTANT(1): TESTED WITH AN ARDUINO UNO, PIN ASSIGNMENTS MAY VARY DEPENDING
ON BOARD
IMPORTANT(2): FUNCTIONS MAY NEED TO BE MOVED TO INTEGRATE CODE WITH OTHER CODE


NO PWM NEEDED
*/

//pin assignments, 
int green= 5;
int yellow= 6;
int red= 7;

int SRbutton = 1;   //start-reset (toggle) button 
boolean start= false; 
//boolean isToggled= false;   //checks if the button has been toggled or not

void setup(){
   pinMode(green, OUTPUT);
   pinMode(yellow, OUTPUT);
   pinMode(red, OUTPUT); 
   
   pinMode(SRbutton, INPUT);   //IMPORTANT: do a "pull-up" configuration unless you 
                               //want to burn some Arduinos...lolol                              
}

void loop(){
   //MAKE SURE THAT THE VOLTAGE IS GREATER THAN 3
   int volt= digitalRead(SRbutton);
   
   if (volt == HIGH){
        start= ~start;
   } 
   
   determineState();
}


/*note: has not been tested*/
void determineState(){
  
  if (start){        //start the sequence
     start= !start;  //only want it to run once (YORO)
     yellowFlash();  //indicate that race is about to start
     trafficLightSeq();
     
  } else {          //turn everything off;
     powerOff(); 
  } 
  
  return;
}

void powerOff(){
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW); 
  return;
}

void trafficLightSeq(){
   digitalWrite(green, HIGH);
   delay(750);
   digitalWrite(yellow, HIGH);
   delay(750);
   digitalWrite(red, HIGH);
   delay(750);
 
   digitalWrite(green, LOW);
   digitalWrite(yellow, LOW);
   digitalWrite(red, LOW); 
   
   return;
}

void yellowFlash(){
   for (int i=0; i<3; i++){
   digitalWrite(yellow, HIGH);
   delay(500); 
   digitalWrite(yellow, LOW);
   delay(500);
   }
   
   return;
}

