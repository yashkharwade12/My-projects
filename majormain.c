#include <LPC21xx.H>
#include"lcd_defines.h"
#include"lcd.h"
#include"delay.h"
#include"types.h"
#include"SPI.h"
#include"KPM.h"
#include"UART_INT.h"
#include<string.h>
extern char str[23],dummy,ch;
char x[10],y[10],z[10];
extern unsigned int i,r_flag;
extern char password[4],checker[4];
char temp[4],temp1[4],new1[4],new2[4];
int checkflag=0,highflag=0;
int enable=0,buzzer=0;

void eint1_isr(void)__irq
{
        int a,int_flag=0;
        cmdLCD(0x01);
        cmdLCD(0x80);
        strLCD("Enter curr password");  //Before changing the password we should confirm the current password for security purpose, if not any one can be able to change the password.
        cmdLCD(0xc0);
        inputpass(temp1);                         //This function is used to take the input from keypas and store in the temp1 variable.
        delay_ms(500);
        //temp[o]='\0';
        if(strncmp(temp1,checker,4)==0) //Compare the taken password with the password in the EEPROMO(checker).
        {
                cmdLCD(0x01);
                cmdLCD(0x80);
                strLCD("unlocked");       //If correct then it will be unlocked
                int_flag=1;                       //This int_flag is used to update the new password.
                delay_s(1);
        }
        else
        {
                cmdLCD(0x01);
                cmdLCD(0x80);
                strLCD("Password not matched");
                delay_s(1);
                cmdLCD(0x01);                      //If password not matched the control will be returning to main.
                cmdLCD(0x80);
                strLCD("Returned to main");
                delay_s(1);
        }
        if(int_flag==1)                            //If the entered current password is correct then it will enter this block
        {
                while(1)
                {
                cmdLCD(0x80);
                strLCD("Enter new password");
                cmdLCD(0xc0);
                inputpass(new1);                          //Stores the entered password in new1
                cmdLCD(0x01);
                cmdLCD(0x80);
                strLCD("Confirm password");
                cmdLCD(0xc0);
                inputpass(new2);                          //stores the entered confirmation password in new2
                for(a=0;a<4;a++)
                {
                        if(new1[a]!=new2[a])
                        {
                                checkflag=1;      //If any one of the char/digit in new1 and new2 are no equal then checkflag=1,else checkflag=0.
                        }
                }
                if(checkflag==0)                          //If checkflag==0, that is if new password and confirmed password is same, it will enter this block.
                {
                        for(a=0;a<4;a++)
                        {
                                password[a]=new1[a]; //Will save the new password (new1) into the password variable.
                        }
                        cmdLCD(0x01);
                        cmdLCD(0x80);
                        strLCD("Password changed");
                        delay_s(1);
                        cmdLCD(0x01);
                        Spi_eeprom_pagewrite(0x0000,(u8 *)password,4);  //Stores the updated password in the EEPROM
                        break;  //will break the loop
                }
                             else
                {
                        cmdLCD(0x01);
                        cmdLCD(0x80);
                        strLCD("Password not matched");  //if new password and confirmed password is not matched then thw while loop will be repeating untill they are matched.
                        delay_s(1);
                }
            }
        }
        if(checkflag==0 && int_flag==1) //If password is changed the process should start from begining
        {
                cmdLCD(0x01);
                cmdLCD(0x80);
                strLCD("Pro start from begin"); //Instead of executing the main from starting , we used the initial starting operation of main block in this isr block.
                highflag=1;                       //This highflag is useful because, if we rise an external interrupt to change the password at the starting of the main block then in main block also it will ask for password again, so to avoid duplication we taken this flag.
                enable=0;
                buzzer=0;                           //made all the devices low as we are starting the process from begining
                IOCLR0 = 1<<22|1<<23|1<<25;
                delay_s(1);
                cmdLCD(0x01);
                cmdLCD(0x80);
                strLCD("Enter the password:");
                delay_s(1);
                while(1)
                {
                        inputpass(temp);                           //Will take the input from keypad and store in the temp
                        setandreadpassword();              //will read the password from the EEPROM(checker);
                        if(strncmp(temp,checker,4)==0)   //Comparing the temp and checker
                        {
                                cmdLCD(0x01);
                                cmdLCD(0x80);
                                strLCD("unlocked");
                                delay_s(1);
                                cmdLCD(0x80);
                                strLCD("Touch: OFF");      //Will display all the divices as OFF
                                cmdLCD(0xc0);
                                strLCD("Buzzer: OFF");
                                cmdLCD(0x94);
                                strLCD("Fan: OFF");
                                cmdLCD(0xd4);
                                strLCD("Light: OFF");
                                break;
                        }
                        else
                        {
                                cmdLCD(0x01);
                                cmdLCD(0x80);
                                strLCD("Re-enter password");    //If entered password is incorrect, the while loop will repeate untill correct password is entered.
                                delay_s(1);
                        }
                }
        }
        VICVectAddr=0;                      //Clearing the registers,it will be useful to raise the external interrupt again
        EXTINT=1<<1;
}

