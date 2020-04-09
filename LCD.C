/*******************************************
      LCD.C 
*******************************************/
#include "LCD.H" 
extern unsigned char bdata f;
extern bit UP_FLAG;
extern unsigned char bdata g;
extern bit INIT_FLAG;
/*unsigned char code FONT[] = {0x04,0x0F,0x12,0x0F,0x0A,0x1F,0x02,0x00,  //年
					      0x1F,0x11,0x1F,0x11,0x1F,0x11,0x11,0x00,  //月
				         0x1F,0x11,0x11,0x1F,0x11,0x11,0x1F,0x00}; //日
						 */
void CGRAM(unsigned char *str, int start, int n) 
{
	unsigned char i;
	n*=8;
	start*=8;
	LCD_Cmd(0x40);           // CGRAM位址設定 
	for (i=0 ; i<n; i++)
		Send_Data(str[start+i]);
}

void Init_LCD(void) // LCD的啟始程式  
{
	LCD_CmdI(0x30); Delay(5000);  
   LCD_CmdI(0x30); Delay(200);
   LCD_CmdI(0x30);  
   LCD_CmdI(0x20);  

   LCD_Cmd(0x28);    
   LCD_Cmd(1); Delay(5000);
   LCD_Cmd(0x0C);  
   LCD_Cmd(6);         
}

void LCD_CmdI(unsigned char Comm) //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(Comm&0xF3); 
	RS=0; EN=1; DLY(100); EN=0; DLY(100);   
}

void LCD_Cmd(unsigned char Comm) //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(Comm&0xF3);  
   RS=0; EN=1; DLY(100); EN=0; DLY(100); 
   DATA_BUS=(DATA_BUS&0x0C)|((Comm<<4)&0xF3);
	RS=0; EN=1; DLY(100); EN=0; DLY(100);  
}

void Send_Data(unsigned char Data)  //傳送資料到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(Data&0xF3);   
   RS=1; EN=1; DLY(100); EN=0; DLY(100); 
   DATA_BUS=(DATA_BUS&0x0C)|((Data<<4)&0xF3); 
	RS=1; EN=1; DLY(100); EN=0; DLY(100);   
}

void Delay(unsigned int Del)       
{
  WAITD: 	
	Del--;	
	if (Del!=0) goto WAITD;	
}

void DLY(unsigned char dly)       
{
  WAITS: 	
	dly--;	
	if (dly!=0) goto WAITS;	
}

void printStr(unsigned char row, unsigned char column, unsigned char n, unsigned char *str){
	unsigned char i;
	if(column<0 || column>15)
		return;
	if(row)
		LCD_Cmd(0xc0+column);
	else
		LCD_Cmd(0x80+column);
	for(i=0;i<n;i++){
		Send_Data(str[i]);
	}
}

/*void printStr2(unsigned char row, unsigned char column, unsigned char start, unsigned char n, unsigned char *str){
	unsigned char i;
	if(row)
		LCD_Cmd(0xc0+column);
	else
		LCD_Cmd(0x80+column);
	for(i=0;i<n;i++){
		Send_Data(str[start+i]);
	}
}*/

void printStr2(unsigned char row, unsigned char col, unsigned char start, unsigned char end, unsigned char *str){
	bit r;
	unsigned char n;
	r=start>end ? 1 : 0;
	n= r ? (start-end+1) : (end-start+1);
	if(row)
		LCD_Cmd(0xC0+col);
	else
		LCD_Cmd(0x80+col);
	while(n--){
		Send_Data(str[start]);
		if(r) start--;
		else start++;
	}
}

void indicateAndPrint(unsigned char row, unsigned char column, unsigned char c){
	if(column<0 || column>15)
		return;
	if(row)
		LCD_Cmd(0xc0+column);
	else
		LCD_Cmd(0x80+column);
	Send_Data(c);
}

void creatCharAndPrint(unsigned char *str, unsigned char n, unsigned char row, unsigned char column){
	unsigned char i;
	n*=8;
	LCD_Cmd(0x40);
	for(i=0;i<8;i++){
		Send_Data(str[n+i]);
	}
	if(row)
		LCD_Cmd(0xc0+column);
	else
		LCD_Cmd(0x80+column);
	Send_Data(0);
}

/*void clearN(unsigned char row, unsigned char column, unsigned char n){
	unsigned char i;
	if(row)
		LCD_Cmd(0xc0+column);
	else
		LCD_Cmd(0x80+column);
	for(i=0;i<n;i++){
		Send_Data(' ');
	}
}*/

void printNumber(unsigned char row, unsigned char column, unsigned char len, int n){
	unsigned char temp;
	if(row)
		temp = 0xC0 + column;
	else
		temp = 0x80 + column;
	while(len--){
		LCD_Cmd(temp+len);
		Send_Data((n%10)+'0');
		n /= 10;
	}
}

void printNumber2(unsigned char row, unsigned char column, unsigned char len, long n){
	unsigned char position=column+len-1;
	if(INIT_FLAG){
		LCD_Cmd(0x01); Delay(5000);  
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	if(row)
		position += 0xC0;
	else
		position += 0x80;
	while(len--){
		LCD_Cmd(position--);
		if((n/10) == 0 && (n%10) == 0) Send_Data(' ');
		else Send_Data((n%10)+'0');
		n /= 10;
	}
}

void printNumber3(unsigned char row, unsigned char column, int n){
	int temp=0;
	if(INIT_FLAG){
		LCD_Cmd(0x01); Delay(5000);  
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	if(row)
		LCD_Cmd(0xC0+column);
	else
		LCD_Cmd(0x80+column);
	while(n){
		temp=(temp*10)+(n%10);
		n/=10;
	}
	while(temp){
		Send_Data((temp%10)+'0');
		temp /= 10;
	}
}

void printNumberF(unsigned char row, unsigned char column, unsigned char len, float n, unsigned char point){
	unsigned char position=column+len-1;
	int temp, i;
	if(INIT_FLAG){
		LCD_Cmd(0x01); Delay(5000);  
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	for(i=0;i<point;++i)
		n*=10;
	temp=n;
	if(row)
		position += 0xC0;
	else
		position += 0x80;
	while(len--){
		LCD_Cmd(position--);
		if(point--){
			if((temp/10) == 0 && (temp%10) == 0) Send_Data(' ');
			else Send_Data((temp%10)+'0');
			temp /= 10;
		}
		else {
			Send_Data('.');
			if(!temp){
				LCD_Cmd(position--);
				Send_Data('0');
				--len;
			}
		}
	}
}

void printNumberBin(unsigned char row, unsigned char column, unsigned char len, unsigned int n){
	unsigned char position=column + len-1;
	if(INIT_FLAG){
		LCD_Cmd(0x01); Delay(5000);  
		LCD_Cmd(0x0C);
		INIT_FLAG = 0;
	}
	if(row)
		position += 0xC0;
	else
		position += 0x80;
	while(len--){
		LCD_Cmd(position--);
		if(n){
			Send_Data((n%2)+'0');
			n>>=1;
		}
		else Send_Data(' ');
	}
}
