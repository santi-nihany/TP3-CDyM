#include "I2C.h"

void I2C_Init(void) {
	TWSR=0x00;
	TWBR=152;
	TWCR=0x04;
}

void I2C_Start(void){
	TWCR= (1<<TWINT) | (1<<TWSTA) | (1 << TWEN);
	while ((TWCR & (1<< TWINT))==0);
}

uint8_t I2C_ReStart( void )
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	while ( !(TWCR & (1<<TWINT)) );

	return 1;
}

void I2C_Write (unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while ((TWCR & (1<<TWINT))==0);
}
unsigned char I2C_Read(unsigned char isLast) {
	if (isLast == 0)
	TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
	else
	TWCR = (1<<TWINT) | (1<<TWEN);
	while ((TWCR & (1<<TWINT))==0);
	return TWDR;
}
void I2C_Stop(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWSTO);
}