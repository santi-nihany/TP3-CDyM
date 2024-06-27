#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "DHT11.h"
#include "RTC.h"
#include "SerialPort.h"

#define BR9600 (0x67)
volatile char RX_Buffer=0;
uint8_t parar = 0;

int main(void)
{
	uint8_t temperatura_int;
	uint8_t temperatura_dec;
	uint8_t humedad_int;
	uint8_t humedad_dec;
	RTC_t currentTime;
	char mensaje[100]; // Define mensaje buffer
	// Configuracion UART
	SerialPort_Init(BR9600); 
	SerialPort_TX_Enable();
	SerialPort_RX_Enable();
	SerialPort_Send_String("Presionando S se detiene/reanuda el sistema\n\r");
	SerialPort_RX_Interrupt_Enable();
	
	sei();
	
	
	DHT11_init();
	/* Para settear el tiempo en Proteus
	currentTime.hora.Second = dec_to_bcd(0);
	currentTime.hora.Minute = dec_to_bcd(0);
	currentTime.hora.Hour = dec_to_bcd(12);
	currentTime.fecha.Day = dec_to_bcd(23);
	currentTime.fecha.Month = dec_to_bcd(6);
	currentTime.fecha.Year = dec_to_bcd(24);
	*/
	RTC_Init();
	_delay_ms(10);
	//RTC_SetTime(&currentTime); //Para settear el tiempo en Proteus
	
	
	while (1) {
		if(RX_Buffer){
			if ((RX_Buffer == 's')||(RX_Buffer == 'S')){
				parar = !parar;
				if(parar){
					SerialPort_Send_String("Recepcion detenida\n\r");
					} else {
					SerialPort_Send_String("Recepcion reanudada\n\r");
				}
			}
			RX_Buffer=0;
		}
		if(!parar){
			RTC_GetTime(&currentTime);
			uint8_t day = bcd_to_dec(currentTime.fecha.Day);
			uint8_t month = bcd_to_dec(currentTime.fecha.Month);
			uint8_t year = bcd_to_dec(currentTime.fecha.Year);
			uint8_t hour = bcd_to_dec(currentTime.hora.Hour);
			uint8_t minute = bcd_to_dec(currentTime.hora.Minute);
			uint8_t second = bcd_to_dec(currentTime.hora.Second);
			uint8_t status = DHT11_read(&temperatura_int, &temperatura_dec ,&humedad_int, &humedad_dec );
				
			if (status) {
				sprintf(mensaje, "TEMP: %d C HUM: %d %% FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\n\r",temperatura_int, humedad_int, day, month, year, hour, minute, second);
				SerialPort_Send_String(mensaje);
			} else {
				SerialPort_Send_String("ERROR\n\r");
			}
			_delay_ms(2000);
		}
	}
}

ISR(USART_RX_vect){
	RX_Buffer = UDR0;
}
