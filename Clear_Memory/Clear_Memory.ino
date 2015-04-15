
#include <EEPROM.h>


const int STEPS_ADDR = 100;
const int RACES_ADDR = 200;


void writeLongEEPROM(int address, unsigned long value)
{
    const byte* p = (const byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(address++, *p++);
}


void setup(){
  writeLongEEPROM(STEPS_ADDR, 88356);
  writeLongEEPROM(RACES_ADDR, 1011);
}

void loop() {
  // nothing to do
}

