#ifndef __LCD_H__
#define __LCD_H__
#include "MPC82N.H"
sbit RS       = P0^0; // P00:RS=0指令暫存器，RS=1資料暫存器
sbit EN       = P0^1; // P01:EN=0禁能LCD，EN=1致能LCD
sfr  DATA_BUS = 0x80; // DATA BUS=P04~P07

void CGRAM(unsigned char *str, int start, int n);
void Init_LCD(void);
void LCD_CmdI(unsigned char Comm);
void LCD_Cmd(unsigned char Comm);
void Send_Data(unsigned char Data);
void Delay(unsigned int Del);
void DLY(unsigned char dly);
void printStr(unsigned char row, unsigned char column, unsigned char n, unsigned char *str);
//void printStr2(unsigned char row, unsigned char column, unsigned char start, unsigned char n, unsigned char *str);
void printStr2(unsigned char row, unsigned char col, unsigned char start, unsigned char end, unsigned char *str);
void indicateAndPrint(unsigned char row, unsigned char column, unsigned char c);
void creatCharAndPrint(unsigned char *str, unsigned char n, unsigned char row, unsigned char column);
//void clearN(unsigned char row, unsigned char column, unsigned char n);
void printNumber(unsigned char row, unsigned char cloumn, unsigned char len, int n);
void printNumber2(unsigned char row, unsigned char column, unsigned char len, long n);
void printNumberBin(unsigned char row, unsigned char column, unsigned char len, unsigned int n);
void printNumberF(unsigned char row, unsigned char column, unsigned char len, float n, unsigned char point);
void printNumber3(unsigned char row, unsigned char column, int n);

#endif