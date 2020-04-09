#include "keyboard.h"

extern unsigned char K_TEMP, KEY_NO;

void SCAN_ON(void)
{
	KEY_NO=0xFF;
	K_TEMP=0xFF;
	P2=0xFE;       //P20=0掃描第1行
	if (P2==0xEE)  {K_TEMP=P2; KEY_NO=0x0F; return;} 
	if (P2==0xDE)  {K_TEMP=P2; KEY_NO=0x0E; return;}  
	if (P2==0xBE)  {K_TEMP=P2; KEY_NO=0x0D; return;}   
	if (P2==0x7E)  {K_TEMP=P2; KEY_NO=0x0C; return;}  
       
	P2=0xFD;       //P21=0掃描第2行
	if (P2==0xED)  {K_TEMP=P2; KEY_NO=0x0B; return;}   
	if (P2==0xDD)  {K_TEMP=P2; KEY_NO=0x0A; return;} 
	if (P2==0xBD)  {K_TEMP=P2; KEY_NO=9; return;} 
	if (P2==0x7D)  {K_TEMP=P2; KEY_NO=8; return;}  
       
	P2=0xFB;       //P22=0掃描第3行
	if (P2==0xEB)  {K_TEMP=P2; KEY_NO=7; return;}   
	if (P2==0xDB)  {K_TEMP=P2; KEY_NO=6; return;}   
	if (P2==0xBB)  {K_TEMP=P2; KEY_NO=5; return;}  
	if (P2==0x7B)  {K_TEMP=P2; KEY_NO=4; return;}  
      
	P2=0xF7;       //P23=0掃描第4行
	if (P2==0xE7)  {K_TEMP=P2; KEY_NO=3; return;}  
	if (P2==0xD7)  {K_TEMP=P2; KEY_NO=2; return;}  
	if (P2==0xB7)  {K_TEMP=P2; KEY_NO=1; return;}  
	if (P2==0x77)  {K_TEMP=P2; KEY_NO=0; return;} 
}

void SCAN_OFF(void)
{
LOOP:	
	P2=0xF0;
	while (P2!=0xF0);
	Delay(5000);
	if (P2!=0xF0) goto LOOP;
}