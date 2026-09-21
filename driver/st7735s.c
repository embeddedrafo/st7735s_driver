#include "st7735s.h"

void st7735_reset(const st7735s_t *handler)
{
	handler->reset_high();
	handler->delay_ms(1);
	handler->reset_low();
	handler->delay_ms(1);
	handler->reset_high();
	handler->delay_ms(10);
}

void st7735_write_command(const st7735s_t *handler, st7735_cmd_t cmd)
{
	handler->A0_DC_Command();
	handler->cs_enable();

	uint8_t cmd_byte = (uint8_t)cmd;
	handler->spi_transmit(&cmd_byte, 1);

	handler->cs_disable();
}

void st7735_write_data(const st7735s_t *handler,const uint8_t *data, uint32_t size)
{
	handler->A0_DC_Data();
	handler->cs_enable();
	handler->spi_transmit(data, size);
	handler->cs_disable();
}

void st7735_init(const st7735s_t *handler)
{
	st7735_reset(handler);

	st7735_write_command(handler, SWRESET);
	handler->delay_ms(150);

	st7735_write_command(handler, SLPOUT);
	handler->delay_ms(150);

	st7735_write_command(handler, INVOFF);

	st7735_write_command(handler, MADCTL);
	uint8_t madctl_data[1] = {0x00};
	st7735_write_data(handler, madctl_data, 1);

	st7735_write_command(handler, COLMOD);
	uint8_t colmod_data[1] = {0x05};
	st7735_write_data(handler, colmod_data, 1);

	st7735_write_command(handler, DISPON);
}

void st7735_set_window(const st7735s_t *handler, uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    uint8_t caset_data[4] = {xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF};
    uint8_t raset_data[4] = {ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF};

	st7735_write_command(handler, CASET);
	st7735_write_data(handler,caset_data, 4);

	st7735_write_command(handler, RASET);
	st7735_write_data(handler, raset_data, 4);
}

void st7735_draw_pixel(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t color)
{
	st7735_set_window(handler, x, y, x, y);

	uint8_t color_data[2] = {color >> 8, color & 0xFF};

	st7735_write_command(handler, RAMWR);
	st7735_write_data(handler, color_data, 2);
}

void st7735_push_pixels(const st7735s_t *handler, const uint16_t *pixels, uint32_t count)
{
	uint8_t color_buffer[128];
	uint32_t total_bytes = count * 2;
	uint32_t remainder = total_bytes % 128;
	uint32_t j = 0;

	st7735_write_command(handler, RAMWR);

	handler->cs_enable();
	handler->A0_DC_Data();

	for (uint32_t i = 0; i < total_bytes / 128; i++)
	{
		for (uint32_t k = 0; k < 128; k = k + 2)
		{
			color_buffer[k] = pixels[j] >> 8;
			color_buffer[k+1] = pixels[j] & 0xFF;
			j++;
		}

		handler->spi_transmit(color_buffer, 128);
	}

	for (uint32_t a = 0; a < remainder; a = a + 2)
	{
		color_buffer[a] = pixels[j] >> 8;
		color_buffer[a+1] = pixels[j] & 0xFF;
		j++;
	}

	if (remainder > 0)
	{
	    handler->spi_transmit(color_buffer, remainder);
	}

	handler->cs_disable();
}

void st7735_fill_rect(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	st7735_set_window(handler, x, y, (x + w) - 1, (y + h) - 1);

	uint8_t color_data[2] = {color >> 8, color & 0xFF};
	uint8_t color_buffer[128];
	uint32_t total_bytes =  w * h * 2;
	uint32_t remainder = total_bytes % 128;

	for (uint32_t i = 0; i < 128; i = i + 2)
	{
		color_buffer[i] = color_data[0];
		color_buffer[i+1] = color_data[1];
	}

	st7735_write_command(handler, RAMWR);

	handler->cs_enable();
	handler->A0_DC_Data();

    for (uint32_t block = 0; block < total_bytes / 128; block++)
    {
    	handler->spi_transmit(color_buffer, 128);
    }

    if (remainder > 0)
    {
    	handler->spi_transmit(color_buffer, remainder);
    }

	handler->cs_disable();
}

void st7735_fill_screen(const st7735s_t *handler, uint16_t color) {st7735_fill_rect(handler, 0, 0, handler->display_width , handler->display_height, color);}

void st7735_draw_hline(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
	if (length > handler->display_width){length = handler->display_width;};
	st7735_fill_rect(handler, x, y, length, 1, color);
}

void st7735_draw_vline(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
	if (length > handler->display_height){length = handler->display_height;};
	st7735_fill_rect(handler, x, y, 1, length, color);
}

uint16_t abs_custom(int16_t value)
{
	return (value > 0) ? value : -value;
}

void st7735_draw_line(const st7735s_t *handler, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;

	int16_t dirx = (dx < 0) ? -1 : 1;
	int16_t diry = (dy < 0) ? -1 : 1;

	dx = abs_custom(dx);
	dy = abs_custom(dy);

	if (dx >= dy)
	{
		int16_t y;
		int16_t p;

		if (dx != 0)
		{
			y = y0;
			p = 2*dy - dx;
			for (int16_t i = 0; i <= dx; i++)
			{
				st7735_draw_pixel(handler,x0 + dirx*i,y, color);

				if (p >= 0)
				{
					y += diry;
					p = p - 2*dx;
				}
				p = p + 2 * dy;
			}
		}
	}
	else
	{

		int16_t x;
		int16_t p;

		if (dy != 0)
		{
			x = x0;
			p = 2*dx - dy;
			for (int16_t i = 0; i <= dy; i++)
			{
				st7735_draw_pixel(handler,x ,y0 + diry*i, color);

				if (p >= 0)
				{
					x += dirx;
					p = p - 2*dy;
				}
				p = p + 2 * dx;
			}
		}
	}
}

void st7735_draw_bitmap(const st7735s_t *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap)
{
	st7735_set_window(handler, x, y, (x + w) - 1, (y + h) - 1);

	st7735_push_pixels(handler, bitmap, w * h);
}

uint16_t st7735_color565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}
