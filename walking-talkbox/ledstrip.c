/*
 * ledstrip.c
 *
 * Created: 15-Dec-21 14:53:32
 *  Author: maxwl
 */ 
/*
 * spi_practice.c
 *
 * Created: 14-Dec-21 20:43:21
 * Author : maxwl
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "ledstrip.h"

uint8_t brightness = 0xFF;
uint32_t color = 0xFF0000;

void sendValue(uint8_t val)
{
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if(!!(val & (1 << (7 - i))) == 0)
		{
			PORTB &= ~(1 << PORTB3);
		}
		else
		{
			PORTB |= (1 << PORTB3);
		}
		PORTB |= (1 << PORTB5);
		PORTB &= ~(1 << PORTB5);
	}
}

void start()
{
	for(int i = 0; i < 4; i++)
	{
		sendValue(0);
	}
}

void writeLED(uint8_t pos, uint8_t brightness, uint8_t R, uint8_t G, uint8_t B)
{
	start();
	for(int i = 0; i < NUM_LEDS; i++)
	{
		sendValue(brightness);
		if(i == pos)
		{
			sendValue(B);
			sendValue(G);
			sendValue(R);
		}
		else
		{
			sendValue(0x00);
			sendValue(0x00);
			sendValue(0x00);
		}
	}
}

void writeLEDBlock(uint8_t center_pos, uint8_t R, uint8_t G, uint8_t B)
{
	start();
	for(int i = 0; i < NUM_LEDS; i++)
	{
		if(i == center_pos)
		{
			sendValue(0xFF);
			sendValue(B);
			sendValue(G);
			sendValue(R);
		}
		else if (i == center_pos - 1 && center_pos > 0)
		{
			sendValue(0xEF);
			sendValue(B);
			sendValue(G);
			sendValue(R);
		}
		else if (i == center_pos + 1 && center_pos < NUM_LEDS - 1)
		{
			sendValue(0xEF);
			sendValue(B);
			sendValue(G);
			sendValue(R);
		}
		else if (i == center_pos - 2 && center_pos > 1)
		{
			sendValue(0xE1);
			sendValue(B);
			sendValue(G);
			sendValue(R);
		}
		else if (i == center_pos + 2 && center_pos < NUM_LEDS - 2)
		{
			sendValue(0xE1);
			sendValue(B);
			sendValue(G);
			sendValue(R);
		}
		else
		{
			sendValue(0xFF);
			sendValue(0x00);
			sendValue(0x00);
			sendValue(0x00);
		}
	}
}

void resetLED()
{
	sendValue(0x00);
	sendValue(0x00);
	sendValue(0x00);
	sendValue(0x00);
	
	for(int i = 0; i < NUM_LEDS; i++)
	{
		sendValue(0xFF);
		sendValue(0x00);
		sendValue(0x00);
		sendValue(0x00);
	}
}

void LEDInitialize()
{
	DDRB |= (1 << DDB3);
	DDRB |= (1 << DDB5);
	
	resetLED();
}

// int main(void)
// {
// 	initialize();
//     while (1) 
//     {		
// int r = 0xFF;
// int g = 0xFF;
// int b = 0xFF;
// 		for(int i = 0; i < NUM_LEDS; i++)
// 		{
// 			writeLED(i, 0xFF, r, g, b);
// 			_delay_ms(10);
// 			g -= 255 / NUM_LEDS;
// 		}
// 		for(int i = NUM_LEDS - 1; i >= 0; i--)
// 		{
// 			writeLED(i, 0xFF, r, g, b);
// 			_delay_ms(10);
// 		}
//     }
// }

