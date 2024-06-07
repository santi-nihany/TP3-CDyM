/*
 * DHT11.h
 *
 * Created: 3/6/2024 14:53:54
 *  Author: Barcala
 */ 
void DHT11_init();
uint8_t DHT11_read(float *dht_temperatura, float *dht_humedad);