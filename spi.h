#include "types.h"
void setandreadpassword(void);
void    Init_SPI(void);
u8      spi(u8 sDat);
void    writeEnable(void);
void    writeDisable(void);
void    Spi_eeprom_bytewrite(u16 wBuffAddr, u8 sDat);
u8      Spi_eeprom_byteread(u16 rBuffAddr);
void Spi_eeprom_seqread(u16 rbuffaddr,u8 *p,u8 nbytes);
void Spi_eeprom_pagewrite(u16 wbuffstartaddr,u8 *p,u8 nbytes);
void inputpass(char *str);
