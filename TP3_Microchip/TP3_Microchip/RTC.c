#include "RTC.h"

uint8_t bcd_to_dec(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}

uint8_t dec_to_bcd(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}

void RTC_Init() {
	I2C_Init();
}

uint8_t DS3231_GetReg(uint8_t address) {
	uint8_t ret;
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(address);
	I2C_ReStart();
	I2C_Write(DS3231_READ);
	ret = I2C_Read(NACK);
	I2C_Stop();
	return ret;
}

void DS3231_SetReg(uint8_t address, uint8_t val) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(address);
	I2C_Write(val);
	I2C_Stop();
}

void RTC_SetHora(Hora_t *hora) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_SECONDS);
	I2C_Write(dec_to_bcd(hora->Second) & MASK_SEC);
	I2C_Write(dec_to_bcd(hora->Minute) & MASK_MIN);
	I2C_Write(dec_to_bcd(hora->Hour) & MASK_HORA);
	I2C_Stop();
}

void RTC_SetFecha(Fecha_t *fecha) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_DAYS);
	I2C_Write(dec_to_bcd(fecha->Day) & MASK_DAY);
	I2C_Write(dec_to_bcd(fecha->Month) & MASK_MES);
	I2C_Write(dec_to_bcd(fecha->Year) & MASK_YEAR);
	I2C_Stop();
}

void RTC_GetHora(Hora_t *hora) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_SECONDS);
	I2C_ReStart();
	I2C_Write(DS3231_READ);
	hora->Second = bcd_to_dec(I2C_Read(ACK) & MASK_SEC);
	hora->Minute = bcd_to_dec(I2C_Read(ACK) & MASK_MIN);
	hora->Hour = bcd_to_dec(I2C_Read(NACK) & MASK_HORA);
	I2C_Stop();
}

void RTC_GetFecha(Fecha_t *fecha) {
	I2C_Start();
	I2C_Write(DS3231_WRITE);
	I2C_Write(DS3231_DAYS);
	I2C_ReStart();
	I2C_Write(DS3231_READ);
	fecha->Day = bcd_to_dec(I2C_Read(ACK) & MASK_DAY);
	fecha->Month = bcd_to_dec(I2C_Read(ACK) & MASK_MES);
	fecha->Year = bcd_to_dec(I2C_Read(NACK) & MASK_YEAR);
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