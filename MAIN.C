/***********************************************  
    ROT_LCD_MB.C   2018/05/25

    16X2 LCD, P00=RS ,P01=EN ,P04~P07=DATA_BUS   
    ROTARY, P36, P37, P32 
    MPC82G516 OK
************************************************/  
#include "MAIN.H" 
#include "ROTARY.H"
#include "LCD.H"
#include "keyboard.h"
#include "ADC.H"
#define ADC_Time 10

const unsigned char menuSize = 7;
unsigned char code MSG[7][16] ={"A: STUDENT ID RL",  
                        "B: W CCW Rotate ",  
                        "C: UP Counter 4 ",
                        "D: DOWN Counter4",  
						"E ADC0 Voltage 3",  
                        "F ADC0 Decimal  ",  
                        "G ADC0 Binary   "}; 
unsigned char code csie[] = {0x00, 0x00, 0x19, 0x02, 0x08, 0x11, 0x07, 0x04,
0x00, 0x10, 0x1f, 0x09, 0x14, 0x02, 0x1c, 0x04,
0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
0x00, 0x1e, 0x08, 0x08, 0x10, 0x10, 0x00, 0x00,
0x07, 0x04, 0x07, 0x04, 0x07, 0x02, 0x04, 0x00,
0x1c, 0x04, 0x1c, 0x04, 0x1c, 0x08, 0x04, 0x00,
0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x1f, 0x00,
0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x1f, 0x00};

unsigned char code temp[] = {0x00, 0x0c, 0x1e, 0x0c, 0x06, 0x07, 0x07, 0x0b,
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x04, 0x02,
0x11, 0x01, 0x01, 0x01, 0x06, 0x1e, 0x10, 0x00,
0x10, 0x10, 0x08, 0x06, 0x03, 0x01, 0x01, 0x00,

0x00, 0x0c, 0x1e, 0x0c, 0x06, 0x07, 0x06, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08,
0x03, 0x05, 0x03, 0x04, 0x04, 0x03, 0x0f, 0x00,
0x08, 0x10, 0x10, 0x10, 0x08, 0x06, 0x0c, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08,
0x00, 0x06, 0x0f, 0x06, 0x0c, 0x1c, 0x1c, 0x1a,
0x01, 0x01, 0x02, 0x0c, 0x18, 0x10, 0x10, 0x00,
0x11, 0x10, 0x10, 0x10, 0x0c, 0x0f, 0x01, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
0x00, 0x06, 0x0f, 0x06, 0x0c, 0x1c, 0x0c, 0x18,
0x02, 0x01, 0x01, 0x01, 0x02, 0x0c, 0x06, 0x00,
0x18, 0x14, 0x18, 0x04, 0x04, 0x18, 0x1e, 0x00};
unsigned char code dateTable[] = {'2','0','1','9',0,'3',1,'2','2',2};
unsigned char code FONT[] = {0x04,0x0F,0x12,0x0F,0x0A,0x1F,0x02,0x00,  //年
					      0x1F,0x11,0x1F,0x11,0x1F,0x11,0x11,0x00,  //月
				         0x1F,0x11,0x11,0x1F,0x11,0x11,0x1F,0x00}; //日
unsigned char code id[]="3A417011";
unsigned char code MSGA[] = "TIME";

int year=2009;      //設定年
unsigned char mon=1,day=20;  //設定月、日
unsigned char hor=13,min2=32; //設定時、分值
int y,m,d;		   //陣列資料計數
unsigned char calendarSetFlag;
int setDataTemp;

unsigned char adcCounter=ADC_Time;
unsigned int  ADC16;

unsigned char bdata f;
sbit  D_FLAG   = f^0;
sbit  D_ABLE   = f^1;
sbit UP_FLAG = f^2;
sbit DOWN_FLAG = f^3;
sbit calendar_FLAG = f^4;
sbit setCalendar_FLAG = f^5;
sbit calendar_UPDATE = f^6;

unsigned char bdata g;
sbit  BE_FLAG  = g^0;
sbit INIT_FLAG = g^1;
sbit kCounter_UPDN = g^2;

unsigned char bdata  h; 
sbit FLAG	 = h^0;
sbit K_FLAG  = h^1;
sbit B_FLAG  = h^2;
sbit Dh_FLAG  = h^3;
sbit F_FLAG  = h^4;
sbit E_FLAG = h^5;
sbit A_FLAG = h^6;
sbit N_FLAG = h^7;

