#include "simpletools.h"

/*
  Keypad diagram:
  
  1 2 3 A
  4 5 6 B
  7 8 9 C
  * 0 # D
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

void setupLCD(){
  // open LCD connection, stores memory address in lcd
  lcd = serial_open(12, 12, 0, 9600);
  
  // turn on LCD by sending global variable ON
  writeChar(lcd, ON);
  
  // clear LCD screen by sending global variable CLR
  writeChar(lcd, CLR);
}

char getKey(){
  // First pull columns one at a time, then poll the rows, then poll the columns.
  low(COL1);
  low(COL2);
  low(COL3);
  low(COL4);
  
  high(ROW1);
  
  int row1input = input(COL1);
  
  print("row1input = %d\n", row1input);
  
  return 'a';
}  

int main(){
  setupLCD();
  int i = 0;
  
  while(1)
  {
    print("%d: ", i);
    getKey();
    pause(1000);
    i++;
  }  
}
