/*!
\file   seguridad.c
\date   2022-06-28
\author Fulvio Vivas <fyvivas@unicauca.edu.co>
\brief  Security House.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2022. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/
#include <xc.h>
#include <pic16f887.h>  /*Header file PIC16f887 definitions*/
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "lcd.h"
#include "keypad.h"
#include "sensor.h"
#include "delay_lms.h"
//Fila salida del micro entrada del teclado
//Columna entrada del micro salida del teclado



int value_temperature;
int value_potenciometro;
int value_protocell;
char buffer[17];

#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistorvoid setup()

void adc_begin(void);
int adc_conversion(int channel);
int convertir_temperature(int temperature);



enum State
{
    seguridad,
    monitoreo,
    bloqueado,
    alarma
};

enum State Estado;

void function_monitoreo(void);
void function_alarma(void);
void function_seguridad(void);
void function_blocked(void);

const char password[5] ={'1','2','3','4',0};
char pass_user[5];
unsigned char idx = 0;
unsigned char bro = 0;

    
unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall = 0;
unsigned char var_sensor_metal = 0;

char buffer [17];

void main() {

    //OSCCON = 0b01110001;
    OSCCON = 0x71;
    TRISD = 0x00;// salida puerto D
    TRISA0 = 1;
    ANSEL = 0x01; //  Configura el Puerto como Entrada Anal?gica.
    
    adc_begin();
    LCD_Init();
    LCD_String_xy(0,0,"cond ambientales");
    __delay_ms(2000); 
    
   
    
    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71; //Configura oscilador interno (FOSC = 8Mhz)
    TRISE=0;
    LCD_Init(); //Inicializa el LCD

    keypad_init(); //Inicializa el keypad
    TRISC=0x07;
    
    var_sensor_ir = SENSOR_IR;
    var_sensor_hall = SENSOR_HALL;
    var_sensor_metal = SENSOR_METAL;
   
    Estado = seguridad;
    
    while (1) {
        if(Estado == seguridad){
           function_seguridad();
        }
        else if(Estado == monitoreo){
           function_monitoreo;
        }
        else if(Estado == bloqueado){
           function_blocked();
        }
        else if(Estado == alarma){
           function_alarma();
        }
        
         value_potenciometro=adc_conversion(0);
        value_temperature=adc_conversion(1);
        int tempcelsius=convertir_temperature(value_temperature);
        value_protocell=adc_conversion(4);  
        int n=sprintf(buffer,"ir=%d,h1=%d,mt=%d",value_potenciometro,tempcelsius,value_protocell);         
            LCD_String_xy(1,0,buffer); 
            __delay_ms(2000); 
            LCD_Clear();
        
        
    }
}



void function_seguridad(void){
    char key = '0';
    LCD_Clear();
    LCD_String_xy(0,0,"password pls :D");
        LCD_Command(0xC0); 
        do{
            key = keypad_getkey();
            if(key != 0){
                LCD_Char('*');/* display pressed key on LCD16x2 */
                //LCD_Char(key);/* display pressed key on LCD16x2 */
                pass_user[idx++] = key;
            }
            __delay_ms(100);
        }while(idx < 4);
        
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"welcome");
            PORTE = 0x02;
            __delay_ms(2000);      
            Estado=monitoreo;
            idx=0;
        }
        else if(bro<3){
           
            LCD_Clear();
            LCD_String_xy(0,0,"try again");
            PORTE = 0x01;
            __delay_ms(2000);
            bro++;
            idx=0;
            }
        else if(bro==3){
            idx = 0;
            Estado=bloqueado;

        }
}

void function_blocked(void){
            LCD_Clear();
            LCD_String_xy(0,0,"blocked");
            __delay_ms(5000);
            PORTE = 0x04; 
            Estado=seguridad;
}
void function_monitoreo(void){
    if(monitoring_sensor() == 1){
            int n=sprintf(buffer,"sir=%d,sh1=%d,smt=%d",var_sensor_ir,var_sensor_hall,var_sensor_metal);
            LCD_Clear();
            LCD_String_xy(0,0,buffer); 
            __delay_ms(2000);
            Estado = alarma;
                   
        }
}

void function_alarma(void){
    
   for(int i=0; i<5; i++){ 
    PORTE=0x04;
    __delay_ms(500);
    PORTE=0x00;
    __delay_ms(500);
   } 
    Estado=monitoreo;
}




