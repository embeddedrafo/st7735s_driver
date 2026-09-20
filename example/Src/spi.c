#include "stm32f4xx.h"

#define GPIOA_EN	 (1U << 0)
#define GPIOB_EN	 (1U << 1)
#define SPI1_EN		 (1U << 12)

#define CR1_CPHA	 (1U << 0)
#define	CR1_CPOL	 (1U << 1)
#define CR1_MSTR	 (1U << 2)
#define CR1_SPE		 (1U << 6)
#define CR1_LSBFIRST (1U << 7)
#define CR1_SSI		 (1U << 8)
#define CR1_SSM		 (1U << 9)
#define CR1_RXONLY	 (1U << 10)
#define CR1_DFF		 (1U << 11)
#define CR1_BIDIOE	 (1U << 14)
#define CR1_BIDIMODE (1U << 15)

#define SR_TXE		 (1U << 1)
#define SR_BSY		 (1U << 7)

/*
 * GPIOA5 	-> SPI1_SCK
 * GPIOA6 	-> SPI1_MISO (not used)
 * GPIOA7 	-> SPI1_MOSI (SDA)
 *
 * GPIOB0 	-> CS
 * GPIOB1 	-> A0 (DC)
 * GPIOB10 	-> RESET
 */

void spi1_init(void)
{
	/*============= Config RCC ==========================*/

	/*Enable RCC for GPIOA and GPIOB*/
	RCC->AHB1ENR |= GPIOA_EN | GPIOB_EN;

	/*Enable RCC for SPI1*/
	RCC->APB2ENR |= SPI1_EN;

	/*============ Config GPIO ==========================*/

	/*GPIOA5 config as Alternate function mode */
	GPIOA->MODER &= ~(1U << 10);
	GPIOA->MODER |=  (1U << 11);

//	/*GPIOA5 config as Fast speed */
//	GPIOA->OSPEEDR &= ~(1U << 10);
//	GPIOA->OSPEEDR |=  (1U << 11);

	/*Config GPIOA5 alternate function register*/
	GPIOA->AFR[0] |=  (1U << 20);
	GPIOA->AFR[0] &= ~(1U << 21);
	GPIOA->AFR[0] |=  (1U << 22);
	GPIOA->AFR[0] &= ~(1U << 23);

	/*GPIOA7 config as Alternate function mode */
	GPIOA->MODER &= ~(1U << 14);
	GPIOA->MODER |=  (1U << 15);

//	/*GPIOA7 config as Fast speed */
//	GPIOA->OSPEEDR &= ~(1U << 14);
//	GPIOA->OSPEEDR |=  (1U << 15);

	/*Config GPIOA7 alternate function register*/
	GPIOA->AFR[0] |=  (1U << 28);
	GPIOA->AFR[0] &= ~(1U << 29);
	GPIOA->AFR[0] |=  (1U << 30);
	GPIOA->AFR[0] &= ~(1U << 31);

	/*GPIOB0 config as General purpose output mode */
	GPIOB->MODER |=  (1U << 0);
	GPIOB->MODER &= ~(1U << 1);

//	/*GPIOB0 config as Medium speed */
//	GPIOB->OSPEEDR |=  (1U << 0);
//	GPIOB->OSPEEDR &= ~(1U << 1);

	/*GPIOB1 config as General purpose output mode */
	GPIOB->MODER |=  (1U << 2);
	GPIOB->MODER &= ~(1U << 3);

//	/*GPIOB1 config as Medium speed */
//	GPIOB->OSPEEDR |=  (1U << 2);
//	GPIOB->OSPEEDR &= ~(1U << 3);

	/*GPIOB10 config as General purpose output mode */
	GPIOB->MODER |=  (1U << 20);
	GPIOB->MODER &= ~(1U << 21);

//	/*GPIOB10 config as Medium speed */
//	GPIOB->OSPEEDR |=  (1U << 20);
//	GPIOB->OSPEEDR &= ~(1U << 21);

	/*=============== SPI1 Config ================*/

	/*Baud rate control (fPCLK/2)*/
	SPI1->CR1 &= ~(1U << 3);
	SPI1->CR1 &= ~(1U << 4);
	SPI1->CR1 &= ~(1U << 5);

	/*Set CPOL=0, CPHA=0*/
	SPI1->CR1 &= ~CR1_CPOL;
	SPI1->CR1 &= ~CR1_CPHA;

	/*Enable full duplxe*/
	SPI1->CR1 &= ~CR1_RXONLY;

	/*Set BIDIMODE as 1-line bidirectional data mode*/
	SPI1->CR1 |= CR1_BIDIMODE;

	/*Set BIDIOE as transmit-only mode*/
	SPI1->CR1 |= CR1_BIDIOE;

	/*Data frame format (set 8-bit)*/
	SPI1->CR1 &= ~CR1_DFF;

	/*Set Frame format (MSB first)*/
	SPI1->CR1 &= ~CR1_LSBFIRST;

	/*Software slave management SSM=1, SSI=1*/
	SPI1->CR1 |= CR1_SSM;
	SPI1->CR1 |= CR1_SSI;

	/*Set mode as Master*/
	SPI1->CR1 |= CR1_MSTR;

	/*Enable SPI*/
	SPI1->CR1 |= CR1_SPE;
}

void spi1_transmit(const uint8_t *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		/*While until buffer be empty*/
		while(!(SPI1->SR & SR_TXE)){}

		/*Write data*/
		SPI1->DR = data[i];
	}

	/*While until buffer be empty*/
	while(!(SPI1->SR & SR_TXE)){}

	/*While until SPI not busy*/
	while(SPI1->SR & SR_BSY){}
}

void cs_enable(void)
{
	GPIOB->ODR &= ~(1U << 0);
}

void cs_disable(void)
{
	GPIOB->ODR |= (1U << 0);
}

void A0_DC_Command(void)
{
	GPIOB->ODR &= ~(1U << 1);
}

void A0_DC_Data(void)
{
	GPIOB->ODR |= (1U << 1);
}

void reset_pin_high(void)
{
	GPIOB->ODR |= (1U << 10);
}

void reset_pin_low(void)
{
	GPIOB->ODR &= ~(1U << 10);
}
