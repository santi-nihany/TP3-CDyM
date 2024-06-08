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


int main(void)
{
	uint8_t temperatura_int;
	uint8_t temperatura_dec;
	uint8_t humedad_int;
	uint8_t humedad_dec;
	
	DHT11_init();
	_delay_ms(10);
	LCD_Init();
	_delay_ms(100);
	
	LCDclr();
	_delay_ms(100);

	UART_init();

	
	while (1) {
		uint8_t status = DHT11_read(&temperatura_int, &temperatura_dec ,&humedad_int, &humedad_dec );
		if (status) {
			LCDclr();
			printf("Temperatura = %d.%d\n\r", temperatura_int, temperatura_dec);
			printf("Humedad = %d.%d\n\r", humedad_int, humedad_dec);
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