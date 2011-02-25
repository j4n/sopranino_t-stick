// Cypress CY8C201x I2C Library
// supports two chips, the second one needs a digital output on the uc 
// connected to the XRES pin
// based on Joseph Mallochs example code
// (c) 2011 Jan Anlauf <janlauff <at> cim <dot> mcgill <dot> ca>

#include "includes/cy8c20180.h"

void cy8c20180_config(void) {
	XRES_PORT_REGISTER |= _BV(XRES_PIN);

	// put chip #0 in reset/deactivate it
	XRES_PORT |= _BV(XRES_PIN);

  long_delay(200);

//  // CONFIGURE CHIP #2
//
//  delay(DEBUG_DELAY);
//  // chip #2: unlock the I2C_DEV_LOCK register
//  Wire.beginTransmission(I2C_ADDR0);
//  Wire.send(I2C_DEV_LOCK);
//  Wire.send(I2CDL_KEY_UNLOCK, 3);
//  Wire.endTransmission();
//  
//  delay(DEBUG_DELAY);
//  // chip #2: change the I2C_ADDR_DM register to I2C_ADDR1
//  Wire.beginTransmission(I2C_ADDR0);
//  Wire.send(I2C_ADDR_DM);
//  Wire.send(I2C_ADDR1);
//  Wire.endTransmission();
//  
//  delay(DEBUG_DELAY);
//  // chip #2: lock register again for change to take effect
//  Wire.beginTransmission(I2C_ADDR0);
//  Wire.send(I2C_DEV_LOCK);
//  Wire.send(I2CDL_KEY_LOCK, 3);
//  Wire.endTransmission();
//  // chip #2 now has the I2C address I2C_ADDR1
//
//  delay(DEBUG_DELAY);
//  // CONFIGURE CHIP #1
//  // let the chip #1 wake up again
//  digitalWrite(xres, LOW);
//
//  delay(200);
// //    
//  for (uint8_t id=0; id<2; id++) {
//    // switch to setup mode
//    Wire.beginTransmission(id);
//    Wire.send(COMMAND_REG);
//    Wire.send(0x08);
//    Wire.endTransmission();
////    delay(DEBUG_DELAY);
//    
//    // setup CS_ENABLE0 register
//    Wire.beginTransmission(id);
//    Wire.send(CS_ENABLE0);
//    Wire.send(B00001111);
//    Wire.endTransmission();
// //   delay(DEBUG_DELAY);
//    
//    // setup CS_ENABLE1 register
//    Wire.beginTransmission(id);
//    Wire.send(CS_ENABLE1);
//    Wire.send(B00001111);
//    Wire.endTransmission();
////    delay(DEBUG_DELAY);
//    
//    // switch to normal mode
//    Wire.beginTransmission(id);
//    Wire.send(COMMAND_REG);
//    Wire.send(0x07);
//    Wire.endTransmission();
//  //  delay(DEBUG_DELAY);
//  }
//
	// activate chip #0 again
	XRES_PORT &= ~_BV(XRES_PIN);

}


uint8_t* cy8c20180_read(void) {
//	twi_start(ADXL345_ADDR + I2C_WRITE);
//	twi_write(register_name);
//	twi_write(value);
//	twi_stop();
}

