/*!
\file   delay_1ms.c
\date   2020-11-13
\author Fulvio Vivas <fulvio.vivas@unicauca.edu.co>
\brief  Generating a delay of 1 ms in PIC18F4550 using Timer1.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2020. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#include <xc.h>
#include "delay_lms.h"		/* Configuration header file */

//#define _XTAL_FREQ 4000000//frecuencia del oscilador
//TMR1=65535-{delay*[fosc/(4*prescaler)]} -1

#define Pulse PORTB  		/* Define Pulse as LATB to output on PORTB */

int count1 = 0;
int count2 = 0;

void sexo(void)
{
    TRISB0 = 0;  		//pinRB0 como salida digital, se colocar? el led
    Pulse = 0;  		//El led se inicia apagado y parpadea cada 500ms
    ANSELH = 0x00;
    while(1)
    {
        Pulse=~Pulse;  		/* Toggle PortB at 500 Hz */ 
        Timer1_delay();  	/* Call delay which provide desired delay */    
    }   
}

void Timer1_delay()
{
    /* Enable 16-bit TMR1 register, No pre-scale, internal clock,timer OFF */
    T1CON=0b00000000;// timer1 pic como temporizador prescaler de 8
    TMR1=0xfc16;//se inicia a este valor para obtener 1ms  
    T1CONbits.TMR1ON=1;  	/* Turn ON Timer1 */

    while(PIR1bits.TMR1IF==0); 	/* Wait for Timer1 overflow interrupt flag */
    TMR1ON=0;  			/* Turn OFF timer */
    TMR1IF=0;  			/* Make Timer1 overflow flag to '0' */
      
}

void Timer_IRQ()
{  
      if(TMR1 == 0xffff){
        count1++;
        count2++;
        Pulse = ~Pulse;    /* Toggle Value at PortB to generate waveform of 500 Hz */   
        PIR1bits.TMR1IF=0; /* Make Timer1 Overflow Flag to '0' */
      }
       if(count1 == 50){

        count1=0; 
       }
        if(count2 == 30){
        count2=0;  
       }
}