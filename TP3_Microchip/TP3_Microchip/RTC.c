#include "RTC.h"
#include "I2C.h"

// Convierte de BCD a decimal
uint8_t bcd_to_dec(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}

// Convierte de decimal a BCD
uint8_t dec_to_bcd(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}

// Inicializa el RTC, que internamente inicializa el bus I2C
void RTC_Init() {
	I2C_Init();
}

// Configura la hora en el RTC
void RTC_SetHora(Hora_t *hora) {
	I2C_Start();               // Inicia la comunicación I2C
	I2C_Write(DS3231_WRITE);   // Dirección de escritura del DS3231
	I2C_Write(DS3231_SECONDS); // Dirección del registro de segundos
	I2C_Write(hora->Second & MASK_SEC); // Escribe los segundos
	I2C_Write(hora->Minute & MASK_MIN); // Escribe los minutos
	I2C_Write(hora->Hour & MASK_HORA);  // Escribe las horas
	I2C_Stop();                // Finaliza la comunicación I2C
}

// Configura la fecha en el RTC
void RTC_SetFecha(Fecha_t *fecha) {
	I2C_Start();               // Inicia la comunicación I2C
	I2C_Write(DS3231_WRITE);   // Dirección de escritura del DS3231
	I2C_Write(DS3231_DAYS);    // Dirección del registro de días
	I2C_Write(fecha->Day & MASK_DAY);   // Escribe el día
	I2C_Write(fecha->Month & MASK_MES); // Escribe el mes
	I2C_Write(fecha->Year & MASK_YEAR); // Escribe el año
	I2C_Stop();                // Finaliza la comunicación I2C
}

// Obtiene la hora del RTC
void RTC_GetHora(Hora_t *hora) {
	I2C_Start();               // Inicia la comunicación I2C
	I2C_Write(DS3231_WRITE);   // Dirección de escritura del DS3231
	I2C_Write(DS3231_SECONDS); // Dirección del registro de segundos
	I2C_ReStart();             // Reinicia la comunicación I2C para cambiar a lectura
	I2C_Write(DS3231_READ);    // Dirección de lectura del DS3231
	hora->Second = (I2C_Read(NACK)) & MASK_SEC; // Lee y almacena los segundos
	hora->Minute = (I2C_Read(NACK)) & MASK_MIN; // Lee y almacena los minutos
	hora->Hour = (I2C_Read(ACK)) & MASK_HORA;   // Lee y almacena las horas
	I2C_Stop();                // Finaliza la comunicación I2C
}

// Obtiene la fecha del RTC
void RTC_GetFecha(Fecha_t *fecha) {
	I2C_Start();               // Inicia la comunicación I2C
	I2C_Write(DS3231_WRITE);   // Dirección de escritura del DS3231
	I2C_Write(DS3231_DAYS);    // Dirección del registro de días
	I2C_ReStart();             // Reinicia la comunicación I2C para cambiar a lectura
	I2C_Write(DS3231_READ);    // Dirección de lectura del DS3231
	fecha->Day = (I2C_Read(NACK)) & MASK_DAY;   // Lee y almacena el día
	fecha->Month = (I2C_Read(NACK)) & MASK_MES; // Lee y almacena el mes
	fecha->Year = (I2C_Read(ACK)) & MASK_YEAR;  // Lee y almacena el año
	I2C_Stop();                // Finaliza la comunicación I2C
}

// Obtiene la hora y la fecha del RTC
void RTC_GetTime(RTC_t *rtc) {
	RTC_GetHora(&rtc->hora);   // Obtiene la hora y la almacena en la estructura
	RTC_GetFecha(&rtc->fecha); // Obtiene la fecha y la almacena en la estructura
}

// Configura la hora y la fecha en el RTC
void RTC_SetTime(RTC_t *rtc) {
	RTC_SetHora(&rtc->hora);   // Configura la hora en el RTC
	RTC_SetFecha(&rtc->fecha); // Configura la fecha en el RTC
}