unsigned char bdata k;
sbit ADC_Hex = k^0;
sbit ADC_Vol = k^1;
sbit ADC_Bin = k^2;
sbit ADC_BCD = k^3;
sbit ADC_update = k^4;
sbit charShift_FLAG = k^5;
sbit kCounter_FLAG = k^6;

unsigned char OLD,MENUPTR,B_CNT;
unsigned char SECOND,HOUR,MIN,SEC, SECOND2, sec2;
unsigned char K_TEMP, KEY_NO, LCD_CNT;
unsigned char SECOND3 = 50;
int counter = 0, counterStart = 0, counterEnd = 9999;
long keyCounter=0, keyCounterStart=0, keyCounterEnd=99999;

void ROTP_INT(void) interrupt 0 using 1
{
	BEEP_PRO();	
	if(setCalendar_FLAG)
		{ assignCalendarData(); ++calendarSetFlag; indicateSetData(); return;}
	k=0;
  	switch (MENUPTR) {
		case 0:
			h = 0; f = 0; Dh_FLAG=1;
			/*counter = counterStart;
			SECOND3 = 50;
			h = 0; f = 0; UP_FLAG = 1;*/
			break;
		case 1: 
			h = 0; f = 0; charShift_FLAG=1;
			break;
		case 2: 	
			h = 0; f = 0; UP_FLAG=1;
			//h = 0; f = 0; calendar_FLAG = 1; calendar_UPDATE = 1;
			break;
		case 3:
			h = 0; f = 0; DOWN_FLAG=1;
			//h = 0; f = 0; B_FLAG=1;
			break;
		case 4:
			h = 0; f = 0; ADC_Vol=1;
			//h = 0; f = 0; E_FLAG=1;
		   break;
		case 5:
			h = 0; f = 0; ADC_BCD = 1;
			//h = 0; f = 0; F_FLAG=1;
			break;
		case 6:
			h = 0; f = 0; ADC_Bin =1;
			//h = 0; f = 0; setCalendar_FLAG = 1;
			break;
		case 7:
			//h = 0; f = 0; ADC_Hex=1;
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		default:
			break;
	}
	INIT_FLAG = 1;
	while (ROTP==0);
}

void T0_INT(void) interrupt 1 using 2 
{
	B_CNT--;
	if (B_CNT==0) {BE_FLAG=0; TR0=0; BEEP=1;}
}

void KEY_INT(void) interrupt 2 using 3
{
	unsigned char COD_TMP;
	SCAN_ON();
	if  (K_TEMP==0xFF) {
		P2=0xF0; return;
	}
	COD_TMP=K_TEMP;
	Delay(5000);
	SCAN_ON();
	if  (K_TEMP==0xFF) {
		P2=0xF0; return;
	}
	if  (COD_TMP!=K_TEMP) {
		KEY_NO=0xFF; P2=0xF0; return;
	}
	SCAN_OFF();
	KEY_PRO();
}

void T2_INT(void) interrupt 5 using 3
{
	TF2=0;
	--SECOND;
	--SECOND2;
	--SECOND3;
	--adcCounter;
	if (SECOND && SECOND2 && SECOND3 && adcCounter) return;
	if(SECOND == 0){
		SECOND=10; TIME_INC();
	}
	if(SECOND2 == 0){
		SECOND2=100; 
		if(setCalendar_FLAG == 0) increaseTime();
	}
	if(SECOND3 == 0){
		SECOND3 = 30;
		if(UP_FLAG) counterUp();
		if(DOWN_FLAG) counterDown();
		if(kCounter_FLAG) kCounter();
	}
	if(adcCounter == 0){
		adcCounter=ADC_Time;
		if(ADC_Hex || ADC_Vol || ADC_Bin || ADC_BCD){
			ADC_update=1;
		}
	}
	if (D_ABLE==1) D_FLAG=1;
	else D_FLAG=0;
} 

