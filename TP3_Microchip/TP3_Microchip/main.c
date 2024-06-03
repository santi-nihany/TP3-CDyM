#include <avr/io.h>
#include "DHT11.h"
#include "lcd.h"
#include <util/delay.h>

#define F_CPU 16000000UL

int main(void)
{
	uint8_t * temperatura;
	uint8_t * humedad;
	_delay_ms(10);
    LCD_Init();
	_delay_ms(100);
	DHT11_init();
	DHT11_read(&temperatura,&humedad);
	printf("%s",temperatura);
	LCDstring(temperatura,strlen(temperatura));
	LCDGotoXY(0,1);
	LCDstring(humedad,strlen(humedad));
}

