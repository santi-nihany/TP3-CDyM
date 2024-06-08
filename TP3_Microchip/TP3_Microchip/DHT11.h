/*
 * DHT11.h
 *
 * Created: 3/6/2024 14:53:54
 *  Author: Barcala
 */ 
void DHT11_init();
uint8_t DHT11_read(uint8_t *temperatura_int, uint8_t *temperatura_dec, uint8_t *humedad_int, uint8_t *humedad_dec);