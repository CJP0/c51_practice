#ifndef __ADC_H__
#define __ADC_H__
#include "MPC82N.H" 
#include "LCD.H"
#include "MAIN.H"
void Init_ADC(void);
void V_ADC0(void);
void V_ADC5(void);
void ADC_HEX_DISPLAY(void);
void adcHexToVolPrint(void);
void adcBinPrint(void);
void adcBCDprint(void);
#endif