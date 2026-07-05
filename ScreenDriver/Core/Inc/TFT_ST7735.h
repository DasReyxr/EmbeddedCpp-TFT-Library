/*
 * TFTST7735.h
 *
 *  Created on: Nov 2, 2025
 *      Author: dasre
 */
 
#ifndef TFT_ST7735_H
#define TFT_ST7735_H
#include <stm32f446xx.h>
      
#include <stdint.h>
#include "fonts.h"  // Assuming FontDef lives here



#define ST7735_NOP     0x0000
#define ST7735_SWRESET 0x0001
#define ST7735_RDDID   0x0004
#define ST7735_RDDST   0x0009

#define ST7735_SLPIN   0x0010
#define ST7735_SLPOUT  0x0011
#define ST7735_PTLON   0x0012
#define ST7735_NORON   0x0013

#define ST7735_INVOFF  0x0020
#define ST7735_INVON   0x0021
#define ST7735_GAMSET  0x0026
#define ST7735_DISPOFF 0x0028
#define ST7735_DISPON  0x0029
#define ST7735_CASET   0x002A
#define ST7735_RASET   0x002B
#define ST7735_RAMWR   0x002C
#define ST7735_RAMRD   0x002E

#define ST7735_PTLAR   0x0030
#define ST7735_COLMOD  0x003A
#define ST7735_MADCTL  0x0036

#define ST7735_FRMCTR1 0x00B1
#define ST7735_FRMCTR2 0x00B2
#define ST7735_FRMCTR3 0x00B3
#define ST7735_INVCTR  0x00B4
#define ST7735_DISSET5 0x00B6

#define ST7735_PWCTR1  0x00C0
#define ST7735_PWCTR2  0x00C1
#define ST7735_PWCTR3  0x00C2
#define ST7735_PWCTR4  0x00C3
#define ST7735_PWCTR5  0x00C4
#define ST7735_VMCTR1  0x00C5

#define ST7735_RDID1   0x00DA
#define ST7735_RDID2   0x00DB
#define ST7735_RDID3   0x00DC
#define ST7735_RDID4   0x00DD

#define ST7735_PWCTR6  0x00FC

#define ST7735_GMCTRP1 0x00E0
#define ST7735_GMCTRN1 0x00E1

#define ST7735_MADCTL_MY  0x0080
#define ST7735_MADCTL_MX  0x0040
#define ST7735_MADCTL_MV  0x0020
#define ST7735_MADCTL_ML  0x0010
#define ST7735_MADCTL_RGB 0x0000
#define ST7735_MADCTL_BGR 0x0008
#define ST7735_MADCTL_MH  0x0004

#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB)


extern void config(void);
extern void confRCC(void);
extern void confGPIO(void);
extern void confSPI(void);
// ------- INITIALIZATION -----------
/* D/CX = 0 => Command
    D/CX = 1 => Data*/
#define DCX_CMD  0
#define DCX_DATA 1
// ----- PINS -----
#define PIN_CS   3
#define PIN_DC   4
#define PIN_RST  6
// --- Screen ----
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160
// ---- COLORS ----
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_CYAN    0x07ff
#define COLOR_MAGENTA 0xF81F
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_PINK    0xFC18
#define COLOR_PURPLE  0x8010
#define COLOR_ORANGE  0xFD20



#define sizeofinit 87

/*
*/
class TFT_ST7735 {
public:
    TFT_ST7735();

    void INIT_FN(void);
    void DrawPixel(uint16_t, uint16_t, uint16_t);

    void WriteChar(uint16_t, uint16_t, char, FontDef,
                   uint16_t, uint16_t);

    void WriteString(uint16_t, uint16_t, const char*, FontDef,
                     uint16_t, uint16_t);

    void WriteData(uint8_t* data, uint16_t size);

    void FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

    void FillScreen(uint16_t color);
    virtual ~TFT_ST7735();

private:
    // ----- Hardware Functions -----
    void modeSel(uint8_t);
    void csSet(uint8_t);
    void rstSet(uint8_t);
    void delay_ms(volatile uint32_t);   
    // ----- Communication Functions -----
    void spiWrite(uint16_t*, uint16_t);
    void WriteCommand(uint16_t*, uint16_t);
    // ----- Overloaded Functions -----
    void spiWrite(uint8_t*, uint16_t);
    void spiWrite(uint8_t);
    void WriteCommand(uint8_t);

    void SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
};



#endif
