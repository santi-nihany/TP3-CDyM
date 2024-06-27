#include "RTC.h"
#include "I2C.h"

uint8_t bcd_to_dec(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}

uint8_t dec_to_bcd(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}

void RTC_Init() {
	I2C_Init();
}

void RTC_SetHora(Hora_t *hora) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_SECONDS);
	I2C_Write(hora->Second & MASK_SEC);
	I2C_Write(hora->Minute & MASK_MIN);
	I2C_Write(hora->Hour & MASK_HORA);
	I2C_Stop();
}

void RTC_SetFecha(Fecha_t *fecha) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_DAYS);
	I2C_Write(fecha->Day & MASK_DAY);
	I2C_Write(fecha->Month & MASK_MES);
	I2C_Write(fecha->Year & MASK_YEAR);
	I2C_Stop();
}

void RTC_GetHora(Hora_t *hora) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_SECONDS);
	I2C_ReStart();
	I2C_Write(DS3231_READ);
	hora->Second = (I2C_Read(NACK)) & MASK_SEC;
	hora->Minute = (I2C_Read(NACK)) & MASK_MIN;
	hora->Hour = (I2C_Read(ACK)) & MASK_HORA;
	I2C_Stop();
}

void RTC_GetFecha(Fecha_t *fecha) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_DAYS);
	I2C_ReStart();
	I2C_Write(DS3231_READ);
	fecha->Day = (I2C_Read(NACK)) & MASK_DAY;
	fecha->Month = (I2C_Read(NACK)) & MASK_MES;
	fecha->Year = (I2C_Read(ACK)) & MASK_YEAR;
	I2C_Stop();
}

void RTC_GetTime(RTC_t *rtc) {
	RTC_GetHora(&rtc->hora);
	RTC_GetFecha(&rtc->fecha);
}

void RTC_SetTime(RTC_t *rtc) {
	RTC_SetHora(&rtc->hora);
	RTC_SetFecha(&rtc->fecha);
}