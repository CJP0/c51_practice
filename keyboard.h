#ifndef __KETBOARD_H__
#define __KEYBOARD_H__
#include "MPC82N.H" 
#include "LCD.H"

sbit COLUMN1 = P2^0;
sbit COLUMN2 = P2^1;
sbit COLUMN3 = P2^2;
sbit COLUMN4 = P2^3;	
sbit ROW1	 = P2^4;
sbit ROW2	 = P2^5;
sbit ROW3	 = P2^6;
sbit ROW4	 = P2^7;

void SCAN_ON(void);
void SCAN_OFF(void);

#endif