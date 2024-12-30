#include<lpc21xx.h>

#define LED1  1<<4
#define LED2  1<<5
#define LED3  1<<6
#define RELAY 1<<7


#define LCD_D 0XF<<14
#define RS    1<<12
#define E     1<<13

void UART0_PIN_CONFIG(void);
void UART0_TX(unsigned char);
char UART0_RX(void);
void CONTROLDEVICE(char);

void LCD_INIT(void);

void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_STR(unsigned char *);
void UART0_STR(unsigned char *);
void delay_millisec(int);


int main()
{
 char ch='0';
 UART0_PIN_CONFIG();
 LCD_INIT();
 //LCD_DATA('A');
 IODIR0|=LED1|LED2|LED3|RELAY;
 while(1)
 {
 ch=UART0_RX();
 CONTROLDEVICE(ch);
 }
 }
													
 void CONTROLDEVICE(char ch)
 {
 	LCD_COMMAND(0X80);
  if(ch=='A')
  {
  IOSET0=LED1;
  LCD_STR("LED1 ON");
  UART0_STR("LED1 ON");
  }
  else if(ch=='B')
  {
  IOCLR0=LED1;
  LCD_STR("LED1 OFF");
  UART0_STR("LED1 OFF");
  }
  else if(ch=='C')
  {
  IOCLR0=LED2;
  LCD_STR("LED2 ON");
  UART0_STR("LED2 ON");
  }
  else if(ch=='D')
  {
  IOSET0=LED2;
  LCD_STR("LED2 OFF");
  UART0_STR("LED2 OFF");
  }
  else if(ch=='E')
  {
  IOCLR0=LED3;
  LCD_STR("LED3 ON");
  UART0_STR("LED3 ON");
  }
  else if(ch=='F')
  {
  IOSET0=LED3;
  LCD_STR("LED3 OFF");
  UART0_STR("LED3 OFF");
  }
  else if(ch=='G')
  {
  IOCLR0=RELAY;
  LCD_STR("RELAY ON");
  UART0_STR("RELAY ON");
  }
  else if(ch=='H')
  {
  IOSET0=RELAY;
  LCD_STR("RELAY OFF");
  UART0_STR("RELAY OFF");
  }	 
 }
void UART0_PIN_CONFIG(void)
{
  PINSEL0=0X05;
  U0LCR=0X83;
  U0DLL=97;
  U0DLM=0;
  U0LCR=0X03;
}

 void UART0_TX(unsigned char ch)
 {
   U0THR=ch;
   while((U0LSR>>5&1)==0);
 }
char UART0_RX(void)
 {
   while((U0LSR&1)==0);
   return U0RBR;
 }
 void LCD_INIT(void)
{
  IODIR0|=LCD_D|RS|E;
  LCD_COMMAND(0X01);
  LCD_COMMAND(0X02);
  LCD_COMMAND(0X0C);
  LCD_COMMAND(0X28);
  LCD_COMMAND(0X80);
}

void LCD_COMMAND(unsigned char cmd)
{
 IOCLR0=LCD_D;
 IOSET0=(cmd&0XF0)<<10;
 IOCLR0=RS;								
 IOSET0=E;
 delay_millisec(2);
 IOCLR0=E;

 IOCLR0=LCD_D;
 IOSET0=(cmd&0X0F)<<14;
 IOCLR0=RS;
 IOSET0=E;
 delay_millisec(2);
 IOCLR0=E;
}

void LCD_DATA(unsigned char d)
{
  IOCLR0=LCD_D;
  IOSET0=(d&0XF0)<<10;
  IOSET0=RS;
  IOSET0=E;
  delay_millisec(2);
  IOCLR0=E;

  IOCLR0=LCD_D;
  IOSET0=(d&0X0F)<<14;
  IOSET0=RS;
  IOSET0=E;
  delay_millisec(2);
  IOCLR0=E;
}

void LCD_STR(unsigned char *s)
{
  while(*s)
  	LCD_DATA(*s++);
}
void UART0_STR(unsigned char *s)
{
 while(*s)
 UART0_TX(*s++);
}
void delay_millisec(int n)
{
  T0PR=15000-1;
  T0TCR=0X001;
  while(T0TC<n);
  T0TCR=0X003;
  T0TCR=0X00;
}