void eint1_enable(void)
{
        PINSEL0 |= 3<<6;                        //P0.3 is selected for swirch to raise an external interrupt as it supports EINT1 in function 4.
        VICIntSelect=0<<15;             //The interrupt is selected as "IRQ" in channel number 15(channel no of EINT1).
        VICIntEnable=1<<15;             //Interrupt enabled
        VICVectCntl1=(1<<5)|15;         //selected slot 1
        VICVectAddr1=(unsigned int)(eint1_isr); // ISR block is called
        EXTMODE=1<<0;
        EXTPOLAR=0<<0;                  //Selected as falling edge as LPC2148 is not supporting with rising edge
}
int main()
{
        int j,k,m=1,d1,d2;              //initially m=1,after entering correct password m=0,so that while loop will be executed only once
        int xint,yint,zint;             // these will store the interger values of x,y,z co-ordinates
        IODIR0 =1<<25|1<<22|1<<23;      // buzzer 25, device1 23, device2 22.
        InitLCD();
        InitKPM();                                      //Initializing all the requirements
        InitUART0();
        Init_SPI();
                                  eint1_enable();                 // External interrupt enabling
        //Spi_eeprom_pagewrite(0x0000,"6305",4); //this line is for storing the password into the EEPROM intially,firstly uncomment this line and execute only this line,after that comment this line and execute the other lines
        setandreadpassword();           //this function is used to read the password from EEPROM
        delay_s(2);
        /*
        cmdLCD(0x01);
        cmdLCD(0x80);
        strLCD((u8 *)password);         //this block is jst for checking the eeprom password
        delay_s(1);       */
        cmdLCD(0x01);
        cmdLCD(0x80);
        if(r_flag)                              //When you connect the touch screen initially it will transmitt a string "Touch Screen Enabled",so r_flag will be '1' if something is transmitted,else it will be '0'
        {
        if(str[0]!='\0')
        {
                strLCD("Touch connected");
                delay_s(1);
        }
        }
        else
        {
                strLCD("Touch not connected");
                delay_s(1);
        }
        IOCLR0 = 1<<23|1<<22;           //Initially clearling all the devices
        cmdLCD(0x01);
        cmdLCD(0x80);
        strLCD("Enter the password:");
        delay_s(1);
        while(1)
        {
                        while(m==1)             //If correct password is entered m will be '0',and this loop will break
                        {
                                if(highflag==0)  //This highflag is used to avoid the repeatation of the process in the isr block and main block
                                {
                                        inputpass(temp); //This function is used to take the input from keypad and store in temp variable
                                        setandreadpassword();  //This function is used to read the password from EEPROM
                                        if(strncmp(temp,checker,4)==0) //Comparing the temp password and checker password,checker is nothing but the variable storing the password read from the EEPROM
                                        {
                                                m=0;         //If password is correct,m=0 and inner while loop will not execute again
                                                cmdLCD(0x01);
                                                cmdLCD(0x80);
                                                strLCD("unlocked");
                                                delay_s(1);
                                                cmdLCD(0x80);
                                                strLCD("Touch: OFF");    //Initially displaying all the devices as OFF
                                                cmdLCD(0xc0);
                                                strLCD("Buzzer: OFF");
                                                cmdLCD(0x94);
                                                strLCD("Fan: OFF");
                                                cmdLCD(0xd4);
                                                strLCD("Light: OFF");
                                                break;                   //Inner while loop will be breaked
                                        }
                                        else
                                        {
                                                cmdLCD(0x01);
                                                cmdLCD(0x80);
                                                strLCD("Re-enter password");    //If entered password is incorrect,this whole while loop will repeat untill correct password is entered.
                                                delay_s(1);
                                        }
                                }
                        }
                        delay_ms(500);
                r_flag=0,i=0;
                while(r_flag==0);          //process will wait untill you touch the touch screen
                        delay_s(1);
                        for(j=0;j<6;j++)
                        {
                                x[j]=str[j];  //rearranging the received xyz co-ordinates into seperate char arrays
                        }
                        x[j]='\0';
                        for(k=0,j=8;j<13;k++,j++)
                        {
                                y[k]=str[j];
                        }
                        y[k]='\0';
                        for(k=0,j=15;j<20;j++,k++)
                        {
                                z[k]=str[j];
                        }
                        z[k]='\0';

                                                                 /*cmdLCD(0x01);
                        delay_ms(2);
                        cmdLCD(0x80);
                        strLCD((u8*)x);
                        cmdLCD(0xc0);
                        strLCD((u8*)y);   // This block is jst to print xyz co-ordinates
                        cmdLCD(0x94);
                        strLCD((u8*)z);
                        delay_ms(1000); */
                        r_flag=0,i=0;
                        xint=(((x[4]-48)*10)+(x[5]-48));        //converting the xyz co-ordinates from ascii to integer
                        yint=(((y[3]-48)*10)+(y[4]-48));
                        zint=(((z[3]-48)*10)+(z[4]-48));
                        if((xint<11) && (yint>=14) && (zint<15))        //co-ordinates region selected for Touch Screen Enable/Disable
                        {
                                enable^=1;          //If touched toggle operation will be done,1 will become 0,0 will become 1.
                        }
                        if((xint>11) && (yint>=14) && (zint<15))        //co-ordinates region selected for Buzzer
                        {
                                if(enable==1)                           //Even it is touched it should work only if it is enabled
                                {
                                        buzzer^=1;                              //If touched toggle operation will be done,if it is ON it will become OFF and vice versa.
                                        if(buzzer==1)
                                        {
                                                IOSET0 = 1<<25;         //If Buzzer==1, Buzzer sound will be started
                                        }
                                        else if(buzzer==0)
                                        {
                                                IOCLR0 = 1<<25;         //If Buzzer==0, Buzzer sound will stoped.
                                        }
                                }
                        }
                        if((xint<11) && (yint<14 && yint>=8) && (zint<15)) //Co-ordinates region selected for Fan       ON
                        {
                                if(enable==1)                            //will work only if touch is enabled
                                {
                                        d1=1;                                    //This is used to display ON/OFF
                                        IOSET0 = 1<<23;                  //If touched LED will be high
                                }
                        }
                        if((xint>11) && (yint<14 && yint>=8) && (zint<15)) //Co-ordinates region selected for Fan OFF
                        {
                                if(enable==1)                            //Will work only if touch is enabled
                                {
                                   d1=0;                                         //This is used to diaplay ON/OFF
                                   IOCLR0 = 1<<23;                       //If Touched LED will be low
                                }
                        }
                        if((xint<11) && (yint<8) && (zint<15))   //Co-ordinates region selected for Light ON
                        {
                                if(enable==1)                            //Will work only if touch is enabled
                                {
                                        d2=1;                                    //This is used to display ON/OFF
                                        IOSET0 = 1<<22;                  //If Touched LED will be high
                                                      }
                        }
                        if((xint>11) && (yint<8) && (zint<15))   //Co-ordinates region selected for Light OFF
                        {
                                if(enable==1)                            //Will work only if touch is enabled
                                {
                                        d2=0;                                    //This is used to display ON/OFF
                                        IOCLR0 = 1<<22;                  //If Touched LED will be low
                                }
                        }
                        cmdLCD(0x01);
                        if(enable==1)
                        {
                                cmdLCD(0x80);
                                strLCD("Touch: ON");             //if enable==1,will print "ON",else "OFF".
                        }
                        else
                        {
                                cmdLCD(0x80);
                                strLCD("Touch: OFF");
                        }
                        if(buzzer==1)
                        {
                                cmdLCD(0xc0);
                                strLCD("Buzzer: ON");            //if Buzzer==1,will print "ON", else "OFF".
                        }
                        else
                        {
                                cmdLCD(0xc0);
                                strLCD("Buzzer: OFF");
                        }
                        if(d1==1)
                        {
                                cmdLCD(0x94);
                                strLCD("Fan: ON");              //if d1==1,will print "ON", else "OFF".
                        }
                        else
                        {
                                cmdLCD(0x94);
                                strLCD("Fan: OFF");
                        }
                        if(d2==1)
                        {
                                cmdLCD(0xd4);
                                strLCD("Light: ON");            //if d2==1,will print "ON",else "OFF".
                        }
                        else
                        {
                                cmdLCD(0xd4);
                                strLCD("Light: OFF");
                        }
        }
}
                                                                                                                                                   