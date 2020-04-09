/*******************************************
      ADC.C 
*******************************************/
#include "ADC.H" 

unsigned char code MSGD[] = " MPC80G516 ADC0 ";
extern unsigned char bdata g;
extern bit INIT_FLAG;
extern unsigned int  ADC16;

void Init_ADC(void)
{
	P1M0=0x21;  P1M1=0;	     // P1.0, P1.5 Input Only
	AUXR=0x00;	ADCTL=0x60;   // High Speed, ADRJ=0  
}

void V_ADC0(void)
{	
	float temp ;

	ADCTL = ADCTL+ADCON+ADCS; // ADC CH0 Start Convert 
	while (!(ADCTL & ADCI));  // Wait ADC Complete
	ADCTL = 0x60;             // ADC Convert OK, Stop ADC
	ADC16 = (ADCH<<2) + ADCL;
	ADCTL =ADCTL+ADCON+ADCS;  // ADC CH0 Start Convert 
	while (!(ADCTL & ADCI));  // Wait ADC Complete
	ADCTL = 0x60;             // ADC Convert OK, Stop ADC
	temp = ADC16 + (ADCH<<2) + ADCL;
   ADC16 = temp; ADC16>>=1;
}

void V_ADC5(void)
{	
	float temp ;

	ADCTL = ADCTL + ADCON + ADCS + 5; // ADC CH5 Start Convert 
	while (!(ADCTL & ADCI));  // Wait ADC Complete
	ADCTL = 0;                // ADC Convert OK, Stop ADC
	ADC16 = (ADCH<<2) + ADCL;

	ADCTL = ADCTL + ADCON + ADCS + 5; // ADC CH5 Start Convert 
	while (!(ADCTL & ADCI));  // Wait ADC Complete
	ADCTL = 0;                // ADC Convert OK, Stop ADC
	temp = ADC16 + (ADCH<<2) + ADCL;

   ADC16 = temp; ADC16>>=1;
}

void ADC_HEX_DISPLAY(void) {			
	unsigned int i,HEX;
	if(INIT_FLAG){
		LCD_Cmd(1); Delay(5000); LCD_Cmd(0x0C);
		for (i=0; i<16; i++) Send_Data(MSGD[i]);
	}
	V_ADC0();
	LCD_Cmd(0xC6);
	HEX=ADC16; 	
	i=HEX>>8;
	if (i>=0x0A) i+=7;
	i+=0x30; Send_Data(i);
	i=(HEX>>4)&0x0F;
	if (i>=0x0A) i+=7;
	i+=0x30; Send_Data(i);
	i=HEX&0x0F;
	if (i>=0x0A) i+=7;
	i+=0x30; Send_Data(i);	
	Send_Data(' ');
}
	
void adcHexToVolPrint(void){
	float temp;
	V_ADC0();
	temp=ADC16*5.0/1023.0;
	printNumberF(0, 0, 5, temp, 3);
	LCD_Cmd(0x85);
	Send_Data('V');
}

void adcBinPrint(void){
	V_ADC0();
	printNumberBin(0, 0, 15, ADC16);
}

void adcBCDprint(void){
	V_ADC0();
	printNumber2(0, 0, 5, ADC16);
}
