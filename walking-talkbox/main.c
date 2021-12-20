#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

#include "ledstrip.h"

int volume = 10;

uint8_t red = 0xFF;
uint8_t green = 0x00;
uint8_t blue = 0x00;

int position = 0;

enum {INCREMENT_R, DECREMENT_R, INCREMENT_G, DECREMENT_G, INCREMENT_B, DECREMENT_B};
int colorCycle = INCREMENT_G;

void colors(int pos)
{
	writeLEDBlock(pos, red, green, blue);
	if (colorCycle == INCREMENT_G)
	{
		if (green == 0xFF)
		{
			colorCycle = DECREMENT_R;
		}
		else
		{
			green++;
		}
	}
	else if (colorCycle == DECREMENT_R)
	{
		if (red == 0x00)
		{
			colorCycle = INCREMENT_B;
		}
		else
		{
			red--;
		}
	}
	else if (colorCycle == INCREMENT_B)
	{
		if (blue == 0xFF)
		{
			colorCycle = DECREMENT_G;
		}
		else
		{
			blue++;
		}
	}
	else if (colorCycle == DECREMENT_G)
	{
		if(green == 0x00)
		{
			colorCycle = INCREMENT_R;
		}
		else
		{
			green--;
		}
	}
	else if (colorCycle == INCREMENT_R)
	{
		if (red == 0xFF)
		{
			colorCycle = DECREMENT_B;
		}
		else
		{
			red++;
		}
	}
	else if (colorCycle == DECREMENT_B)
	{
		if (blue == 0x00)
		{
			colorCycle = INCREMENT_G;
		}
		else
		{
			blue--;
		}
	}
}

void initialize()
{	
//	SOFT-POT INTERNAL PULLUP
	PORTC |= (1 << PORTC0);

//	OCTAVE SWITCH 1
	DDRD &= ~(1 << DDD7);
	PORTD |= (1 << PORTD7);
	
//	OCTAVE SWITCH 2
	DDRD &= ~(1 << DDD6);
	PORTD |= (1 << PORTD6);

//	OC0B OUT	
	DDRD |= (1 << DDD5);

//	TIMER0 SETUP
	TCCR0B &= ~(1 << CS00);
	TCCR0B &= ~(1 << CS01);
	TCCR0B |= (1 << CS02);

	TCCR0A |= (1 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << WGM02);

	TCCR0A |= (1 << COM0A0);
	TCCR0A &= ~(1 << COM0A1);
	OCR0A = 255;
	
	TCCR0A &= ~(1 << COM0B0);
	TCCR0A |= (1 << COM0B1);
	OCR0B = 1;
		
//	ADC SETUP
	PRR &= ~(1 << PRADC);
	
	// Vcc voltage reference
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	
	// 128 prescale = 125 kHz
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);
	
	// ADC0
	ADMUX &= ~(1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
	
	// Auto trigger
	ADCSRA |= (1 << ADATE);
	
	// Free running
	ADCSRB &= ~(1 << ADTS0);
	ADCSRB &= ~(1 << ADTS1);
	ADCSRB &= ~(1 << ADTS2);
	
	// Digital input disable
	DIDR0 |= (1 << ADC0D);
	DIDR0 |= (1 << ADC1D);
	
	// Enable ADC
	ADCSRA |= (1 << ADEN);
	
	// Start conversion
	ADCSRA |= (1 << ADSC);
	
	LEDInitialize();
}

int main(void)
{
	initialize();
	while(1)
	{
		// OCTAVE CHECK
		if(!(PIND & ( 1 << PIND7)))
		{
			TCCR0B |= (1 << CS00);
			TCCR0B &= ~(1 << CS01);
			TCCR0B |= (1 << CS02);
		}
		else if (!(PIND & (1 << PIND6)))
		{
			TCCR0B &= ~(1 << CS00);
			TCCR0B &= ~(1 << CS01);
			TCCR0B |= (1 << CS02);
		}
				
		// PLAY NOTE IF TOUCHED
		if (ADC < 820)
		{
			float ADC_temp = 0;
			for (int i = 0; i < 500; i++)
			{
				ADC_temp += ADC;
			}
			float ADC_avg = ADC_temp / 500;
			OCR0A = ADC_avg - 510;
			OCR0B = (float)OCR0A / volume;
			int position = 30 - (OCR0A * ((float)30 / 255)) + 3;
			colors(position);
 		}
		else
		{
			resetLED();
			OCR0A = 0;
			OCR0B = 0;
		}
	}
}