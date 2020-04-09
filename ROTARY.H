#ifndef __ROTARY_H__
#define __ROTARY_H__
#include "MPC82N.H" 
#include "MAIN.H"
sbit ROTA	= P3^6;  // Rotary Switch
sbit ROTB   = P3^7;
sbit ROTP   = P3^2;

void ROT_SCAN(unsigned int DelN); 
void ROT_LEFT(void);
void ROT_RIGHT(void); 
void indicateSetData(void);
#endif