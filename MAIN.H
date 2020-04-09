/*******************************************
      MAIN.H 
*******************************************/
#ifndef __MAIN_H__
#define __MAIN_H__
#include "MPC82N.H" 

sbit BEEP   = P3^4;

void BEEP_PRO(void);
void MENU_DISP(void);
void TIME_INC(void);
void TIME_DISP(void);
void KEY_PRO(void);
void cursorShift(void);
void printDate(void);
void printCSIE(void);
void printCSIE2(unsigned char column);
void printCSIEShift(void);
void printGreen(void);
void printId(void);
void K_DISP(void);
void K_DISP2(void);
void printCalendar(void);
void increaseTime(void);
void setCalendar(void);
void assignCalendarData(void);
void counterUp(void);
void counterDown(void);
void charShiftCW(unsigned char c);
void charShiftCCW(unsigned char c);
void kCounter(void);
void shiftStrCW(unsigned char row, unsigned char col, unsigned char *str, unsigned char n);
void shiftStrCCW(unsigned char row, unsigned char col, unsigned char *str, unsigned char n);
void strShiftR(unsigned char row, unsigned char col, unsigned char *str, unsigned char n);

#endif
/*******************************************
      ROTARY.H 
*******************************************/
/*sbit ROTA	= P3^6;  // Rotary Switch
sbit ROTB   = P3^7;
sbit ROTP   = P3^2;

void ROT_SCAN(unsigned int DelN); 
void ROT_LEFT(void);
void ROT_RIGHT(void); */

/*******************************************
      LCD.H 
*******************************************/
/*sbit RS       = P0^0; // P00:RS=0指令暫存器，RS=1資料暫存器
sbit EN       = P0^1; // P01:EN=0禁能LCD，EN=1致能LCD
sfr  DATA_BUS = 0x80; // DATA BUS=P04~P07

void CGRAM(void);
void Init_LCD(void);
void LCD_CmdI(unsigned char Comm);
void LCD_Cmd(unsigned char Comm);
void Send_Data(unsigned char Data);
void Delay(unsigned int Del);
void DLY(unsigned char dly);*/