void MAIN(void)
{
	unsigned char j,k;
	EN=0;	f=0; g=0; h=0; 	
 	TMOD=2; TH0=0; TL0=0;
	SECOND=10; // SECOND=100, 1秒
	SECOND2=100;
	T2CON=0;   // T2 16 BIT Timer 
              // Fosc=22.1184MHz，Timer時脈=Fosc/12=1.8432MHz
              // Timer延時時間=(1/1.8432MHz)*18432=10000uS=10ms  
	TH2=(65536-18432)>>8; TL2=(65536-18432); TR2=1; // 10ms 
	RCAP2H=(65536-18432)>>8; RCAP2L=(65536-18432); 

	Init_LCD(); 
	j=ROTA; k=ROTB; OLD=k*2+j; 
	MENUPTR=0; MENU_DISP();
	HOUR=0; MIN=0; SEC=0;
	EX0=1; IT0=1;      // ROTARY PUSH BUTTON
	ET0=1; PT0=1;  
	ET2=1; PT2=1;
	P2=0xF0; EX1=1;
	EA=1;
	
	kCounter_UPDN = 1;
	while (1) {
		ROT_SCAN(10);
		if (D_FLAG==1) TIME_DISP(); 
		else if(calendar_UPDATE) printCalendar();
		else if(setCalendar_FLAG) setCalendar();
		else if(UP_FLAG) printNumber2(0, 0, 4, counter);
		else if(DOWN_FLAG) printNumber2(0, 0, 4, counter);
		else if(B_FLAG) cursorShift(); 
		else if(Dh_FLAG) printId();
		else if(F_FLAG) printGreen();
		else if(E_FLAG) printCSIE();
		else if(A_FLAG) printDate();
		else if(N_FLAG) K_DISP2();
		else if(charShift_FLAG) charShiftCCW('w');
		else if(kCounter_FLAG) printNumber2(0, 0, 7, keyCounter);
		else if(ADC_update && ADC_Hex) {
			ADC_HEX_DISPLAY();
			ADC_update=0;
		}
		else if(ADC_update && ADC_Vol) {
			adcHexToVolPrint();
			ADC_update=0;
		}
		else if(ADC_update && ADC_Bin) {
			adcBinPrint();
			ADC_update=0;
		}
		else if(ADC_update && ADC_BCD) {
			adcBCDprint();
			ADC_update=0;
		}
	}
}

void BEEP_PRO(void)
{
	B_CNT=0; BE_FLAG=1; 
	BEEP=0; TR0=1;
}

void MENU_DISP(void)
{
	unsigned char i;
	f = 0; k = 0;
	LCD_Cmd(0x80);	
	for (i=0; i<16; i++) Send_Data(MSG[MENUPTR%menuSize][i]); 
	LCD_Cmd(0xC0);	
	for (i=0; i<16; i++) Send_Data(MSG[(MENUPTR+1)%menuSize][i]); 
}

void TIME_INC(void)
{
	SEC+=1; 
	if (SEC < 60) return; 
	SEC=0; MIN+=1;
	if (MIN < 60) return;
	MIN=0; HOUR+=1;
	if (HOUR < 24) return;
	HOUR=0;	
}

void TIME_DISP(void)
{
	unsigned char i,j;
	if(INIT_FLAG){
		LCD_Cmd(0x01); Delay(5000);  
		LCD_Cmd(0x0C); LCD_Cmd(0x86);
  		for (i=0; i<4; i++) Send_Data(MSGA[i]);  
		LCD_Cmd(0xC4); 	 
		INIT_FLAG = 0;
	}
	LCD_Cmd(0xC4); LCD_Cmd(0x0C);

	j=HOUR; i=j/10; 
	if (i==0) i=0x20;
	else i+=0x30;
	Send_Data(i);
	j%=10; j+=0x30;
	Send_Data(j); Send_Data(':');

	j=MIN; i=j/10; 
	if (i==0) i=0x20;
	else i+=0x30;
	Send_Data(i);
	j%=10; j+=0x30;
	Send_Data(j); Send_Data(':');

	j=SEC; i=j/10; 
	if (i==0) i=0x20;
	else i+=0x30;
	Send_Data(i);
	j%=10; j+=0x30;
	Send_Data(j);
	D_FLAG=0;
}

void counterUp(void){
	++counter;
	if(counter>counterEnd)
		counter = counterStart;
}

void counterDown(void){
	--counter;
	if(counter<counterStart)
		counter = counterEnd;
}

