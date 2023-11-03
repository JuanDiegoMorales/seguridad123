/* 
 * File:   mainadc.h
 * Author: WINDOWS
 *
 * Created on 27 de octubre de 2023, 03:14 PM
 */

#ifndef MAINADC_H
#define	MAINADC_H

#ifdef	__cplusplus
extern "C" {
#endif

void adc_begin(void);
int adc_conversion(int channel);
int convertir_temperature(int temperature);



#ifdef	__cplusplus
}
#endif

#endif	/* MAINADC_H */

