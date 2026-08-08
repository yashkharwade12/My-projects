#include <LPC21xx.h>
#include "spi_defines.h"
#include "cfgportpinfunc.h"
#include"lcd.h"
#include"KPM.h"
#include "delay.h"
#include"types.h"
char password[4],checker[4];
int o,oflag;
                                                          

void Init_SPI(void)
{
    cfgportpinfunc(0, 4, 1);  // P0.4 for SCLK
    cfgportpinfunc(0, 5, 1);  // P0.5 for MISO
    cfgportpinfunc(0, 6, 1);  // P0.6 for MOSI
    IOSET0  = 1 << CS;          // P0.7 high for CS
    IODIR0  |= 1 << CS;
    S0SPCCR = LOADVAL;          // cfg SPI clock speed
    S0SPCR  = 1 << MSTR | 3 << CPHA;  // cfg SPI peripheral for master mode, Mode 3
}

u8 spi(u8 sDat)
{
    S0SPDR = sDat;              // load sDat into SOSPDR
    while (((S0SPSR >> SPIF) & 1) == 0);  // wait for TX to complete
    return S0SPDR;              // return read data from SOSPDR
}

void WriteEnable(void)
{
    IOCLR0 = 1 << CS;           // make chip select low
    spi(WREN);                  // issue WREN via SPI transfer (0x06)
    IOSET0 = 1 << CS;           // make chip select high
}

void WriteDisable(void)
{
    IOCLR0 = 1 << CS;           // make chip select low
    spi(WRDI);                  // issue WRDI via SPI transfer (0x04)
    IOSET0 = 1 << CS;           // make chip select high
}

void Spi_eeprom_bytewrite(u16 wBuffAddr, u8 sDat)
{
    WriteEnable();              // enable write enable latch
    IOCLR0 = 1 << CS;           // make chip select low
    spi(WRITE);                 // issue write instruction via SPI transfer
    spi(wBuffAddr >> 8);        // MSB byte of 16-bit address loc via SPI transfer
    spi(wBuffAddr);             // LSB byte of 16-bit address loc via SPI transfer
    spi(sDat);                  // Data for that address location
    IOSET0 = 1 << CS;           // make chip select high
    delay_ms(5);
    WriteDisable();             // Disable write enable latch
}

u8 Spi_eeprom_byteread(u16 rBuffAddr)
{
    u8 rDat;
    IOCLR0 = 1 << CS;           // make chip select low
    spi(READ);                  // issue read instruction
    spi(rBuffAddr >> 8);        // MSB byte of 16-bit address loc to be read via SPI
    spi(rBuffAddr);             // LSB byte of 16-bit address loc to be read via SPI
    rDat = spi(0x00);           // sending garbage & reading data at loc
    IOSET0 = 1 << CS;           // make chip select high
    return rDat;
}

void Spi_eeprom_pagewrite(u16 wbuffstartaddr,u8 *p,u8 nbytes)
{
	u8 i;
	WriteEnable();	     //Makes the write Enable.
	IOCLR0=1<<CS;	     //Selecting the CS as it is active low
	spi(WRITE);		     //Passing the write command
	spi(wbuffstartaddr>>8);//As MSB is first in SPI, sending that MSB
	spi(wbuffstartaddr);   //After MSB sending LSB
	for(i=0;i<nbytes;i++)  //Writing the number of bytes
	{
		spi(p[i]);
	}
	IOSET0=1<<CS;	     //Deselecting the CS, as it is active low
	delay_ms(5);
	WriteDisable();	     //Disabling the write operation
}

void Spi_eeprom_seqread(u16 rbuffaddr,u8 *p,u8 nbytes)
{
	u8 i;
	IOCLR0 = 1<<CS;  	     //Selecting the CS as it is active low
	spi(READ);		     //Sending the read command
	spi(rbuffaddr>>8);     //AS MSB is first in SPI, sending that MSB
	spi(rbuffaddr);	     //After MSB sending LSB
	for(i=0;i<nbytes;i++)
	{
		p[i]=spi(0x0000);	//Reading the number of bytes
	}
	IOSET0=1<<CS;		//Deselecting the CS, as it is active low.
}
void setandreadpassword()
{
	Spi_eeprom_seqread(0x0000,(u8 *)password,4);	//This is same as checker, just used to copy the new updated password and stored in the this variable, and this statement is used to read that.
	delay_ms(5);
	Spi_eeprom_seqread(0x0000,(u8 *)checker,4); //Will read the password from the EEEPROM and store in checker
}
void inputpass(char *str)
{
	oflag=0;
	cmdLCD(0xc0);
	for(o=0;o<5;o++)	 // total 5 inputs, last one is for the enter key, I took enter key as any key other than digits
	{
		if(oflag)
		{
			o=0;
			oflag=0;
		}
		str[o]=keyscan();	 //takes the key values
		if(str[o]=='+')	 //"+" is treated as backspace here
		{
			o=o-1;	 //to delete the previously entered key 'o' is decremented.
			cmdLCD(0xc0 + o);	  //position will be travelled to the 'o' that is previous position
			charLCD(' ');	  //space is printed in that previous position
			if(o==0)
			{
				oflag=1;	  //if o==0,that is in the starting if we press the backspace then oflag will be "1".
			}
			o=o-1;	 //This decrementation is because to replace the next iteration in the previous iteration, because in the for loop it will automatically increment, so to get the proper position we decremented it
			if(o<0)
				o=0;	 //if 'o' is negative then we made it '0'.
		}
		else			 //if the pressed key is some digit instead of back space then it will enter this block
		{
			if(o!=4)	 //From 0 to 4, 4th key is enter key, this condition is while it is not enter key
			{
				cmdLCD(0xc0 + o);	  //Travel to the particular position
				//charLCD(temp[o]);
				charLCD('*');	  //Will Print the Encripted Password.
			}
		}
		delay_ms(500);
	}
}