void K_DISP(void)
{
	unsigned char TEMP;
	B_FLAG=0;
	if (K_FLAG==0) {
  		LCD_Cmd(0x01); Delay(5000);
		LCD_Cmd(0x0F); 
		LCD_Cmd(0x6); 
		FLAG=0; K_FLAG=1;
		LCD_CNT=16;					
	}
 	KEY_NO |= 0x30; 
	TEMP=KEY_NO;
	Send_Data(TEMP); LCD_CNT--;
	if (LCD_CNT!=0) return;
	FLAG=!FLAG;
	if (FLAG==1) {
		LCD_Cmd(0xC0); LCD_CNT=16;
	}	
	else {
   	LCD_Cmd(0x01); Delay(5000);
		LCD_CNT=16;				
	}			
}

void K_DISP2(void)
{
	static unsigned char lcdBuffer[16];
	unsigned char TEMP;
	N_FLAG = 0;
	if (K_FLAG==0) {
  		LCD_Cmd(0x01); Delay(5000);
		LCD_Cmd(0x0F); 
		LCD_Cmd(0x6); 
		FLAG=0; K_FLAG=1;
		LCD_CNT=16;					
	}
 	KEY_NO |= 0x30; 
	TEMP=KEY_NO;
	Send_Data(TEMP); 
	if(FLAG)
		lcdBuffer[16-LCD_CNT]=TEMP;
	LCD_CNT--;
	if (LCD_CNT!=0) return;
	if (FLAG) {
		LCD_Cmd(0x01); Delay(5000);
		printStr(0, 0, 16, lcdBuffer);
		LCD_Cmd(0xC0);
		LCD_CNT=16;
	}	
	else {
		LCD_Cmd(0xC0); LCD_CNT=16;
		FLAG=1;				
	}			
}

