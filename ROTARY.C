/***********************************************  
       ROTARY.C 	
************************************************/  
#include "ROTARY.H"
#include "LCD.H"

unsigned char CW_CNT,CCW_CNT;
extern unsigned char MENUPTR,OLD, menuSize;
extern unsigned int setDataTemp;
extern unsigned char calendarSetFlag;
extern unsigned char bdata f;
extern unsigned char bdata h; 
extern unsigned char bdata k;
extern bit setCalendar_FLAG;

void ROT_SCAN(unsigned int DelN)       
{
	unsigned char j,a,NEW,ABTEMP;
WAITN: 	
	DelN--;  
	j=ROTA; a=ROTB; NEW=a*2+j;
	ABTEMP=NEW^OLD;
	if (ABTEMP!=0) {
		OLD=NEW;
		if (ABTEMP==2) {
			if ((NEW==1)|(NEW==2)) ROT_RIGHT();    
			if ((NEW==0)|(NEW==3)) ROT_LEFT();  
		}	
		else {
			if ((NEW==0)|(NEW==3)) ROT_RIGHT();  
			if ((NEW==1)|(NEW==2)) ROT_LEFT();    
		}	
		h = 0; k = 0; f = 0;
	}
	if (DelN != 0) goto WAITN;	
}

void ROT_LEFT(void) 
{
	CW_CNT=0; CCW_CNT+=1;
	if (CCW_CNT > 3) {
		CCW_CNT=0;  
		if(setCalendar_FLAG){
			--setDataTemp;
			indicateSetData();
		}
		else{
			if (MENUPTR==0) MENUPTR=menuSize - 1;
			else MENUPTR-=1;
			BEEP_PRO(); MENU_DISP();
		}
	}
}

void ROT_RIGHT(void) 
{
	CCW_CNT=0; CW_CNT+=1;
	if (CW_CNT > 3) {
		CW_CNT=0; 
		if(setCalendar_FLAG){
			++setDataTemp;
			indicateSetData();
		}
		else{
			if (MENUPTR==(menuSize - 1)) MENUPTR=0;
			else MENUPTR+=1;
			BEEP_PRO(); MENU_DISP();  
		}
	}
}

void indicateSetData(void){
	switch(calendarSetFlag){
		case 0:
			LCD_Cmd(0x80);	//顯示第一行位置
			setDataTemp %= 10000;
			Send_Data(setDataTemp/1000+'0');  //年的千位數到LCD顯示
			Send_Data(setDataTemp%1000/100+'0'); //年的百位數到LCD顯示
			Send_Data(setDataTemp%100/10+'0'); //年的十位數到LCD顯示
			Send_Data(setDataTemp%10+'0'); //年的個位數到LCD顯示
			break;
		case 1:
			LCD_Cmd(0x85);
			if(setDataTemp<1) setDataTemp = 12;
			if(setDataTemp>12) setDataTemp = 1;
			Send_Data(setDataTemp/10+'0'); //月的十位數到LCD顯示
			Send_Data(setDataTemp%10+'0'); //月的個位數到LCD顯示
			break;
		case 2:
			LCD_Cmd(0x88);
			if(setDataTemp<1) setDataTemp = 31;
			if(setDataTemp>31) setDataTemp = 1;
			Send_Data(setDataTemp/10+'0'); //日的十位數到LCD顯示
			Send_Data(setDataTemp%10+'0'); //日的個位數到LCD顯示
			break;
		case 3:
			LCD_Cmd(0xC3);
			if(setDataTemp<1) setDataTemp = 24;
			if(setDataTemp>24) setDataTemp = 1;
			Send_Data(setDataTemp/10+'0');Send_Data(setDataTemp%10+'0'); //顯示時
			break;
		case 4:
			LCD_Cmd(0xC6);
			if(setDataTemp<0) setDataTemp = 59;
			if(setDataTemp>59) setDataTemp = 0;
			Send_Data(setDataTemp/10+'0');Send_Data(setDataTemp%10+'0'); //顯示分
			break;
	}
}
