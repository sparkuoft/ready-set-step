//defining a signal for each side fo the pedal
#define LEFT 1
#define RIGHT 2
#define SeqLen 6

//global variables that need to be added in the main file
boolean player1EE= false;  //a flag used to check if the previous sequences have been met
int player1Seq= 0;

boolean player2EE= false;
int player2Seq= 0;

//stores the previous sequences. has to be initialized to zero
int sequence[SeqLen];
int easterEggSeq[SeqLen]= {LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT};
int curSequence= 0;   //we'll start at array 0

/* check the sequence if it matches */
//easterEggSeq is the array
//seqSize is the size of the array (i.e. how many steps there should be
//to enable the easter egg)
//side is the signal indicating whether it is left or right
//note: has to be non-blocking
boolean isEasterEggEnable(int easterEggSeq[], int seqSize, int *playerCurSeq, int side){
  /*check if the isSequenceSoFar is true (i.e. previous patterns have been met*/
      /*check if the cur sequence matches*/
    if (side == easterEqqSeq[*playerCurSeq]){
      *playerCurSeq++;  
      
      /*check if the entire pattern matches, enables easter egg, resets to */
      if (curSequence == seqLen){
          void easterEgg();
          *playerCurSeq= 0;
      }
      
      return true;
    }
    else{   /*resets */
      *playerCurSeq = 0; 
      return false;
    }
    return false; 
}

void easterEgg(){
   credits();
   return;
} 

#define displayNameDelay 2500

void credits(){
   //print out the execs that did this project
   //LCD
   reverseThermometer();
   lcd.begin(16, 2);
   lcd.setCursor(0,0);
   lcd.print("Spark Design Club Presents...");
   lcd.setCursor(0, 1);
   lcd.print("The last project of 1T4-1T5...");
   
   lcdPrint("Easter egg enabled!", WHITE);
   delay(displayNameDelay);
   printName();
   applyToSpark();
   return;
   
}

void reverseThermometer(){
  for (int index=numT; index>0; index--){
    digitalWrite(thermometer[index], HIGH);
  }
  delay(1000);
  
  for (int index=numT; index>0; index--){
    digitalWrite(thermometer[index], LOW);
  }  
}

void printNames(){
  /*not the most effective, but meh. (I'm looking at you Sandro)*/
  
   /*-- Co-pres --*/
   lcdPrint("Co-presidents:", YELLOW);
   delay(displayNameDelay);
   lcdPrint("Gabrielle Sebaldt", YELLOW);
   delay(displayNameDelay);
   lcdPrint("Evelyn Sham", YELLOW);
   delay(displayNameDelay);
   
   /*-- Electrical Specialists--*/
   lcdPrint("Electrical:", GREEN);
   delay(displayNameDelay);
   lcdPrint("Akshay Parelkar", GREEN);
   delay(displayNameDelay);
   lcdPrint("Sandro Young", GREEN);
   delay(displayNameDelay);
   lcdPrint("Wenyi Yin", GREEN);
   delay(displayNameDelay);
   
   /*-- Mechanical Specialist --*/
   lcdPrint("Mechanical", WHITE);
   delay(displayNameDelay);
   lcdPrint("Alexander Xu", WHITE);
   delay(displayNameDelay);
   lcdPrint("James Chen", WHITE);
   delay(displayNameDelay);
   lcdPrint("Jason Yang", WHITE);
   delay(displayNameDelay);
   
   /*-- Finance Specialist --*/
   lcdPrint("Finance", VIOLET);
   delay(displayNameDelay);
   lcdPrint("Cora Pulnicki", VIOLET);
   delay(displayNameDelay);
   
   /*-- Webmaster & Marketing Specialist --*/
   lcdPrint("Marketing", RED);
   delay(displayNameDelay);
   lcdPrint("Jason Wong", RED);
   delay(displayNameDelay);
   return;  
}

void applyToSpark(){
   lcdPrint("Apply to be an executive!", WHITE);
   delay(displayNameDelay);
   lcdPrint("spark.skule.ca", WHITE);
   delay(displayNameDelay);
   return; 
}

void startUpSequence(){
  
}
  
