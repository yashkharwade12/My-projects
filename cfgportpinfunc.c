#include <LPC21xx.H>
void cfgportpinfunc(int portno,int pinno,int fn)  //This function is useful to configuring the pins
{
   if(portno==0)								  // if the port is '0', that is p0.0 to p0.31
   {
       if(pinno<16)								  // if pin number is p0.0 to p0.15
       {
	     PINSEL0=(PINSEL0&(~(3<<pinno*2)))|(fn<<pinno*2);
	   }
	   else										  // if pin number is p0.16 to p0.31
	   {
	     PINSEL1=(PINSEL1&(~(3<<(pinno-16)*2)))|(fn<<pinno*2);
	   }
   }
   else											  //if port number is '1'
   {
       PINSEL2=(PINSEL2&(~(3<<(pinno-16)*2)))|(fn<<pinno*2);
   }
}
