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
const int ROW1 = 0;
const int ROW2 = 1;
const int ROW3 = 2;
const int ROW4 = 3;
const int COL1 = 4;
const int COL2 = 5;
const int COL3 = 6;
const int COL4 = 7;

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
  // This code can be rewritten more elegantly,
  // but right now I only want it to function.
  
  // Applying low voltage across rows
  low(ROW1);
  low(ROW2);
  low(ROW3);
  low(ROW4);
  low(COL1);
  low(COL2);
  low(COL3);
  low(COL4);
  
  /* Applying high voltage to column 1.
  *  This means, if a button in column 1 is pressed, the row 
  *  corresponding to the button pressed will read high voltage
  *  on input.
  */
  high(COL1);
  
  // Checking rows for input. If there is input, returns char
  // corresponding to the button at the row/column.
  if (input(ROW1) == 1){
    return '1';
  }
  else if (input(ROW2) == 1){
    return '4';
  }
  else if (input(ROW3) == 1){
    return '7';
  }
  else if (input(ROW4) == 1){
    return '*';
  }
  
  // Now polling Column 2
  low(COL1);
  high(COL2);
  
  if (input(ROW1) == 1){
    return '2';
  }
  else if (input(ROW2) == 1){
    return '5';
  }
  else if (input(ROW3) == 1){
    return '8';
  }
  else if (input(ROW4) == 1){
    return '0';
  }
  
  // Column 3
  low(COL2);
  high(COL3);
  
  if (input(ROW1) == 1){
    return '3';
  }
  else if (input(ROW2) == 1){
    return '6';
  }
  else if (input(ROW3) == 1){
    return '9';
  }
  else if (input(ROW4) == 1){
    return '#';
  }
  
  // Column 4
  low(COL3);
  high(COL4);
  
  if (input(ROW1) == 1){
    return 'a';
  }
  else if (input(ROW2) == 1){
    return 'b';
  }
  else if (input(ROW3) == 1){
    return 'c';
  }
  else if (input(ROW4) == 1){
    return 'd';
  }
  
  // returns 'X' if no button is pressed
  return 'X';
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
    char keyPressed = getKey();
    
    print("keyPressed: %c\n", keyPressed);
    
    pause(1000);
    
    
    /* Commented out while I work on getting the keypad working.
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
    */
  }
}
