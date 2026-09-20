#ifndef SPI_H_
#define SPI_H_

void spi1_init(void);
void spi1_transmit(const uint8_t *data, uint32_t size);
void A0_DC_Data(void);
void A0_DC_Command(void);
void cs_enable(void);
void cs_disable(void);
void reset_pin_high(void);
void reset_pin_low(void);

#endif /* SPI_H_ */
