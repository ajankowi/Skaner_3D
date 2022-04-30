#include <inttypes.h>
#include <compat/twi.h>
#include "i2cmaster.h"


//Inicjalizacja i2c
void i2c_init(void){
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;
}



//		Wysy³a adres i kierunek transferu
//		Zwraca
//		1 gdy jest dostêp do uzadzenia
//	    0 gdzy nie ma dostepu do uzadzenia

unsigned char i2c_start(unsigned char address){
    uint8_t   twst;
    uint16_t timeout=0xFFFF;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until  START condition has been transmitted
    while( --timeout > 0 ){
        if( TWCR & (1<<TWINT) ){
            break;
        }
    }
    if( timeout == 0 ){

        return 1;
    }
    timeout=0xFFFF;

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
    while( --timeout > 0 ){
        if( TWCR & (1<<TWINT) ){
            break;
        }
    }
    if( timeout == 0 ){

        return 1;
    }
	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ){
		return 1;
	}
	return 0;
}


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 Input:   address and transfer direction of I2C device
*************************************************************************/
void i2c_start_wait(unsigned char address){
    uint8_t   twst;
    while ( 1 ) {
	    // send START condition
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    	// wait until transmission completed
    	while(!(TWCR & (1<<TWINT)));

    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;

    	// send device address
    	TWDR = address;
    	TWCR = (1<<TWINT) | (1<<TWEN);

    	// wail until transmission completed
    	while(!(TWCR & (1<<TWINT)));

    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) )
    	{
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	        // wait until stop condition is executed and bus released
	        while(TWCR & (1<<TWSTO));

    	    continue;
    	}
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }

}/* i2c_start_wait */


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction
 Input:   address and transfer direction of I2C device

 Return:  0 device accessible
          1 failed to access device
*************************************************************************/
unsigned char i2c_rep_start(unsigned char address){
    return i2c_start( address );

}/* i2c_rep_start */


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
void i2c_stop(void){
    uint16_t timeout=0xFFFF;
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	// wait until stop condition is executed and bus released
    while( --timeout > 0 ){
        if( !(TWCR&(1<<TWSTO)) ){
            return;
        }
    }

}/* i2c_stop */


/*************************************************************************
  Send one byte to I2C device

  Input:    byte to be transfered
  Return:   0 write successful
            1 write failed
*************************************************************************/
unsigned char i2c_write( unsigned char data ){
    uint8_t   twst;
    uint16_t timeout=0xFFFF;
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
    // wait until  START condition has been transmitted
    while( --timeout > 0 ){
        if( TWCR & (1<<TWINT) ){
            break;
        }
    }
    if( timeout == 0 ){

        return 1;
    }

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK ){

		return 1;
	}
	return 0;

}/* i2c_write */


/*************************************************************************
 Read one byte from the I2C device, request more data from device

 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck(void){
	uint16_t timeout=0xFFFF;
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while( --timeout > 0 ){
		if( TWCR & (1<<TWINT) ){
		    return TWDR;
		}
	}

    return 0;
}/* i2c_readAck */


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition

 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void){
	uint16_t timeout=0xFFFF;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( --timeout > 0 ){
		if( TWCR & (1<<TWINT) ){
			return TWDR;
		}
	}

	return 0;
}/* i2c_readNak */

// Print out all the active I2C addresses on the bus
void searchI2C(){
	uint8_t devAdr;

	for( devAdr=0; devAdr<=127; devAdr++ ){
		if( !i2c_start( (devAdr<<1) | I2C_WRITE ) ){

		}
		i2c_stop();                             // set stop conditon = release bus
	}

}
