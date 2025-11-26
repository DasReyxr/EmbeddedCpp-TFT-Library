/*
------- Kevin Lara  -------
--------- Auf Das  ---------
----------- TFT  -----------
-------- 02/11/2025 --------
*/
// ------- Main Library -------
#include "TFT_ST7735.h"

// --------- Function ---------
void osc(void);
void conf_osc(void);
// ---------- Class ----------
TFT_ST7735 Screen1;
// -------- Variables --------

// ----------- Main -----------

int main(void){
	conf_osc();
	Screen1.INIT_FN();
	
	Screen1.FillRectangle(2, 2, 128, 160, 0x0000);
	// -- Axes --
	Screen1.FillRectangle(2, 2, 113, 1, COLOR_WHITE); 
	Screen1.FillRectangle(2, 2, 1, 146, COLOR_WHITE); 

	uint16_t iterative = 0;		
	while(1){//*
		uint16_t adc_val = (ADC1->DR)>>5; // ADC-Val * 2^12/2^7 = ADC-Val / 32
		iterative++;
		Screen1.DrawPixel(adc_val+3,iterative+3,COLOR_CYAN);
		if(iterative>=145){
			iterative=0;
			Screen1.FillRectangle(2, 2, 128, 160, COLOR_BLACK);
		}//*/
	}
}


// ------- Configuration -------
void conf_osc(void){
	// PLL Config
	RCC->CR |=(1 << 16);
	while(!(RCC->CR & RCC_CR_HSERDY));
	/*Acivamos el prefetch del FLASH y cambiamos la latencia entre lectura de la CPU y la memoria a 2 waitstates*/
	FLASH->ACR |= (1<<8) ;
	FLASH->ACR |= (5<<0);
	/*Cambiamos la velicidad del apb1 ya que no puede trabajar a mas de 45MHZ y la de apb2 que no trabaja a mas 90*/
	RCC->CFGR |= (4<<10);
	//RCC->CFGR |= (4<<13);
	RCC->PLLCFGR |=(1 << RCC_PLLCFGR_PLLSRC_Pos);
	RCC->PLLCFGR |= (360 << RCC_PLLCFGR_PLLN_Pos) |
									(8 << RCC_PLLCFGR_PLLM_Pos) |
									(1 << RCC_PLLCFGR_PLLP_Pos) ;
	/*LAS VELOCIDADES DEL AHB, EL APB1,APB2 DEBEN MODIFICARSE SI CAMBIO LA FRECUENCIA A LA MAS ALTA*/
	RCC->CR |=(1 << 24);
	RCC->CFGR |= (2 << 0);
	SystemCoreClockUpdate();

	// ADC Config
	GPIOA->MODER|=(3<<0);//ANALOGPA0
	RCC->APB2ENR |=(RCC_APB2ENR_ADC1EN);//ADCENABLE
	ADC1->CR2|=(3<<0);//ACTIVAMOS CONTINUOS CONV Y ADCON
    ADC1->CR2|=(1<<30);//INICIAMOS LA CONVERSIÓN
	ADC1->CR1|=(1<<8);//SCANMODE
	ADC1->SMPR2|=(2<<0); //ESTAMOS CONFIGURANDO 28 CICLOS DE MUESTREO
}



