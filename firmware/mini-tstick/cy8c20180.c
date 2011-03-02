// Cypress CY8C201x I2C Library
// supports two chips, the second one needs a digital output on the uc 
// connected to the XRES pin
// based on Joseph Mallochs example code
// (c) 2011 Jan Anlauf <janlauff <at> cim <dot> mcgill <dot> ca>
//
// Explanation on Chip IDs:
//  Chip #0 is the lower chip, responsible for channels 0-7
//  Chip #1 is the upper chip, responsible for channels 8-15

#include "includes/cy8c20180.h"
#include "includes/io.h"

void cy8c20180_config(void) {
	// this has actually never worked, used the arduino sketch 
	// in ../test_capsense/firmware to set the variables and write
	// them to the non-volatile memory (NVM)
	// note that it takes 120 ms for the chip to do that!


	//  First, we will configure the chip addresses
	//  Chip #0 will be disabled to make Chip #1 be the only one with address 0x00
	//  Then Chip #1 aka. 0x00 will be set to have address 0x01, and finally
	//  Chip #0 will be enabled again.

	// put chip #0 in reset/deactivate it by pulling XRES_PIN low
	XRES_PORT_REGISTER |= _BV(XRES_PIN);
	XRES_PORT |= _BV(XRES_PIN);

  long_delay(200);

  // CONFIGURE CHIP #1

	// unlock the I2C_DEV_LOCK register with magic codes
	i2c_start(I2C_ADDR0);
	i2c_write(I2C_DEV_LOCK);
  i2c_write(0x3C);
  i2c_write(0xA5);
  i2c_write(0x69);
	i2c_stop();

  long_delay(20);
  // chip #2: change the I2C_ADDR_DM register to I2C_ADDR1
	i2c_start(I2C_ADDR0);
  i2c_write(I2C_ADDR_DM);
	i2c_write(I2C_ADDR1);
	i2c_stop();

  long_delay(20);
  // chip #2: lock register again for change to take effect
	i2c_start(I2C_ADDR0);
	i2c_write(I2C_DEV_LOCK);
  i2c_write(0x96);
  i2c_write(0x5A);
  i2c_write(0xC3);
	i2c_stop();

  long_delay(20);
  // chip #2 now has the I2C address I2C_ADDR1

	// activate chip #0 again
	XRES_PORT &= ~_BV(XRES_PIN);
  long_delay(20);

	// configure capsense ports for both chips, 0x00 & 0x01
  for (uint8_t id=0; id<=1; id++) {
    // switch to setup mode
		i2c_start(id);
		i2c_write(COMMAND_REG);
		i2c_write(0x08);
		i2c_stop();
    
    // setup CS_ENABLE0 register
    i2c_start(id);
    i2c_write(CS_ENABLE0);
    i2c_write(0x0F);
    i2c_stop();
 //   delay(DEBUG_DELAY);
    
    // setup CS_ENABLE1 register
    i2c_start(id);
    i2c_write(CS_ENABLE1);
    i2c_write(0x0F);
    i2c_stop();
//    delay(DEBUG_DELAY);
    
    // switch to normal mode
    i2c_start(id);
    i2c_write(COMMAND_REG);
    i2c_write(0x07);
    i2c_stop();
  //  delay(DEBUG_DELAY);
  }
}

uint8_t cy8c20180_read(uint8_t address) {
	// request data like described in the datasheet page 9 - one byte of data
	// BUG: data only reads from chip #0 when both chips are sampled, and turns
	// 			up in the space for chip #1 - maybe it takes too long?
	
  uint8_t ret = 0x00;
	
  // request Register 00h: INPUT_PORT0

  i2c_start_wait(address);
  i2c_write(INPUT_PORT0);
  i2c_stop();

  i2c_start_wait(address);
	ret = i2c_readNak() << 4;
  i2c_stop();

//	if (address == 0) { ret |= 1 << 4; }

  // request Register 01h: INPUT_PORT1
  i2c_start_wait(address);
  i2c_write(INPUT_PORT1);
  i2c_stop();

  i2c_start_wait(address);
	ret |= i2c_readNak();
  i2c_stop();

  return ret;
}

