# STM32F4 TFT ST7735 Driver and Demo

- **MCU**: STM32F446 (bare‑metal, register access)
- **Display**: ST7735 1.8" TFT (128x160) over SPI
- **Library files**: `src/TFT_ST7735.h`, `src/TFT_ST7735.cpp`, `src/fonts.h`, `src/fonts.c`
- **Example app**: `src/main_tst.cpp` (basic drawing demo)

This README documents how to use the ST7735 driver: initialization, public API, parameters, return values, possible error conditions, and example usage. It also outlines a midterm project concept built on this driver.

## Getting Started
- **Wiring (STM32F446 → TFT)**
	- `PA5` → `SCK`
	- `PA7` → `MOSI`
	- `PA3` → `CS`
	- `PA4` → `DC`
	- `PA6` → `RST`
	- `VCC/GND` per display datasheet
- **Clocking**: SPI1 runs at ~1 MHz (`BR=011` → fPCLK/16). Adjust in `confSPI()` if needed.
- **Screen geometry**: `SCREEN_WIDTH=128`, `SCREEN_HEIGHT=160`. Color format: RGB565 (16‑bit).

## Initialization
The constructor configures RCC, GPIO, and SPI via `config()`. Call `INIT_FN()` once to reset and send the full ST7735 init sequence.

```cpp
#include "TFT_ST7735.h"

int main() {
	TFT_ST7735 tft;
	tft.INIT_FN();
	tft.FillScreen(COLOR_BLACK);
	while (1) {}
}
```

## Public API

- `void INIT_FN()`
	- Initializes the TFT: hardware reset, sends init commands, sets color mode and addressing.
	- Parameters: none
	- Returns: none
	- Errors: If SPI is misconfigured or pins/wiring are incorrect, the screen may remain blank.

- `void FillScreen(uint16_t color)`
	- Fills entire screen with `color` (RGB565).
	- Params: `color` (e.g., `COLOR_BLACK`)
	- Returns: none
	- Errors: none; internally clips to screen bounds.

- `void FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)`
	- Draws a filled rectangle with top‑left at `(x,y)`.
	- Params: `x`, `y` (pixels), `w`, `h` (size), `color` (RGB565)
	- Returns: none
	- Errors: If the rectangle exceeds bounds, the function clips to fit. If `x>=SCREEN_WIDTH` or `y>=SCREEN_HEIGHT`, it returns early.

- `void DrawPixel(uint16_t x, uint16_t y, uint16_t color)`
	- Draws a single pixel.
	- Params: `x`, `y`, `color`
	- Returns: none
	- Errors: No explicit check; writing outside set window would be ignored. Prefer passing valid coordinates.

- `void WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)`
	- Renders a character using the provided bitmap font.
	- Params: position `(x,y)`, ASCII `ch`, `font` (`Font_7x10`, `Font_11x18`, `Font_16x26`), `color`, `bgcolor`.
	- Returns: none
	- Errors: Character codes below 32 are not supported; font indexing assumes `ch >= 32`.

- `void WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)`
	- Renders a zero‑terminated string left‑to‑right, advancing by `font.width` per character.
	- Params: `(x,y)`, `str`, `font`, `color`, `bgcolor`
	- Returns: none
	- Errors: No bounds clipping beyond what per‑character drawing performs; strings can extend off‑screen.

- `void WriteData(uint8_t* data, uint16_t size)`
	- Sends a data buffer with DC set to data mode. Useful for low‑level operations.
	- Params: `data` pointer, `size` bytes
	- Returns: none
	- Errors: If SPI is busy, write waits; incorrect size/pointer leads to undefined behavior.

### Color Constants (RGB565)
`COLOR_BLACK`, `COLOR_WHITE`, `COLOR_RED`, `COLOR_CYAN`, `COLOR_MAGENTA`, `COLOR_GREEN`, `COLOR_BLUE`, `COLOR_YELLOW`, `COLOR_PINK`, `COLOR_PURPLE`, `COLOR_ORANGE`.

### Fonts
Defined in `fonts.h`:
```cpp
typedef struct {
	const uint8_t width;
	uint8_t height;
	const uint16_t *data;
} FontDef;
extern FontDef Font_7x10, Font_11x18, Font_16x26;
```

