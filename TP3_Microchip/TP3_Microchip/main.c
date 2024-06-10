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
	unsigned char c;
	RTC_t currentTime;
	
	DHT11_init();
	UART_init();
	
	_delay_ms(10);
	LCD_Init();
	_delay_ms(100);
	
	
	currentTime.hora.Second = dec_to_bcd(1); 
	currentTime.hora.Minute = dec_to_bcd(1); 
	currentTime.hora.Hour = dec_to_bcd(12);   
	currentTime.fecha.Day = dec_to_bcd(9);
	currentTime.fecha.Month = dec_to_bcd(6);
	currentTime.fecha.Year = dec_to_bcd(24);
	RTC_Init();
	_delay_ms(10);
	RTC_SetTime(&currentTime);
	
	printf("Ingrese 's' o 'S' para parar/reanudar \n\r");
	while (1) {
		RTC_GetTime(&currentTime);
		uint8_t day = bcd_to_dec(currentTime.fecha.Day);
		uint8_t month = bcd_to_dec(currentTime.fecha.Month);
		uint8_t year = bcd_to_dec(currentTime.fecha.Year);
		uint8_t hour = bcd_to_dec(currentTime.hora.Hour);
		uint8_t minute = currentTime.hora.Minute;
		uint8_t second = currentTime.hora.Second;
		
		uint8_t status = DHT11_read(&temperatura_int, &temperatura_dec ,&humedad_int, &humedad_dec );
		
		c = UART_read();
		if(c != 0) {
			if((c == 's') || (c == 'S')){
				do
				{
					c = UART_read();
				} while ((c != 's') && (c!= 'S'));
			}
		}
		
		if (status) {
			LCDclr();
			// Escritura en PC
			printf("TEMP: %d C HUM: %d %% FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\n\r",temperatura_int, humedad_int, day, month, year, hour, minute, second);
			
			// Escritura LCD Display
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
			} else {
			printf("ERROR\n\r");
			LCDclr();
			_delay_ms(100);
			LCDstring("ERROR", 5);
		}
		
		_delay_ms(2000);
	}

	
}