void printId(void){
	unsigned char i, j;
	if(INIT_FLAG){
		LCD_Cmd(0x01); Delay(5000);
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	printStr(1, 0, 8, id);
	for (j=10; j>0; j--) Delay(65000);
	while(1){ 
		for(i=0;i<8;i++){
			/*LCD_Cmd(0x1C);*/
			ROT_SCAN(1000000);
			if(Dh_FLAG==0) return;
			indicateAndPrint(1, i, ' ');
			if(Dh_FLAG==0) return;
			printStr(1, i+1, 8, id);
			for (j=10; j>0; j--) Delay(65000);
		}
		for(i=0;i<8;i++){
			/*LCD_Cmd(0x18);*/
			ROT_SCAN(1000000);
			if(Dh_FLAG==0) return;
			printStr(1, 7-i, 8, id);
			if(Dh_FLAG==0) return;
			indicateAndPrint(1, 15-i, ' ');
			for (j=10; j>0; j--) Delay(65000);
		}
	}
}

void printGreen(void){
	unsigned char i, j;
	if(INIT_FLAG){
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	CGRAM(temp, 8, 8);
	for(i=0;i<14;i+=2){
		LCD_Cmd(0x01);
		Delay(5000);
		if(F_FLAG==0) return;
		indicateAndPrint(0, i, 0);
		indicateAndPrint(0, i+1, 1);
		indicateAndPrint(1, i, 2);
		indicateAndPrint(1, i+1, 3);
		ROT_SCAN(1000000);
		if(F_FLAG==0) return;
		for (j=10; j>0; j--) Delay(65000);
		if(F_FLAG==0) return;
		LCD_Cmd(0x01);
		Delay(5000);
		if(F_FLAG==0) return;
		indicateAndPrint(0, i+1, 4);
		indicateAndPrint(0, i+2, 5);
		indicateAndPrint(1, i+1, 6);
		indicateAndPrint(1, i+2, 7);
		ROT_SCAN(1000000);
		if(F_FLAG==0) return;
		for (j=10; j>0; j--) Delay(65000);
		if(F_FLAG==0) return;
	}
	CGRAM(temp, 0, 8);
	for(i=14;i>0;i-=2){
		LCD_Cmd(0x01);
		Delay(5000);
		if(F_FLAG==0) return;
		indicateAndPrint(0, i, 0);
		indicateAndPrint(0, i+1, 1);
		indicateAndPrint(1, i, 2);
		indicateAndPrint(1, i+1, 3);
		ROT_SCAN(1000000);
		if(F_FLAG==0) return;
		for (j=10; j>0; j--) Delay(65000);
		LCD_Cmd(0x01);
		Delay(5000);
		if(F_FLAG==0) return;
		indicateAndPrint(0, i-1, 4);
		indicateAndPrint(0, i, 5);
		indicateAndPrint(1, i-1, 6);
		indicateAndPrint(1, i, 7);
		ROT_SCAN(1000000);
		if(F_FLAG==0) return;
		for (j=10; j>0; j--) Delay(65000);
		if(F_FLAG==0) return;
	}
	
}

void printCSIE(void){
	unsigned char j;
	CGRAM(csie, 0, 8);
	LCD_Cmd(0x01);
	Delay(5000);
	LCD_Cmd(0x0C);
	LCD_Cmd(0x80);
	for(j=0;j<4;j++){
		Send_Data(j);
	}
	LCD_Cmd(0xc0);
	for(j=4;j<8;j++)
		Send_Data(j);
	E_FLAG=0;
}

void printCSIE2(unsigned char column){
	unsigned char i;
	if(INIT_FLAG){
		CGRAM(csie, 0, 8);
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	LCD_Cmd(0x01);
	Delay(5000);
	LCD_Cmd(0x80+column);
	for(i=0;i<4;i++)
		Send_Data(i);
	LCD_Cmd(0xc0+column);
	for(i=4;i<8;i++)
		Send_Data(i);
	ROT_SCAN(1000000);
}

void printCSIEShift(void){
	unsigned char i=0;
	while(i<13){
		printCSIE2(i++);
		if(!E_FLAG)
			return;
	}
	while(i){
		printCSIE2(--i);
		if(!E_FLAG)
			return;
	}
}

void printDate(void){
	unsigned char i;
	CGRAM(FONT, 0, 3);
	LCD_Cmd(0x01);  // 清除顯示幕   
	Delay(5000);
   	LCD_Cmd(0x0C);
   	LCD_Cmd(0x83);
   	for (i=0 ; i<10 ; i++)
       	Send_Data(dateTable[i]);
	A_FLAG=0;
}

void cursorShift(void){
	unsigned char i, j;
	LCD_Cmd(0x01);  // 清除顯示幕   
	Delay(5000);
	LCD_Cmd(0x0E);
	LCD_Cmd(0x80);	
	for (j=10; j>0; j--) Delay(65000);
	ROT_SCAN(1000000);
	if (B_FLAG==0) return;  
	for (i=15; i>0; i--) {
		LCD_Cmd(0x14);	
		for (j=10; j>0; j--) Delay(65000);
		ROT_SCAN(1000000);
		if (B_FLAG==0) return;  				
	} 	 
	LCD_Cmd(0xCF);	
	for (j=10; j>0; j--) Delay(65000);
	if (B_FLAG==0) return;  
	for (i=15; i>0; i--) {
		LCD_Cmd(0x10);	
		for (j=10; j>0; j--) Delay(65000);
		ROT_SCAN(1000000);
		if (B_FLAG==0) return;  
	}
}

void charShiftCCW(unsigned char c){
	unsigned char j, position = 0xC0;
	LCD_Cmd(0x01);  // 清除顯示幕   
	Delay(5000);
	LCD_Cmd(0x0C);
	LCD_Cmd(position);	
	Send_Data(c);
	for (j=10; j>0; j--) Delay(65000);
	ROT_SCAN(1000000);
	if (charShift_FLAG==0) return;  
	while (position<0xCF) {
		LCD_Cmd(position++);
		Send_Data(' ');
		LCD_Cmd(position);
		Send_Data(c);
		for (j=10; j>0; j--) Delay(65000);
		ROT_SCAN(1000000);
		if (charShift_FLAG==0) return;  				
	} 	 
	LCD_Cmd(0xCF);		
	Send_Data(' ');
	LCD_Cmd(0x8F);
	Send_Data(c);
	position = 0x8F;
	for (j=10; j>0; j--) Delay(65000);
	if (charShift_FLAG==0) return;  
	while (position>0x80) {
		LCD_Cmd(position--);
		Send_Data(' ');
		LCD_Cmd(position);
		Send_Data(c);
		for (j=10; j>0; j--) Delay(65000);
		ROT_SCAN(1000000);
		if (charShift_FLAG==0) return;  
	}
}

void charShiftCW(unsigned char c){
	unsigned char j, position = 0x80;
	LCD_Cmd(0x01);  // 清除顯示幕   
	Delay(5000);
	LCD_Cmd(0x0C);
	LCD_Cmd(position);	
	Send_Data(c);
	for (j=10; j>0; j--) Delay(65000);
	ROT_SCAN(1000000);
	/*if (wShift_FLAG==0) return;  */
	while (position<0x8F) {
		LCD_Cmd(position++);
		Send_Data(' ');
		LCD_Cmd(position);
		Send_Data(c);
		for (j=10; j>0; j--) Delay(65000);
		ROT_SCAN(1000000);
		//if (wShift_FLAG==0) return;  				
	} 	 
	LCD_Cmd(0x8F);		
	Send_Data(' ');
	LCD_Cmd(0xCF);
	Send_Data(c);
	position = 0xCF;
	for (j=10; j>0; j--) Delay(65000);
	//if (wShift_FLAG==0) return;  
	while (position>0xC0) {
		LCD_Cmd(position--);
		Send_Data(' ');
		LCD_Cmd(position);
		Send_Data(c);
		for (j=10; j>0; j--) Delay(65000);
		ROT_SCAN(1000000);
		//if (wShift_FLAG==0) return;  
	}
}

void shiftStrCW(unsigned char row, unsigned char col, unsigned char *str, unsigned char n){
	unsigned char temp = n;
	LCD_Cmd(0x01);
	Delay(5000);
	LCD_Cmd(0x0C);
	printStr(row, col, n, str);
	/*while(col+n<17){
		indicateAndPrint(row, col++, ' ');
		printStr(row, col, n, str);
	}
	while(col<16){
		indicateAndPrint(row, col++, ' ');
		printStr(row, col, n-(col+n-16), str);
		printStr2(1, 16-(col+n-16), n-1, n-(col+n-16)-1, str);
	}*/
	while(col<16){
		indicateAndPrint(row, col++, ' ');
		if(col>15) break;
		if(col+n<17)
			printStr(row, col, n, str);
		else{
			printStr(row, col, 16-col, str);
			printStr2(1, 16-(col+n-16), n-1, n-(col+n-16)-1, str);
		}
	}
	while(col>1){
		indicateAndPrint(1, --col, ' ');
		if(col-n>=0)
			printStr2(1, col-n, n-1, 0, str);
		else{
			printStr2(1, 0, col-1, 0, str);
			printStr2(0, 0, col, n-1, str);
		}
	}
}

void shiftStrCCW(unsigned char row, unsigned char col, unsigned char *str, unsigned char n){
	unsigned char temp = n;
	LCD_Cmd(0x01);
	Delay(5000);
	LCD_Cmd(0x0C);
	printStr(row, col, n, str);
	while(col<16){
		indicateAndPrint(row, col++, ' ');
		if(col>15) break;
		if(col+n<17)
			printStr(row, col, n, str);
		else{
			printStr(row, col, 16-col, str);
			printStr2(0, 16-(col+n-16), n-1, n-(col+n-16)-1, str);
		}
	}
	while(col>1){
		indicateAndPrint(0, --col, ' ');
		if(col-n>=0)
			printStr2(0, col-n, n-1, 0, str);
		else{
			printStr2(0, 0, col-1, 0, str);
			printStr2(1, 0, col, n-1, str);
		}
	}
}

void strShiftR(unsigned char row, unsigned char col, unsigned char *str, unsigned char n){
	LCD_Cmd(0x01);
	Delay(5000);
	LCD_Cmd(0x0C);
	printStr(row, col, n, str);
	while(col<16){
		indicateAndPrint(row, col++, ' ');
		if(col>15) break;
		if(col+n<17)
			printStr(row, col, n, str);
		else{
			printStr(row, col, 16-col, str);
			printStr2(row, 0, n-(col+n-16)-1, n-1, str);
		}
	}
}

void kCounter(void){
	if(kCounter_UPDN){
		++keyCounter;
		if(keyCounter>keyCounterEnd)
			keyCounter = keyCounterStart;
	}
	else{
		--keyCounter;
		if(keyCounter<keyCounterStart)
			keyCounter = keyCounterEnd;
	}
}

void increaseTime(void){
	++sec2;
	if(year%4==0) //閏年
	{
		y=((year-1)+(year-1)/4-(year-1)/100+(year-1)/400); 
		switch (mon)   //逢4年閏年,逢百不閏,逢400年閏年
		{
			case 1:	 m=day; break;	     //1月
			case 2:	 m=(day+31); break;  //2月
			case 3:	 m=(day+60); break;  //3月
			case 4:	 m=(day+91); break;  //4月
			case 5:	 m=(day+121); break; //5月
			case 6:	 m=(day+152); break; //6月
			case 7:	 m=(day+182); break; //7月
			case 8:	 m=(day+213); break; //8月
			case 9:	 m=(day+244); break; //9月
			case 10: m=(day+274); break; //10月
			case 11: m=(day+305); break; //11月
			case 12: m=(day+335); break; //12月
		}
	}
	else if(year%4==1||2||3)	//非閏年
	{
		y=((year-1)+(year-1)/4-(year-1)/100+(year-1)/400);
		switch (mon)
		{
			case 1:  m=day;       break; //1月
			case 2:  m=(day+31);  break; //2月
			case 3:  m=(day+59);  break; //3月
			case 4:  m=(day+90);  break; //4月
			case 5:  m=(day+120); break; //5月
			case 6:  m=(day+151); break; //6月
			case 7:  m=(day+187); break; //7月
			case 8:  m=(day+212); break; //8月
			case 9:  m=(day+243); break; //9月
			case 10: m=(day+273); break; //10月
			case 11: m=(day+304); break; //11月
			case 12: m=(day+334); break; //12月
		}
	}
	if(calendar_FLAG) calendar_UPDATE = 1;
	if (sec2 < 60) return; //若秒小於60到while(1)處   
    sec2=0; min2++;           //秒等於60則令秒=0，分加一
    if (min2 < 60) return; //若分小於60到while(1)處   
    min2=0; hor++;           //若分等於60則令分=0，時加一
	if(hor==24) {
	 
		switch (day){	//檢查日
	   
	  	case 28:   //日=28
			if(year%4==0 && mon==2) day++;//29天//閏年2月
			else if(mon==2) { day=0; mon++;}//28天
		        else day++;	 break;
		
		case 29: //日=29
			if(year%4==0 && mon==2) { day=0;mon++; }
		    else day++;	 break;
		
		case 30: //日=30
			if(mon==4 || mon==6 || mon==9 || mon==11) 
			{day=0;mon++;}	//4.6.9.11月30天
			else day++; break;
			
		case 31: //日=31
		  if(mon==1||mon==3||mon==5||mon==7||mon==8||mon==10) 
			{ day=0;mon++; }   //1.3.5.7.8.10月31天
			else if(mon==12)
			{ day=0; mon=1;year++;}	 //過年
			break;
		}
		if(day<28) day++;	//日<28
	}
    if (hor <25)  return; //若時小於24到while(1)處
    hor=1;min2=0; sec2=0;//若時等於24則令時、分、秒=0 
}

void printCalendar(void){
	if(INIT_FLAG){
		CGRAM(FONT, 0, 3);
		LCD_Cmd(0x01); Delay(5000); 
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	LCD_Cmd(0x80);	//顯示第一行位置
	Send_Data(year/1000+'0');  //年的千位數到LCD顯示
	Send_Data(year%1000/100+'0'); //年的百位數到LCD顯示
	Send_Data(year%100/10+'0'); //年的十位數到LCD顯示
    Send_Data(year%10+'0'); //年的個位數到LCD顯示
	Send_Data(0);
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	Send_Data(mon/10+'0'); //月的十位數到LCD顯示
    Send_Data(mon%10+'0'); //月的個位數到LCD顯示
    Send_Data(1); //顯示月
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	Send_Data(day/10+'0'); //日的十位數到LCD顯示
    Send_Data(day%10+'0'); //日的個位數到LCD顯示
	Send_Data(2);
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	LCD_Cmd(0xc0);
	if(hor==12) //12時上午轉下午
 	{ Send_Data('P'); Send_Data('M'); Send_Data(' ');} //顯示PM
	if(hor==24) //24時下午轉上午 
	{ Send_Data('A'); Send_Data('M'); Send_Data(' ');}//顯示AM
	if(hor<12) //時小於12
	{ Send_Data('A'); Send_Data('M'); Send_Data(' ');}  //顯示AM
	else if(hor>12 && hor<=23) //小時介於12-23之間
	{ Send_Data('P');  Send_Data('M');  Send_Data(' ');} //顯示PM
   	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	if(hor<=12){	 	//時小於/等於12
        Send_Data(hor/10+'0');Send_Data(hor%10+'0'); //顯示時
        Send_Data(':');
	} 
	else{	   //時大於12
	    Send_Data((hor-12)/10+'0'); Send_Data((hor-12)%10+'0');//顯示時
        Send_Data(':');
	}
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
    Send_Data(min2/10+'0');Send_Data(min2%10+'0'); //顯示分
    Send_Data(':');
    if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
    Send_Data(sec2/10+'0'); Send_Data(sec2%10+'0');//顯示秒
    Send_Data(' ');
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	d=(y+m)%7;	//總天數除以7的餘數
	if(d==1) //星期一
	{ Send_Data('M');Send_Data('o');Send_Data('n');} //顯示Mon
	
	if(d==2) //星期二
	{ Send_Data('T');Send_Data('u');Send_Data('e');} //顯示Tue
	
	if(d==3) //星期三
	{ Send_Data('W');Send_Data('e');Send_Data('d');} //顯示Wed
	
	if(d==4) //星期四
	{ Send_Data('T');Send_Data('h');Send_Data('u');} //顯示Thu
	
	if(d==5) //星期五
	{ Send_Data('F');Send_Data('r');Send_Data('i');} //顯示Fri
	
	if(d==6)  //星期六
	{ Send_Data('S');Send_Data('a');Send_Data('t');} //顯示Sat
	
	if(d==0)  //星期日
	{ Send_Data('S');Send_Data('u');Send_Data('n');} //顯示Sun
	ROT_SCAN(100);
	if(calendar_FLAG == 0 && setCalendar_FLAG == 0) return;
	calendar_UPDATE = 0;
}

void setCalendar(void){
	LCD_Cmd(0x01); Delay(5000); 
	printCalendar();
	calendarSetFlag = 0;
	setDataTemp = year;
	LCD_Cmd(0x0F);
	indicateSetData();
	while(calendarSetFlag < 5){
		ROT_SCAN(10);
	}
	sec2 = 0;
	f = 0;
	calendar_FLAG = 1;
	calendar_UPDATE = 1;
}

void assignCalendarData(void){
	switch(calendarSetFlag){
		case 0:
			year = setDataTemp;
			setDataTemp = mon;
			break;
		case 1:
			mon = setDataTemp;
			setDataTemp = day;
			break;
		case 2:
			day = setDataTemp;
			setDataTemp = hor;
			break;
		case 3:
			hor = setDataTemp;
			setDataTemp = min2;
			break;
		case 4:
			min2 = setDataTemp;
			break;
	}
}

void KEY_PRO(void)
{
	//k=0;
	switch (KEY_NO)
	{
		case 0: 
		case 1: 
		case 2: 
		case 3: 
		case 4: 
		case 5: 
		case 6: 
		case 7: 
		case 8: 
		case 9: 
			if(setCalendar_FLAG){
				if(calendarSetFlag && (setDataTemp/10))
					setDataTemp = 0;
				else if(setDataTemp/1000)
					setDataTemp = 0;
				setDataTemp = setDataTemp*10+KEY_NO;
				indicateSetData();
			}
			else{
				f = 0;
				h&=0x03;
				N_FLAG = 1;
			}
			k=0;
			break;	
		case 0x0A: 
			k=0;
			h = 0; f = 0; kCounter_FLAG=1;
			/*if(setCalendar_FLAG){
				assignCalendarData(); ++calendarSetFlag; indicateSetData(); return;
			}
			else{
				counter = counterStart;
				h = 0; f = 0; UP_FLAG = 1;
				SECOND3 = 50;
			}*/
			break;	
		case 0x0B: 
			h = 0; f = 0; k&=0x40; kCounter_UPDN=!kCounter_UPDN;
			break;
		case 0x0C: 
			h = 0; f = 0; calendar_FLAG = 1; calendar_UPDATE = 1;
			break;	
		case 0x0D: 
			h = 0; f = 0; B_FLAG=1;
			break;	
		case 0x0E: 
			h = 0; f = 0; E_FLAG=1;
			break;	
		case 0x0F: 
			h = 0; f = 0; F_FLAG=1;
			break;		
	}
	INIT_FLAG = 1;
}