/*
 * TFTST7735.h
 *
 *  Created on: Nov 2, 2025
 *      Author: dasre
 */
 
#ifndef TOUCH_SCREEN_H
#define TOUCH_SCREEN_H
#include "stm32f4xx.h"
#include "main.h"      
#include <stdint.h>

#include "TFT_Screen.h"




#define SCREEN_TOUCH_IRQ_GPIO_Port GPIOA
#define SCREEN_TOUCH_IRQ_Pin 0

#define SCREEN_SPI_PORT hspi1
extern SPI_HandleTypeDef SCREEN_SPI_PORT;



static const GpioPin ScreenTouchCS = {
    .port = SCREEN_CS_GPIO_Port, .pin  = SCREEN_CS_Pin
};

static const GpioPin ScreenTouchIRQ = {
    .port = SCREEN_TOUCH_IRQ_GPIO_Port, .pin  = SCREEN_TOUCH_IRQ_Pin
};

static const GpioPin ScreenTouchRST = {
    .port = SCREEN_RST_GPIO_Port, .pin  = SCREEN_RST_Pin
};






    #define TOUCH_READ_X 0xD0
    #define TOUCH_READ_Y 0x90
    #define TOUCH_READ_Z1 0xB0
    #define TOUCH_READ_Z2 0xC0

    #define SCREEN_TOUCH_SCALE_X 240
    #define SCREEN_TOUCH_SCALE_Y 320
    // default orientation
    /*
    #define SCREEN_TOUCH_MIN_RAW_X 4400//4500
    #define SCREEN_TOUCH_MAX_RAW_X 30110//30500
    #define SCREEN_TOUCH_MIN_RAW_Y 2680//2680
    #define SCREEN_TOUCH_MAX_RAW_Y 29740//30110
    */
    // rotate right
    /*
    #define SCREEN_TOUCH_MIN_RAW_X 4400//4500
    #define SCREEN_TOUCH_MAX_RAW_X 30110//30500
    #define SCREEN_TOUCH_MIN_RAW_Y 2680//2680
    #define SCREEN_TOUCH_MAX_RAW_Y 29740//30110
    */
    // rotate left

    #define SCREEN_TOUCH_MIN_RAW_X 4400//4500
    #define SCREEN_TOUCH_MAX_RAW_X 30110//30500
    #define SCREEN_TOUCH_MIN_RAW_Y 2680//2680
    #define SCREEN_TOUCH_MAX_RAW_Y 29740//30110

    // upside down
    /*
    #define SCREEN_TOUCH_MIN_RAW_X 4400//4500
    #define SCREEN_TOUCH_MAX_RAW_X 30110//30500
    #define SCREEN_TOUCH_MIN_RAW_Y 2680//2680
    #define SCREEN_TOUCH_MAX_RAW_Y 29740//30110
    */


// ------- INITIALIZATION -----------


// ----- Hardware Functions -----
void delay_ms(volatile uint32_t);   
// ----- Communication Functions -----
uint8_t Screen_TouchPressed(void);
uint8_t Screen_TouchGetCoordinates(uint16_t* x, uint16_t* y);
void spiRead_8b_Vector(uint8_t* DATA, uint16_t size);




#endif
