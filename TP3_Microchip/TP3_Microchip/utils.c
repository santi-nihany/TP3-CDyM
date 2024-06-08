#include "utils.h"
#include <string.h>
void escribir(uint8_t * msj1, uint8_t * msj2){
    LCDGotoXY(0, 0);
    LCDstring(msj1, strlen(msj1));
    LCDGotoXY(0, 1);
    LCDstring(msj2, strlen(msj2));
}

void Sol_error(void){
	LCDclr();
	LCDGotoXY(0, 0);
	LCDstring("Cargando ...", 12);
	LCDclr();
}