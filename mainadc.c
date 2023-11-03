#include    <xc.h>
#include    <math.h>
#include    "mainadc.h"
#include    <stdio.h>
#include    <string.h>
#include    "lcd.h"

#define	beta1 1024

void adc_begin(void) {
    ADCON1bits.ADFM = 1; //  Justificaci?n Izquierda (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; //  Selecciona Voltajes de Referencia (5v-0v). 
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversi?n Fosc/8.
    
}

int adc_conversion(int channel){
    
    ADCON0bits.CHS=(0x0f & channel);
    ADCON0bits.ADON = 1; //  Habilita el M?dulo AD.
        __delay_us(30);
        ADCON0bits.GO = 1; //  Inicia la COnversi? AD.
        while (ADCON0bits.GO); //  Espera a que termine la conversi?n AD.
        ADCON0bits.ADON = 0; //  Habilita el M?dulo AD.
        return ((ADRESH << 8) | ADRESL);  
}

int convertir_temperature(int temperature){
    
    long a =1023 - temperature;
    //the calculating formula of temperature
    float tempC = (float)(beta1 /(log((1025.0 * 10 / a - 10) / 10) + beta1 / 298.0) - 273.0);
    float tempF = (float)(tempC + 273.15);
    return (int)(tempC);
}





