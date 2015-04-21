#include "simpletools.h"

/*
  Keypad diagram:
  
  1 2 3 A
  4 5 6 B
  7 8 9 C
  * 0 # D
  
  0 - reset signal
  A - In House
  B - Outside
  C - Untreated
  D - Treated
  * - Custom Label
  # - Print  
*/

// defining signal values for LCD to turn on and clear its screen
const int ON = 22;
const int CLR = 12;

// variable to hold address for LCD connection
serial *lcd;

// Keypad pin numbers corresponding to each row/column.
const int COL1 = 3;
const int COL2 = 2;
const int COL3 = 1;
const int COL4 = 0;
const int ROW1 = 7;
const int ROW2 = 6;
const int ROW3 = 5;
const int ROW4 = 4;

struct itemInfo{
  char *itemName;
  int inHouseDays;
  int outsideDays;
  int treatDays;
  int untreatDays;
};

struct itemInfo itemArray[100];

void initializeItemArray(){
  for (int i = 0; i < 100; i++){
    strcpy(itemArray[i].itemName, "");
    itemArray[i].inHouseDays = -1;
    itemArray[i].outsideDays = -1;
    itemArray[i].treatDays = -1;
    itemArray[i].untreatDays = -1;
  }    
}

void initializeLCD(){
  // open LCD connection, stores memory address in lcd
  lcd = serial_open(12, 12, 0, 9600);
  
  // turn on LCD by sending global variable ON
  writeChar(lcd, ON);
  
  // clear LCD screen by sending global variable CLR
  writeChar(lcd, CLR);
}

char getKey(){
  // Returns character corresponding to the button pushed
  // on keypad
  
  // Placeholder
  return 'a';
}

void printToLCD(char stringToPrint[]){
  // clear LCD screen
  writeChar(lcd, CLR);
  
  // print message
  dprint(lcd, stringToPrint);
  
  return;
}  

void getItemCode(int itemCode[2]){
  itemCode[0] = getKey();
  itemCode[1] = getKey();
}

int getLocationFromUser(){
  // Returns 1 for in-house, 0 for outside
  while(1){  
    char keyEntered = getKey();
    
    if (keyEntered == 'a' || keyEntered == 'b'){
      return keyEntered;
    }
    else{
      continue;
    }      
  }
  return -1; 
}  

int main(){
  initializeLCD();
  
  while(1)
  {
    int itemCode[2];
    for (int i = 0; i < 2; i++){
      itemCode[i] == 0;
    }      
    
    getItemCode(itemCode);
    
    int isInHouse = getLocationFromUser();
    
    if (isInHouse == -1){
      // -1 is error code; something went wrong getting location
      continue;
    }          
  }
}
