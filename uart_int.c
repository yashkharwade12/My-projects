#include <LPC21xx.H>  /* LPC21xx definitions         */
#include"lcd_defines.h"
#include"lcd.h"
#include"types.h"
#include"delay.h"
#include <string.h>
char str[23],dummy,ch;
unsigned int i=0,r_flag=0;
#define UART_INT_ENABLE 1
void UART0_isr(void) __irq
{
  if((U0IIR & 0x04)) //check if receive interrupt,that is Receive data available(RDA).This confirms that the interrupt was triggeres
  {
		ch = U0RBR;	/* Read to Clear Receive Interrupt */

		if(ch != 0x0d)	 //"0x0d" is nothing but a carriage return,it return the carriage to the begining of the current line,used as a terminator. 
			str[i++] = ch; 	  //each character will be stored in a string.
		else
		{
			str[i] = '\0'; //if the termination occurs then the last character will be placed as NULL.
			i=0;		   //'i' is made '0',because we need to use it again
			r_flag = 1;	   //This r_flag indicates that you touched one of the region in the touch screen and some data is transmitted.
		}
	}
  	else
	{
      dummy=U0IIR; //Read to Clear transmit interrupt
	}
   	VICVectAddr = 0; 
} 

void InitUART0 (void) /* Initialize Serial Interface       */ 
{  
  PINSEL0 = 0x00000005; /* Enable RxD0 and TxD0              */
  U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U0LCR = 0x03;         /* DLAB = 0  */
  #if UART_INT_ENABLE > 0
  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART0_isr;
  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
  VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   
  #endif
}



