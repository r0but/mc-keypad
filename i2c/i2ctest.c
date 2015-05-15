#include "simpletools.h"

// I2C address for clock: 0b1101000

i2c *TIME_BUS;

const int SCL_PIN = 15;
const int SDA_PIN = 14;

int convertClockNumsToInt(uint8_t weirdNum){
  int regOnes = weirdNum % 16;
  int regTens = weirdNum >> 4;
  
  return ((regTens * 10) + regOnes) % 60;
}

int convertClockHourToInt(uint8_t clockHour){
  int regOnes = clockHour % 16;
  int regTens = clockHour >> 4;
  
  if ((regTens >> 1) % 2 == 1){
    return regOnes + 20;
  }    
  if ((regTens % 2) == 1){
    return regOnes + 10;
  }    
}

// register 0: seconds
// 
int initClockValue(){
  uint8_t *valueToSend;
  
  // secs
  *valueToSend = 0b00000001;
  i2c_out(TIME_BUS, 0b1101000, 0x00, 1, valueToSend, 1);
  
  // mins
  *valueToSend = 0b00000111;
  i2c_out(TIME_BUS, 0b1101000, 0x01, 1, valueToSend, 1);
  
  // hours
  *valueToSend = 0b01010101;
  i2c_out(TIME_BUS, 0b1101000, 0x02, 1, valueToSend, 1);
  
  // days of week (1-7)
  *valueToSend = 0b00000101;
  i2c_out(TIME_BUS, 0b1101000, 0x03, 1, valueToSend, 1);
  
  // day of month (1-31)
  *valueToSend = 0b00010100;
  i2c_out(TIME_BUS, 0b1101000, 0x04, 1, valueToSend, 1);
  
  // month
  *valueToSend = 0b00000101;
  i2c_out(TIME_BUS, 0b1101000, 0x05, 1, valueToSend, 1);
  
  // year
  *valueToSend = 0b00010101;
  i2c_out(TIME_BUS, 0b1101000, 0x06, 1, valueToSend, 1);
  
  // start oscillator
  int *valueToGet;
  i2c_in(TIME_BUS, 0b1101000, 0x0E, 1, valueToGet, 1);
  *valueToSend = *valueToGet + 0b10000000;
  i2c_out(TIME_BUS, 0b1101000, 0x0E, 1, valueToSend, 1);
  
  return 0;
}  

int main(){
  TIME_BUS = i2c_newbus(SCL_PIN, SDA_PIN, 0);
  
  while(i2c_busy(TIME_BUS, 0b1101000));
  
  initClockValue();
  
  while(1){
    uint8_t *recieveDate;
    uint8_t *recieveMonth;
    uint8_t *recieveYear;
    
    i2c_in(TIME_BUS, 0b1101000, 0x04, 1, recieveDate, 1);
    
    i2c_in(TIME_BUS, 0b1101000, 0x05, 1, recieveMonth, 1);
    
    i2c_in(TIME_BUS, 0b1101000, 0x06, 1, recieveYear, 1);
    
    print("Year: %b\nMonth: %b\nDay: %b\n", *recieveYear, 
          *recieveMonth, *recieveDate);
    
    pause(1000);
  }  
  return 0;
}