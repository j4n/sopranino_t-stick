// vi:ft=c
// Test program for reading CY8C201xx using I2C
// Joseph Malloch 2011
// Input Devices and Music Interaction Lab

// See the CY8C201xx Register Reference Guide for more info:
// http://www.cypress.com/?docID=24620

// include Wire library to read and write I2C commands:

#include <Wire.h>

int xres = 5;  // XRES pin on one of the CY8C201xx chips is connected to Arduino pin 13

//define values for slip coding
byte escapeChar = 101;
byte delimiterChar = 100;

// I2C adresses
#define I2C_ADDR0 0x00
#define I2C_ADDR1 0x01

// some CY8C201xx registers
#define INPUT_PORT0 0x00
#define INPUT_PORT1 0x01
#define CS_ENABLE0 0x06
#define CS_ENABLE1 0x07
#define I2C_DEV_LOCK 0x79
#define I2C_ADDR_DM 0x7C
#define COMMAND_REG 0xA0

#define DEBUG_DELAY 1
// Secret codes for locking/unlocking the I2C_DEV_LOCK register
byte I2CDL_KEY_UNLOCK[3] = {0x3C, 0xA5, 0x69};
byte I2CDL_KEY_LOCK[3] = {0x96, 0x5A, 0xC3};

void setup() {
  // start serial interface
  Serial.begin(57600);
  
  //start I2C bus
  Wire.begin();
  
  // set pin modes
  pinMode(xres, OUTPUT);

  // chip #1: put into reset mode
  digitalWrite(xres, HIGH);
  delay(200);


  // CONFIGURE CHIP #2

  //delay(DEBUG_DELAY);
  // chip #2: unlock the I2C_DEV_LOCK register
  Wire.beginTransmission(I2C_ADDR0);
  Wire.send(I2C_DEV_LOCK);
  Wire.send(I2CDL_KEY_UNLOCK, 3);
  Wire.endTransmission();
  
  //delay(DEBUG_DELAY);
  // chip #2: change the I2C_ADDR_DM register to I2C_ADDR1
  Wire.beginTransmission(I2C_ADDR0);
  Wire.send(I2C_ADDR_DM);
  Wire.send(I2C_ADDR1);
  Wire.endTransmission();
  
  //delay(DEBUG_DELAY);
  // chip #2: lock register again for change to take effect
  Wire.beginTransmission(I2C_ADDR0);
  Wire.send(I2C_DEV_LOCK);
  Wire.send(I2CDL_KEY_LOCK, 3);
  Wire.endTransmission();
  // chip #2 now has the I2C address I2C_ADDR1

  //delay(DEBUG_DELAY);
  // CONFIGURE CHIP #1
  // let the chip #1 wake up again
  digitalWrite(xres, LOW);

  delay(200);
 //    
  for (uint8_t id=0; id<2; id++) {
    // switch to setup mode
    Wire.beginTransmission(id);
    Wire.send(COMMAND_REG);
    Wire.send(0x08);
    Wire.endTransmission();
//    delay(DEBUG_DELAY);
    
    // setup CS_ENABLE0 register
    Wire.beginTransmission(id);
    Wire.send(CS_ENABLE0);
    Wire.send(B00001111);
    Wire.endTransmission();
 //   delay(DEBUG_DELAY);
    
    // setup CS_ENABLE1 register
    Wire.beginTransmission(id);
    Wire.send(CS_ENABLE1);
    Wire.send(B00001111);
    Wire.endTransmission();
//    delay(DEBUG_DELAY);
    
    // switch to normal mode
    Wire.beginTransmission(id);
    Wire.send(COMMAND_REG);
    Wire.send(0x07);
    Wire.endTransmission();
  //  delay(DEBUG_DELAY);
  }
}

void loop() {
  byte i;
  
//  while (Serial.read() == -1) {
 //   ; // do nothing until polled
  //}
  
  // get the touch values from 2 x CY8C201xx chips
//  slipOut(readTouch(I2C_ADDR0));
  //slipOut(readTouch(I2C_ADDR1));
//  
  Serial.print("Reading Chip 0: ");
  Serial.print(readTouch(I2C_ADDR0),BIN);
  
  Serial.print(", Reading Chip 1: ");
 Serial.print(readTouch(I2C_ADDR1),BIN);

//  Serial.print(delimiterChar, BYTE);
  Serial.println();
	delay(50);
}

byte readTouch(int address) {
  byte touch = 0;
  
  // request Register 00h: INPUT_PORT0
  Wire.beginTransmission(address);
  Wire.send(INPUT_PORT0);
  Wire.endTransmission();
  
  Wire.requestFrom(address, 1);
  while (!Wire.available()) {}
  touch = Wire.receive() << 4;
  
  // request Register 01h: INPUT_PORT1
  Wire.beginTransmission(address);
  Wire.send(INPUT_PORT1);
  Wire.endTransmission();
  
  Wire.requestFrom(address, 1);
  while (!Wire.available()) {}
  touch |= Wire.receive();
  
  return touch;
}

void slipOut(byte output) {
    if ((output==escapeChar)||(output==delimiterChar)) Serial.print(escapeChar, BYTE);
    Serial.print(output, BYTE);
}