## Example Usage
Minimal demo similar to `main_tst.cpp`:
```cpp
#include "TFT_ST7735.h"

int main() {
	TFT_ST7735 tft;
	tft.INIT_FN();

	tft.FillScreen(COLOR_WHITE);
	tft.FillRectangle(10, 10, 50, 30, COLOR_BLUE);
	tft.DrawPixel(20, 20, COLOR_RED);
	tft.WriteString(5, 50, "Hello", Font_11x18, COLOR_BLACK, COLOR_WHITE);
	while (1) {}
}
```

## Error Conditions & Tips
- **No display output**: Check `CS/DC/RST/SCK/MOSI` wiring and 3.3V power. Ensure `INIT_FN()` is called after constructor.
- **Garbage/tearing**: Verify SPI mode and timing; current setup uses 8‑bit frames and software‑managed DC/CS.
- **Out‑of‑bounds coords**: `FillRectangle` clips; `DrawPixel` assumes valid coordinates.
- **Blocking writes**: SPI polling waits until `TXE` and `BSY` are clear; operations are synchronous.

## Internals (Advanced)
- `INIT_FN()` drives `RST`, asserts `CS` once, and pushes an encoded init sequence (`init_cmd[]`) through `WriteCommand(uint16_t*, uint16_t)` which multiplexes data/commands via bit 8 in the 16‑bit entries.
- `SetAddressWindow(x0,y0,x1,y1)` issues `CASET`, `RASET`, `RAMWR` and prepares subsequent data writes.
- GPIO and SPI configuration live in `confRCC()`, `confGPIO()`, `confSPI()`, called by `config()` in the constructor.

## Midterm Project Concept

### How to Test
- Power the board, connect TFT per wiring above.
- Flash the firmware. Verify startup screen and subsequent updates every sampling interval.

### Limitations
- Uses polling; CPU is busy during large fills and string rendering.
- No DMA; SPI throughput is limited.
- No clipping for `WriteString`; text can run off‑screen.
- Fixed rotation/memory access settings in init sequence.

### Possible Improvements
- Add SPI DMA for faster area fills and text drawing.
- Add text clipping and word‑wrap utilities.
- Support multiple rotations and offsets.
- Double‑buffer drawing primitives; batch pixels.
- Add drawing primitives: lines, circles, images (bitmaps).


## Included Midterm Project: Oscilloscope Demo

File: `src/main_osc.cpp`

- **Purpose**: Demonstrate a functional prototype that reads an analog sensor via `ADC1` and plots the sampled waveform live on the ST7735, behaving like a simple oscilloscope.
- **Initialization**:
	- `conf_osc()` sets up HSE and PLL for higher system clock, configures `ADC1` on `PA0` (analog), starts continuous conversion.
	- `Screen1.INIT_FN()` initializes the TFT.
- **Operation**:
	- Clears the drawing area, draws axes (X and Y) using `FillRectangle`.
	- In the loop, reads `ADC1->DR`, scales the reading (`>>5`) to 7‑bit range for the 128‑pixel X axis.
	- Increments a vertical index `iterative` as time; plots `DrawPixel(adc_val+3, iterative+3, COLOR_CYAN)`.
	- When `iterative` reaches 145 rows, clears the area and restarts, creating a scrolling plot.

### How to Test the Oscilloscope Demo
- Wire a sensor or signal source to `PA0` (e.g., potentiometer between 3.3V and GND with wiper to `PA0`).
- Connect the TFT per wiring section.
- Build and flash the firmware that uses `main_osc.cpp` as the entry point.
- Observe the cyan trace scrolling vertically; adjust the signal and see horizontal movement corresponding to amplitude.

### Limitations (Oscilloscope Demo)
- Sampling is continuous and untriggered; the vertical axis is time index, not scaled to real units.
- No horizontal clipping or scaling beyond a simple right‑shift; signal mapping is coarse.
- Screen clears every 145 rows; no buffer or persistence.
- CPU performs polling for SPI; large draws are blocking.

### Possible Improvements (Oscilloscope Demo)
- Add triggers (rising edge) and horizontal/vertical scaling controls.
- Implement SPI DMA for faster pixel plotting.
- Add grid/labels and numerical readouts (min/max/mean).
- Use a ring buffer to plot without full clears; add decimation.
