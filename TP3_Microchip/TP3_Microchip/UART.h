

void UART_init(void);							//funci�n para iniciar el USART AVR as�ncrono, 8 bits, 9600 baudios,
unsigned char UART_read(void);					//funci�n para la recepci�n de caracteres
void UART_write(unsigned char);				//funci�n para la transmisi�n de caracteres
void UART_msg(char*);						//funci�n para la transmisi�n de cadenas de caracteres
int USART_printCHAR(char character, FILE *stream);

static FILE USART_0_stream = FDEV_SETUP_STREAM(USART_printCHAR, NULL, _FDEV_SETUP_WRITE);

int USART_printCHAR(char character, FILE *stream)
{
	UART_write(character);	
	return 0;

}


void UART_init(void)
{
	DDRD |= (1<<1);							//PD1	COMO SALIDA TXa
	DDRD &= ~(1<<0);						//PD0	COMO ENTRADA RX
	UCSR0A = (0<<TXC0)|(0<<U2X0)|(0<<MPCM0);
	UCSR0B = (1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<TXB80);
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	stdout = &USART_0_stream;
	//UBRR0 = 103;								//NO DUPLICA VELOCIDAD 9600B A 160000
}

unsigned char UART_read(){
	if(UCSR0A&(1<<7)){			//si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;			//devuelve el dato almacenado en el registro UDR0
	}
	else
	return 0;
}

void UART_write(unsigned char caracter){
	while(!(UCSR0A&(1<<5)));    // mientras el registro UDR0 est� lleno espera
	UDR0 = caracter;            //cuando el el registro UDR0 est� vacio se envia el caracter
}

void UART_write_txt(char* cadena){			//cadena de caracteres de tipo char
	while(*cadena !=0x00){				//mientras el �ltimo valor de la cadena sea diferente a el caracter nulo
		UART_write(*cadena);			//transmite los caracteres de cadena
		cadena++;						//incrementa la ubicaci�n de los caracteres en cadena
										//para enviar el siguiente caracter de cadena
	}
}


