#include "TFT_ST7735.h"
uint16_t init_cmd[]= {            // Init for 7735R, part 1 (red or green  
    ST7735_SWRESET,     //  1: Software reset, 0 args, w/delay
    ST7735_SLPOUT ,  //  2: Out of sleep mode, 0 args, w/delay
    ST7735_FRMCTR1,  //  3: Frame rate ctrl - normal mode, 3 args:
    0x0101,
    0x012C,
    0x012D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2,  //  4: Frame rate control - idle mode, 3 args:
      0x0101, 
      0x012C, 
      0x012D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x0101, 
      0x012C, 
      0x012D,       //     Dot inversion mode
      0x0101, 
      0x012C, 
      0x012D,       //     Line inversion mode
    ST7735_INVCTR ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x0107,                   //     No inversion
    ST7735_PWCTR1 ,  //  7: Power control, 3 args, no delay:
      0x01A2,
      0x0102,                   //     -4.6V
      0x0184,                   //     AUTO mode
    ST7735_PWCTR2 ,  //  8: Power control, 1 arg, no delay:
      0x01C5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 ,  //  9: Power control, 2 args, no delay:
      0x010A,                   //     Opamp current small
      0x0100,                   //     Boost frequency
    ST7735_PWCTR4 ,  // 10: Power control, 2 args, no delay:
      0x018A,                   //     BCLK/2, Opamp current small & Medium low
      0x012A,  
    ST7735_PWCTR5 ,  // 11: Power control, 2 args, no delay:
      0x018A, 
      0x01EE,
    ST7735_VMCTR1 ,  // 12: Power control, 1 arg, no delay:
      0x010E,
    ST7735_INVOFF ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL ,  // 14: Memory access control (directions), 1 arg:
      ST7735_ROTATION,        //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD ,  // 15: set color mode, 1 arg, no delay:
      0x0105 ,                 //     16-bit color
  ST7735_CASET  ,  //  1: Column addr set, 4 args, no delay:
      0x0100, 
      0x0100,             //     XSTART = 0
      0x0100, 
      0x017F,             //     XEND = 127
    ST7735_RASET  ,  //  2: Row addr set, 4 args, no delay:
      0x0100, 
      0x0100,             //     XSTART = 0
      0x0100, 
      0x017F,            //     XEND = 127 // 31 Init for 7735R, part 3 (red or green tab)
    ST7735_GMCTRP1, //  1: Gamma Adjustments (pos. polarity), 16 args, no delay:
      0x0102, 
      0x011c, 
      0x0107, 
      0x0112,
      0x0137, 
      0x0132, 
      0x0129, 
      0x012d,
      0x0129, 
      0x0125, 
      0x012B, 
      0x0139,
      0x0100, 
      0x0101, 
      0x0103, 
      0x0110,
    ST7735_GMCTRN1, //  2: Gamma Adjustments (neg. polarity), 16 args, no delay:
      0x0103, 
      0x011d, 
      0x0107, 
      0x0106,
      0x012E, 
      0x012C, 
      0x0129, 
      0x012D,
      0x012E, 
      0x012E, 
      0x0137, 
      0x013F,
      0x0100, 
      0x00, 
      0x02, 
      0x10,
    ST7735_NORON  , //  3: Normal display on, no args, w/delay
    ST7735_DISPON //  4: Main screen turn on, no args w/delay
    };
/*

 A3 CS OUT
 A4 DC OUT
 A5 SCK AFR
 A6 RES OUT
 A7 MOSI AFR
 
*/

TFT_ST7735::TFT_ST7735() 
{
    config();
}

TFT_ST7735::~TFT_ST7735() {
}
/*----------- Private Functions -----------*/
void TFT_ST7735::delay_ms(volatile uint32_t ms){
	while(ms--){
		for(volatile uint32_t i =0; i<6000;i++){
		}}
}

void TFT_ST7735::modeSel(uint8_t cmd){

    /* D/CX = 0 => Data (DC LOW)
       D/CX = 1 => Command (DC HIGH)*/
    if (cmd)
        GPIOA->BSRR = (1 << PIN_DC);      // Set DC HIGH for command
    else
        GPIOA->BSRR = (1 << (PIN_DC+16)); // Set DC LOW for data
}

void TFT_ST7735::csSet(uint8_t csEn){
    /* CS LOW to select, CS HIGH to deselect */
    if (csEn)
        GPIOA->BSRR = (1 << PIN_CS);      // Set CS HIGH (deselect)
    else
        GPIOA->BSRR = (1 << (PIN_CS+16)); // Set CS LOW (select)
}

void TFT_ST7735::rstSet(uint8_t rstEn){
    /* RST HIGH for normal, RST LOW for reset */
    if (rstEn)
        GPIOA->BSRR = (1 << PIN_RST);     // Set RST HIGH (normal operation)
    else
        GPIOA->BSRR = (1 << (PIN_RST+16));// Set RST LOW (reset)
}


void TFT_ST7735::spiWrite(uint16_t* DATA, uint16_t size){
    uint16_t sent = 0;
    while(sent<size){

    while(!(SPI1->SR & SPI_SR_TXE));
    if(DATA[sent] & (1<<8)){
        modeSel(DCX_DATA);
        uint8_t data = DATA[sent] & 0xFF;
        SPI1->DR = data;
        while(SPI1->SR & SPI_SR_BSY);
    }
    else{
        modeSel(DCX_CMD);
        uint8_t ola=DATA[sent];
        SPI1->DR = ola;
        while(SPI1->SR & SPI_SR_BSY);
        switch(DATA[sent]){
            case ST7735_SWRESET:
                delay_ms(150);
                break;
            case ST7735_SLPOUT:
                delay_ms(500);
                break;
            case ST7735_NORON:
                delay_ms(10);
                break;
            case ST7735_DISPON:
                delay_ms(100);
                break;
            default:
                break;}
        }
    sent++;
	}

}

