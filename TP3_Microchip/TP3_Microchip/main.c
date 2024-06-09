#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/setbaud.h>

#include "UART.h"
#include "DHT11.h"
#include "lcd.h"
#include "utils.h"
#include "RTC.h"


int main(void)
{
	uint8_t temperatura_int;
	uint8_t temperatura_dec;
	uint8_t humedad_int;
	uint8_t humedad_dec;
	
	DHT11_init();
	UART_init();
	
	_delay_ms(10);
	LCD_Init();
	_delay_ms(100);
	
	while (1) {
		uint8_t status = DHT11_read(&temperatura_int, &temperatura_dec ,&humedad_int, &humedad_dec );
		if (status) {
			LCDclr();
			printf("Temperatura = %d.%d\n\r", temperatura_int, temperatura_dec);
			printf("Humedad = %d.%d\n\r", humedad_int, humedad_dec);
			
			LCDGotoXY(0,0);
			LCDstring("Temp: ",strlen("Temp: "));
			LCDescribeDato(temperatura_int,2);
			LCDstring(".",strlen("."));
			LCDescribeDato(temperatura_dec,2);
			LCDstring(" C",strlen(" C"));
			LCDGotoXY(0,1);
			LCDstring("Hum: ",strlen("Hum: "));
			LCDescribeDato(humedad_int,2);
			LCDstring(".",strlen("."));
			LCDescribeDato(humedad_dec,2);
			LCDstring(" %",strlen(" %"));
			
			_delay_ms(100);
			/*sprintf(temperatura_int, "%d", temperatura_int);
			sprintf(humedad_int, "%d", humedad_int);
			Sol_error();
			escribir(temperatura_int, humedad_int);*/
		} else {
			printf("ERROR\n\r");
			LCDclr();
			_delay_ms(100);
			LCDstring("ERROR", 5);
		}
		_delay_ms(2000);
	}

	
}