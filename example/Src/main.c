#include "stm32f4xx.h"
#include "systick.h"
#include "spi.h"
#include "st7735s.h"

int main(void)
{
	systick_init();
	spi1_init();

	st7735s_t st7735s = {
			.spi_transmit = spi1_transmit,
			.A0_DC_Data = A0_DC_Data,
			.A0_DC_Command = A0_DC_Command,
			.cs_enable = cs_enable,
			.cs_disable = cs_disable,
			.reset_high = reset_pin_high,
			.reset_low = reset_pin_low,
			.delay_ms = delay_ms,
			.display_height = 160,
			.display_width = 128,
	};

	st7735_init(&st7735s);

	uint16_t color =  st7735_color565(255, 0, 255);

	st7735_fill_screen(&st7735s, color);


	while (1)
	{

	}
}
