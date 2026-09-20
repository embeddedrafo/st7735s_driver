#ifndef ST7735S_H_
#define ST7735S_H_

#include <stdint.h>

/* ====================== COMMANDS ======================= */

typedef enum {
    SWRESET = 0x01,
    SLPIN   = 0x10,
    SLPOUT  = 0x11,
    PTLON   = 0x12,
    NORON   = 0x13,
    INVOFF  = 0x20,
    INVON   = 0x21,
    GAMSET  = 0x26,
    DISPOFF = 0x28,
    DISPON  = 0x29,
    CASET   = 0x2A,
    RASET   = 0x2B,
    RAMWR   = 0x2C,
    RGBSET  = 0x2D,
    PTLAR   = 0x30,
    SCRLAR  = 0x33,
    TEOFF   = 0x34,
    TEON    = 0x35,
    MADCTL  = 0x36,
    VSCSAD  = 0x37,
    IDMOFF  = 0x38,
    IDMON   = 0x39,
    COLMOD  = 0x3A
} st7735_cmd_t;

/* ======================= COLORS ======================== */

typedef enum {
    COLOR_BLACK    	 = 0x0000,
    COLOR_WHITE    	 = 0xFFFF,
    COLOR_RED      	 = 0xF800,
    COLOR_GREEN    	 = 0x07E0,
    COLOR_BLUE     	 = 0x001F,
    COLOR_YELLOW   	 = 0xFFE0,
    COLOR_CYAN    	 = 0x07FF,
    COLOR_MAGENTA  	 = 0xF81F,
    COLOR_ORANGE   	 = 0xFD20,
    COLOR_PURPLE   	 = 0x780F,
    COLOR_BROWN    	 = 0xA145,
    COLOR_NAVY     	 = 0x000F,
    COLOR_TEAL     	 = 0x0410,
    COLOR_OLIVE    	 = 0x7BE0,
    COLOR_GRAY     	 = 0x8410,
    COLOR_DARK_GRAY  = 0x4208,
    COLOR_LIGHT_GRAY = 0xC618,
} color565_t;

/* ==================== DRIVER HANDLE ==================== */

typedef void (*spi_transmit_t)(const uint8_t *data, uint32_t size);
typedef void (*A0_DC_Data_t)(void);
typedef void (*A0_DC_Command_t)(void);
typedef void (*cs_enable_t)(void);
typedef void (*cs_disable_t)(void);
typedef void (*reset_high_t)(void);
typedef void (*reset_low_t)(void);
typedef void (*delay_ms_t)(uint32_t ms);

typedef struct
{
    spi_transmit_t      spi_transmit;
    A0_DC_Data_t        A0_DC_Data;
    A0_DC_Command_t     A0_DC_Command;
    cs_enable_t         cs_enable;
    cs_disable_t        cs_disable;
    reset_high_t        reset_high;
    reset_low_t         reset_low;
    delay_ms_t          delay_ms;
    uint16_t			display_width;
    uint16_t    		display_height;
} st7735s_t;

void st7735_init(const st7735s_t *handler);
void st7735_write_command(const st7735s_t *handler, st7735_cmd_t cmd);
void st7735_write_data(const st7735s_t *handler, const uint8_t *data, uint32_t size);
void st7735_reset(const st7735s_t *handler);
void st7735_set_window(const st7735s_t *handler, uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);

void st7735_draw_pixel(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t color);
void st7735_push_pixels(const st7735s_t *handler, const uint16_t *pixels, uint32_t count);
void st7735_fill_rect(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void st7735_fill_screen(const st7735s_t *handler, uint16_t color);
void st7735_draw_hline(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void st7735_draw_vline(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t length, uint16_t color);

//void st7735_draw_line(const st7735s_t *handler, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
//void st7735_draw_rect(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
//void st7735_draw_circle(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t r, uint16_t color);
//void st7735_fill_circle(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t r, uint16_t color);
//void st7735_draw_triangle(const st7735s_t *handler, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
//void st7735_fill_triangle(const st7735s_t *handler, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

//void st7735_draw_char(const st7735s_t *handler, uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t size);
//void st7735_draw_string(const st7735s_t *handler, uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color, uint8_t size);
void st7735_draw_bitmap(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap);

//void st7735_set_rotation(const st7735s_t *handler, uint8_t rotation);
//void st7735_invert_colors(const st7735s_t *handler, uint8_t invert);
//void st7735_sleep(const st7735s_t *handler);
//void st7735_wake(const st7735s_t *handler);

uint16_t st7735_color565(uint8_t r, uint8_t g, uint8_t b);

#endif /* ST7735S_H_ */
