#define F_CPU 16000000UL
#define PIN 0
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


void DHT11_init(){
	DDRC |= (1<<PIN);		//PINC0 COMO SALIDA
	PORTC |= (1<<PIN);	// PORTC0 EN ALTO
};

uint8_t DHT11_read(uint8_t *temperatura_int, uint8_t *temperatura_dec, uint8_t *humedad_int, uint8_t *humedad_dec) {
	uint8_t bytes[5];
	uint8_t i,j=0;
	uint8_t contador = 0;
	
	//Paso 1, enviar un puslo en bajo durante18ms
	
	PORTC &= ~(1<<PIN);		//Nivel bajo
	_delay_ms(18);
	PORTC |= (1<<PIN);		//Nivel alto
	DDRC &= ~(1<<PIN);		//PINC0 como entrada
	
	//Paso 2 esperamos 20 a 40us hasta que el dht11 envie 0
	contador = 0;
	while(PINC & (1<<PIN))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 60)
		{
			DDRC |= (1<<PIN);	//PINC0 como salida
			PORTC |= (1<<PIN);	//Nivel alto
			return 0;
		}
	}
	
	//Paso 3 esperamos 80us hasta que el dht11 envie 1
	contador = 0;
	while(!(PINC & (1<<PIN)))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DDRC |= (1<<PIN);	//PINC0 como salida
			PORTC |= (1<<PIN);	//Nivel alto
			return 0;
		}
	}
	
	//Paso 4 esperamos 80us hasta que el dht11 envie 0
	contador = 0;
	while(PINC & (1<<PIN))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DDRC |= (1<<PIN);	//PINC0 como salida
			PORTC |= (1<<PIN);	//Nivel alto
			return 0;
		}
	}
	
	//Paso 5 leemos los 40 bits o 5 bytes
	for (j=0; j<5; j++)
	{
		uint8_t result=0;
		for (i=0; i<8; i++)
		{
			while (!(PINC & (1<<PIN))); // ESPERO alto 50us
			_delay_us(35);

			if (PINC & (1<<PIN))		// si està en alto, escribo 1 en el bit correspondiente
				result |= (1<<(7-i));
			
			while(PINC & (1<<PIN));	// espero bajo 
		}
		bytes[j] = result;
	}
	
	DHT11_init(); // REINICIALIZO SENSOR
	if ((uint8_t) (bytes[0] + bytes[1] + bytes[2] + bytes[3]) == bytes[4])		//Pregunta por el cheksum
	{
		 * temperatura_int = bytes[0];
		 * temperatura_dec = bytes[1];
		 * humedad_int = bytes[2];
		 * humedad_dec = bytes[3];
		 return 1;
	}
	
	return 0;
		
}