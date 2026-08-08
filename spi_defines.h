#define SCLK    4    // P0.4
#define MISO    5    // P0.5
#define MOSI    6    // P0.6
#define CS      7    // P0.7

// Clock settings
#define FOSC        12000000
#define CCLK        (FOSC*5)
#define PCLK        (CCLK/4)
#define SPI_SPEED   100000
#define LOADVAL     (PCLK/SPI_SPEED)

#define CPHA    3
#define CPOL    4
#define MSTR    5
#define SPIF    7

// Commands
#define WRITE   0x02
#define WREN    0x06
#define WRDI    0x04
#define READ    0x03