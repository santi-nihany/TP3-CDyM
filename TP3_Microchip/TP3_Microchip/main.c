#include <avr/io.h>
#include "DHT11.h"
#include "lcd.h"
#include <util/delay.h>

#define F_CPU 16000000UL

int main(void)
{
	//uint8_t * temperatura;
	//uint8_t * humedad;
	char printbuff[10];
	float temperatura;
	float humedad;
	DHT11_init();
	_delay_ms(10);
	LCD_Init();
	_delay_ms(100);
	
	
	while (1) {
		LCDstring(" Temp. ",strlen(" Temp. "));
		uint8_t status = DHT11_read(&temperatura,&humedad);
		if (status) {
			
			LCDclr();
			LCDstring(" Temp. ",strlen(" Temp. "));
			dtostrf(temperatura, 2, 2, printbuff);
			LCDstring(printbuff,strlen(printbuff));
			LCDstring(" C",strlen(" C"));
			
			LCDGotoXY(0,1);
			LCDstring(" Hume. ",strlen(" Hume. "));
			dtostrf(humedad, 2, 2, printbuff);
			LCDstring(printbuff,strlen(printbuff));
			LCDstring(" %",strlen(" %"));
			_delay_ms(200);
		}
	}

	
}

