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
	
	RTC_t currentTime;
	currentTime.hora.Second = dec_to_bcd(1);    // 0 segundos
	currentTime.hora.Minute = dec_to_bcd(1);    // 0 minutos
	currentTime.hora.Hour = dec_to_bcd(12);     // 12 horas (mediodía)
	currentTime.fecha.Day = dec_to_bcd(9);      // 9 del mes
	currentTime.fecha.Month = dec_to_bcd(6);  // Junio (mes 6)
	currentTime.fecha.Year = dec_to_bcd(24);
	RTC_Init();
	_delay_ms(10);
	RTC_SetTime(&currentTime);
	RTC_SetFecha(&currentTime.fecha);
	
	
	while (1) {
		RTC_GetTime(&currentTime);
		uint8_t day = bcd_to_dec(currentTime.fecha.Day);
		uint8_t month = bcd_to_dec(currentTime.fecha.Month);
		uint8_t year = bcd_to_dec(currentTime.fecha.Year);
		uint8_t hour = bcd_to_dec(currentTime.hora.Hour);
		uint8_t minute = currentTime.hora.Minute;
		uint8_t second = currentTime.hora.Second;

		// Imprimir la fecha y hora en formato legible
		printf("%02d/%02d/%02d\n %02d:%02d:%02d\r", day, month, year, hour, minute, second);
		
		uint8_t status = DHT11_read(&temperatura_int, &temperatura_dec ,&humedad_int, &humedad_dec );
		if (status) {
			LCDclr();
			//printf("Temperatura = %d.%d\n\r", temperatura_int, temperatura_dec);
			//printf("Humedad = %d.%d\n\r", humedad_int, humedad_dec);
			
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