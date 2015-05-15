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




/***********************
*   CHANGEABLE STUFF   *
***********************/

// Number of questions to ask the user.
const int NUM_OF_QUESTIONS = 3;

// Prompt that shows for two seconds on startup.
// MAXIMUM 16 CHARACTERS EACH.
const char STARTUP_PROMPT_LINE1[17] = "Zone Two";
const char STARTUP_PROMPT_LINE2[17] = "Warehouse";

void initializeArrays(char *ITEM_CODE_LOOKUP[100],
                      char *FIRST_OPTION_LIST[NUM_OF_QUESTIONS],
                      char *SECOND_OPTION_LIST[NUM_OF_QUESTIONS]){
  for (int i = 0; i < 100; i++){
    ITEM_CODE_LOOKUP[i] = "";
  }    
                        
  ITEM_CODE_LOOKUP[11] = "Pump Shaft";
  ITEM_CODE_LOOKUP[12] = "Drive Gear";
  ITEM_CODE_LOOKUP[13] = "Drum Housing";
  ITEM_CODE_LOOKUP[14] = "Slide Sprocket";
  ITEM_CODE_LOOKUP[15] = "Drum Support";
  ITEM_CODE_LOOKUP[16] = "Plate Band";
  
  // Defining choices user can make.
  // MAX LENGTH IS 14 CHARACTERS.
  FIRST_OPTION_LIST[0]  = "Treated";
  SECOND_OPTION_LIST[0] = "Untreated";

  FIRST_OPTION_LIST[1]  = "In-house";
  SECOND_OPTION_LIST[1] = "Outside";

  FIRST_OPTION_LIST[2]  = "Print";
  SECOND_OPTION_LIST[2] = "Don't print";
}  

/***********************
*   CHANGEABLE STUFF   *
***********************/




// defining signal values for LCD to turn on and clear its screen
const int LCD_ON = 22;
const int LCD_CLR = 12;
const int LCD_LINE1 = 128;
const int LCD_LINE2 = 148;

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

char getKey(){
  // This code can be more elegant but I'm just happy it functions.
  
  // Pulling low voltage on all connections
  low(ROW1);
  low(ROW2);
  low(ROW3);
  low(ROW4);
  low(COL1);
  low(COL2);
  low(COL3);
  low(COL4);
  
  /* Pulling high voltage to column 1.
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

void initializeLCD(){
  // open LCD connection, stores memory address in lcd
  lcd = serial_open(12, 12, 0, 9600);
  
  // turn on LCD by sending global variable LCD_ON
  writeChar(lcd, LCD_ON);
  
  // clear LCD screen by sending global variable LCD_CLR
  writeChar(lcd, LCD_CLR);
}

char getInput(){
  char userInput = 'X';
  
  while (userInput == 'X'){
    userInput = getKey();
    pause(100);
  }    
  
  return userInput;
}

void printStartupScreen(){
  writeChar(lcd, LCD_CLR);
  dprint(lcd, STARTUP_PROMPT_LINE1);
  writeChar(lcd, LCD_LINE2);
  dprint(lcd, STARTUP_PROMPT_LINE2);
  pause(2000);
}  

int askUserQuestions(char *FIRST_OPTION_LIST[NUM_OF_QUESTIONS],
                     char *SECOND_OPTION_LIST[NUM_OF_QUESTIONS],
                     char userAnswers[NUM_OF_QUESTIONS][15]){
  
  for (int i = 0; i < NUM_OF_QUESTIONS; i++){
    writeChar(lcd, LCD_CLR);
    writeChar(lcd, LCD_LINE1);
    dprint(lcd, "1:");
    dprint(lcd, FIRST_OPTION_LIST[i]);
    writeChar(lcd, LCD_LINE2);
    dprint(lcd, "2:");
    dprint(lcd, SECOND_OPTION_LIST[i]);
    
    while(1){
      char charInput = getInput();
      
      if (charInput == '1'){
        strcpy(userAnswers[i], FIRST_OPTION_LIST[i]);
        break;
      }
      else if (charInput == '2'){
        strcpy(userAnswers[i], SECOND_OPTION_LIST[i]);
        break;
      }
      else if (charInput == '0'){
        // Exit code 1 means user wants to reset
        return 1;
      }
    }      

    writeChar(lcd, LCD_CLR);
    writeChar(lcd, LCD_LINE1);
    dprint(lcd, userAnswers[i]);
    pause(2000);
  }
  
  // Exit code 0 means user didn't reset
  return 0;
}

int getItemCode(){
  while(1){
    writeChar(lcd, LCD_CLR);
    writeChar(lcd, LCD_LINE1);
    dprint(lcd, "Enter item code.");
    writeChar(lcd, LCD_LINE2);
    char cTensDigit = getInput();
    writeChar(lcd, cTensDigit);
    pause(1000);
    char cOnesDigit = getInput();
    writeChar(lcd, cOnesDigit);
    pause(1000);
    
    int tensDigit = cTensDigit - '0';
    int onesDigit = cOnesDigit - '0';
    
    // 0 is the reset key on the keypad.
    // Returns sentinel value -1 to tell main loop to restart.
    if (tensDigit == 0 || onesDigit == 0){
      return -1;
    }
    // if user entered a non-numeric key, this will invalidate
    // and tell them to enter a new code
    else if (tensDigit < 1 || tensDigit > 9 ||
             onesDigit < 1 || onesDigit > 9){
      continue;
    }
    else{
      return (tensDigit * 10) + onesDigit;
    }      
  }
}  

int main(){
  // Declaring array of strings to hold item names.
  char *ITEM_CODE_LOOKUP[100];
  
  char *FIRST_OPTION_LIST[NUM_OF_QUESTIONS];
  char *SECOND_OPTION_LIST[NUM_OF_QUESTIONS];
  
  initializeArrays(ITEM_CODE_LOOKUP, FIRST_OPTION_LIST, SECOND_OPTION_LIST);
  
  int wantToExit = 0;
  int wantToReset = 0;
  while(1)
  {
    initializeLCD();
    dprint(lcd, STARTUP_PROMPT_LINE1);
    writeChar(lcd, LCD_LINE2);
    dprint(lcd, STARTUP_PROMPT_LINE2);
    pause(2000);
    
    int itemCode = getItemCode();
    
    print("itemCode: %d\n", itemCode);
    
    if (itemCode == -1){
      print("continuing\n");
      continue;
    }
    
    // Checks to see if there is a matching item for the code.
    // If not, restarts loop.
    if (!strcmp("", ITEM_CODE_LOOKUP[itemCode])){
      print("Item code invalid\n");
      continue;
    }      
    
    // declaring and initializing userAnswers
    char userAnswers[NUM_OF_QUESTIONS][15];
    for (int i = 0; i < NUM_OF_QUESTIONS; i++){
      for (int y = 0; y < 14; y++){
        userAnswers[i][y] = ' ';
      }
      userAnswers[i][14] = '\0';
    } 
    
    // function stores results in userAnswers
    wantToReset = askUserQuestions(FIRST_OPTION_LIST, 
                                   SECOND_OPTION_LIST, 
                                   userAnswers);
    if (wantToReset){
      continue;
    }
    
    
    if (!strcmp("Print", userAnswers[NUM_OF_QUESTIONS])){  
      print("%s\n", ITEM_CODE_LOOKUP[itemCode]);
      for (int i = 0; i < NUM_OF_QUESTIONS; i++){
        print("%s\n", userAnswers[i]);
      }
    }      
    
    pause(5000);
  }
  return 0;
}