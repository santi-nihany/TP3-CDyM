#include "I2C.h"

void I2C_Init(void){
	TWSR=0x00;    
	TWBR=152;     
	TWCR=0x04;    
}

void I2C_Start(void){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}
void I2C_Write(unsigned char data){
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}

unsigned char I2C_Read(unsigned char isLast){
	if (isLast == 0){  // send ACK
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	}
	else{  // send NACK
		TWCR = (1 << TWINT) | (1 << TWEN);
	}
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

void I2C_Stop(void){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}