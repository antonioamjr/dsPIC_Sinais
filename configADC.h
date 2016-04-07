/* 
 * File:   configADC.h
 * Author: Antonio
 *
 * Created on 10 de Abril de 2014, 14:36
 */

#ifndef CONFIGADC_H
#define	CONFIGADC_H

#include <xc.h>
#include <libpic30.h>

#define NUMSAMP	      1024  //atualmente usando duas capturas, ver código dependente da variável
#define LOG2NUMSAMP   10
#define FCY           40000000
#define ADC_Dly_Time  (20E-6 * FCY)
#define Q15toFloat(X) ((float)X/(32768))

extern unsigned int DmaBuffer;

void initADC1(void);
void initDma0(void);
void __attribute__((__interrupt__)) _DMA0Interrupt(void);
void ProcessADCSamples(int * AdcBuffer, int * AdcBuffer2);

#endif	/* CONFIGADC_H */

