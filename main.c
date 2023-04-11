/*
Semaforo con Push Button, cambio de color por cada pulso
Estudiantes:
    Julian Emilio Gutierrez Loya
    Sebastian De la Cruz Delgado
Fecha:
    Abril / 2023
*/
//Libreria principal
#include "msp.h"

//Definiciones:
#define RETARDO (100000)
#define PIN0 (0x01)
#define PIN1 (0x02)

//Acceso a memoria, con apuntadores (bla-bla-bla*)
uint8_t *p1_in  = (uint8_t*)0x040004c00;
uint8_t *p1_out = (uint8_t*)0x040004c02;
uint8_t *p1_dir = (uint8_t*)0x040004c04;
uint8_t *p1_ren = (uint8_t*)0x040004c06;

uint8_t *p2_in  = (uint8_t*)0x040004c01;
uint8_t *p2_out = (uint8_t*)0x040004c03;
uint8_t *p2_dir = (uint8_t*)0x040004c05;
//Direcciones dadas en manual de fabricante

void main(void)
{
    //Variables locales
    uint32_t i;
    uint8_t cont_cambio = 0;
    uint_fast16_t inputPinValue;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Apuntadores y acciones
	*p1_dir &= ~0x10;  //PIN P1.4 COMO ENTRADA, LECTOR DEL PB
	*p1_ren |= 0x10;   //RESISTENCIA INTERNA
	*p1_out |= 0x10;   //PULL UP

	*p2_dir |= 0x03;  //PIN P2.0, P2.1, P2.2  COMO SALIDA, LEDS RGB
	*p2_out &= ~0x07;              // APAGADOS

	while(1)
	{
	    inputPinValue = *p1_in & (0x10);
	    if ( inputPinValue == 0)        // LEE EL REGISTRO QUE INDICA EL VALOR DE ENTRADA DE UN PIN == 0)
	    {
	    cont_cambio = cont_cambio + 1; //Contador de estado
	                                            //BANDERA EN ALTO
	        if(cont_cambio == 1)
	        {
	            *p2_out &= ~0x03;   //APAGA LEDS
	            *p2_out |= 0x02;    //RGB, VERDE en alto
	            for(i=RETARDO; i>0 ;i--);
	        }
	        else if(cont_cambio == 2)
	        {
                *p2_out &= ~0x02;   //RGB, Verde en bajo
                *p2_out |= 0x03;    //RGB, Verde y Rojo en alto
                for(i=RETARDO; i>0 ;i--);
	        }
	        else if(cont_cambio == 3)
	        {
	        *p2_out &= ~0x03;   //RGB, Verde y Rojo en bajo
	        *p2_out |= 0x01;    //RGB, Rojo en alto
	        for(i=RETARDO; i>0 ;i--);
	        cont_cambio == 1;   //Retornamos a 0... entrara al primer if de verde por la condición +1, previamente programada
	        }
	    }
	}
}
