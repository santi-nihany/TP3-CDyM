/*
 * CFile1.c
 *
 */
#include <avr/io.h>
#include <util/delay.h>

void DHT11_init(){
	DDRC |= 0x01;		//PINC0 COMO SALIDA
	PORTC |= 0x01;	// PORTC0 EN ALTO
};

uint8_t DHT11_read(float *dht_temperatura, float *dht_humedad) {
	uint8_t bytes[5];
	uint8_t i,j=0;
	uint8_t contador = 0;
	
	//Paso 1, enviar un puslo en bajo durante18ms
	
	PORTC &= ~(1<<PINC0);		//Nivel bajo
	_delay_ms(18);
	PORTC |= (1<<PINC0);		//Nivel alto
	DDRC &= ~(1<<PINC0);		//PINC0 como entrada
	
	//Paso 2 esperamos 20 a 40us hasta que el dht11 envie 0
	contador = 0;
	while(PINC & (1<<PINC0))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 60)
		{
			DDRC |= (1<<PINC0);	//PINC0 como salida
			PORTC |= (1<<PINC0);	//Nivel alto
			return 0;
		}
	}
	
	//Paso 3 esperamos 80us hasta que el dht11 envie 1
	contador = 0;
	while(!(PINC & (1<<PINC0)))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DDRC |= (1<<PINC0);	//PINC0 como salida
			PORTC |= (1<<PINC0);	//Nivel alto
			return 0;
		}
	}
	
	//Paso 4 esperamos 80us hasta que el dht11 envie 0
	contador = 0;
	while(PINC & (1<<PINC0))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DDRC |= (1<<PINC0);	//PINC0 como salida
			PORTC |= (1<<PINC0);	//Nivel alto
			return 0;
		}
	}
	
	//Paso 5 leemos los 40 bits o 5 bytes
	for (j=0; j<5; j++)
	{
		uint8_t result=0;
		for (i=0; i<8; i++)
		{
			while (!(PINC & (1<<PINC0))); // ESPERO alto 50us
			_delay_us(35);

			if (PINC & (1<<PINC0))		// si està en alto, escribo 1 en el bit correspondiente
				result |= (1<<(7-i));
			
			while(PINC & (1<<PINC0));	// espero bajo 
		}
		bytes[j] = result;
	}
	
	DHT11_init(); // REINICIALIZO SENSOR
	/*if ((uint8_t) (bytes[0] + bytes[1] + bytes[2] + bytes[3]) == bytes[4])		//Pregunta por el cheksum
	{
		char * t_decimal;
		sprintf(temperatura, "%d", bytes[0]);
		sprintf(t_decimal, "%d", bytes[1]);
		strcat(temperatura,".");
		strcat(temperatura,t_decimal);
		char * h_decimal;
		sprintf(humedad, "%d", bytes[2]);
		sprintf(h_decimal, "%d", bytes[3]);
		strcat(humedad,".");
		strcat(humedad,h_decimal);
	}*/
	
	if ((uint8_t) (bytes[0] + bytes[1] + bytes[2] + bytes[3]) == bytes[4])		//Pregunta por el cheksum
	{
		uint16_t rawhumedad = bytes[0]<<8 | bytes[1];
		uint16_t rawtemperatura = bytes[2] <<8 | bytes[3];
		
		
		if (rawtemperatura & 0x8000)
		{
			*dht_temperatura = (float)((rawtemperatura & 0x7fff) / 10.0)* -1.0;
			}else{
			*dht_temperatura = (float)(rawtemperatura)/10.0;
		}

		*dht_humedad = (float)(rawhumedad)/10.0;

		return 1;
	}
}