void TFT_ST7735::spiWrite(uint8_t* DATA, uint16_t size){
    for(uint16_t i = 0; i < size; i++){
        while(!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = DATA[i];
    }
    while(SPI1->SR & SPI_SR_BSY);  // Wait for last byte to complete
}

void TFT_ST7735::spiWrite(uint8_t DATA){
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = DATA;
    while(SPI1->SR & SPI_SR_BSY);
}

/*----------- Public Functions -----------*/

void TFT_ST7735::WriteCommand(uint16_t* cmd, uint16_t size) {
    spiWrite(cmd, size);
}



void TFT_ST7735::WriteCommand(uint8_t cmd) {
    // Leave CS control to the caller. Set DC for command then write byte.
    modeSel(DCX_CMD);
    spiWrite(cmd);
}


void TFT_ST7735::WriteData(uint8_t* data, uint16_t size) {
    // Leave CS control to the caller. Set DC for data then write buffer.
    modeSel(DCX_DATA);
    spiWrite(data, size);
}

void TFT_ST7735::INIT_FN(void) {
    // HW reset
    rstSet(false);
    delay_ms(5);
    rstSet(true);
    delay_ms(50);

    // Drive CS for the whole init sequence to avoid glitches
    csSet(false);
    WriteCommand(init_cmd,sizeofinit);
    csSet(true);

}


void TFT_ST7735::SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

    WriteCommand(0x2A); // CASET (Column Address Set)
    uint8_t data[] = { x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF };
    WriteData(data, 4);

    WriteCommand(0x2B); // RASET (Row Address Set)
    data[0] = y0 >> 8; data[1] = y0 & 0xFF;
    data[2] = y1 >> 8; data[3] = y1 & 0xFF;
    WriteData(data, 4);

    WriteCommand(0x2C); // RAMWR (start writing pixels)
}


void TFT_ST7735::FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT)) return;
    if((x + w - 1) >= SCREEN_WIDTH ) w = SCREEN_WIDTH  - x;
    if((y + h - 1) >= SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;

    csSet(false);
    SetAddressWindow(x, y, x+w-1, y+h-1);
    
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    modeSel(DCX_DATA);
    
    uint32_t totalPixels = (uint32_t)w * (uint32_t)h;
    for(uint32_t i = 0; i < totalPixels; i++) {
        while(!(SPI1->SR & SPI_SR_TXE));
        spiWrite(hi);
        spiWrite(lo);
    }
    
    while(SPI1->SR & SPI_SR_BSY);  // Wait for last transmission to complete
    csSet(true);
}

void TFT_ST7735::FillScreen(uint16_t color) {
    FillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

void TFT_ST7735::DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    // Ensure CS is held low while setting window and writing pixel data
    csSet(false);
    SetAddressWindow(x, y, x+1, y+1);

    uint8_t buf[2];
    buf[0] = color >> 8;
    buf[1] = color & 0xFF;

    modeSel(DCX_DATA);
    spiWrite(buf[0]);
    spiWrite(buf[1]);
    csSet(true);
}

void TFT_ST7735::WriteChar(uint16_t x, uint16_t y, char ch, FontDef font,
                           uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;

    uint16_t w = font.width;
    uint16_t h = font.height;

    for (i = 0; i < h; i++) {
        b = font.data[(ch - 32) * h + i];
        for (j = 0; j < w; j++) {
            if (b & (1 << (w - j - 1))) {
                DrawPixel(x + j, y + i, color);
            } else {
                DrawPixel(x + j, y + i, bgcolor);
            }
        }
    }
}

void TFT_ST7735::WriteString(uint16_t x, uint16_t y, const char* str, FontDef font,
                             uint16_t color, uint16_t bgcolor)
{
    while (*str) {
        WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
}


//----------- Configuration Functions -----------//

void confRCC(void){
                 
    RCC->AHB1ENR |= (1<<0); // GPIOA SPI1
    RCC->APB2ENR |=(1<<12);//ENCENDEMOS SPI1
}
void confGPIO(void){
                /*    SCK    |     MOSI*/
	GPIOA->MODER |= (2<<(2*5)| 2<<(2*7)); 
	GPIOA->AFR[0]|= (5<<(4*5)| 5<<(4*7)); 
	
                  /*      CS    |     DC/X |    RESET */
    GPIOA->MODER |= (1<<(2*3))|(1<<(2*4))| (1<<(2*6));
    
    // Set initial states
    GPIOA->BSRR = (1 << (3));  // CS high (inactive)
    GPIOA->BSRR = (1 << (4));  // DC high (data mode)
    GPIOA->BSRR = (1 << 6);       // RST high (active)
}



void confSPI(void){
	// Baud rate: divide by 16 for 1MHz communication (16MHz / 16 = 1MHz)
	SPI1->CR1 |=(3<<3);//BR = 011 -> divide by 16
	
	// Master mode
	SPI1->CR1 |=(1<<2);//activamos modo maestro


    
	// Software NSS management
	SPI1->CR1 |=(1<<9);//ACTIVAMOS SS POR SOFTWARE SSM
	SPI1->CR1 |=(1<<8);//ACTIVAMOS EL SS INTERNO PARA FORZAR UNA HABILITACIÓN EN EL MAESTRO SSI
	// Data frame format: 8-bit (not 16-bit as commented)

    // Enable SPI
    SPI1->CR1 |= (1<<6); // PRENDEMOS EL SPI

}


void config(void){
    confRCC();
    confGPIO();
    confSPI();
}
