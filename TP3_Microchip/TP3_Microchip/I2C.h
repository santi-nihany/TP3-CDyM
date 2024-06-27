#include <avr/io.h>

#define F_CPU 16000000UL
#define F_SCL 400000UL

#define TWBRX0 (((F_CPU/F_SCL)-16)/2)
#define TWBRX1 TWBRX0/4
#define TWBRX2 TWBRX0/16
#define TWBRX3 TWBRX0/64

#define NACK 0
#define ACK 1

void I2C_Init(void);
void I2C_Start(void);
void I2C_Write(unsigned char data);
unsigned char I2C_Read(unsigned char isLast);
void I2C_Stop();
