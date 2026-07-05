/*
------- Kevin Lara  -------
--------- Auf Das  ---------
----------- TFT  -----------
-------- 02/11/2025 --------
*/
// ------- Main Library -------
#include "TFT_ST7735.h"

// --------- Function ---------
void test(void);
void conf_osc(void);
// ---------- Class ----------
TFT_ST7735 Screen1;
// -------- Variables --------

// ----------- Main -----------

int main(void){
	Screen1.INIT_FN();
	test();
	while(1);
}


// ------- Configuration -------
void test(void){
	uint8_t i=0;
	Screen1.FillScreen(COLOR_WHITE);
	Screen1.FillRectangle(2+15*0, 2+15*0, 15, 15, COLOR_PURPLE);
	Screen1.FillRectangle(2+15*1, 2+15*1, 15, 15, COLOR_BLACK);
	Screen1.FillRectangle(2+15*2, 2+15*2, 15, 15, COLOR_RED);
	Screen1.FillRectangle(2+15*3, 2+15*3, 15, 15, COLOR_CYAN);
	Screen1.FillRectangle(2+15*4, 2+15*4, 15, 15, COLOR_MAGENTA);
	Screen1.FillRectangle(2+15*5, 2+15*5, 15, 15, COLOR_GREEN);
	Screen1.FillRectangle(2+15*6, 2+15*6, 15, 15, COLOR_BLUE);
	Screen1.FillRectangle(2+15*7, 2+15*7, 15, 15, COLOR_YELLOW);
	Screen1.FillRectangle(2+15*8, 2+15*8, 15, 15, COLOR_PINK);
	
	Screen1.DrawPixel(10,10,COLOR_PINK);

}






