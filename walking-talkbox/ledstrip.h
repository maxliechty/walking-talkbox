/*
 * ledstrip.h
 *
 * Created: 15-Dec-21 14:53:14
 *  Author: maxwl
 */ 


#ifndef LEDSTRIP_H_
#define LEDSTRIP_H_

#define DDR_SPI DDRB
#define DD_MOSI DDB3
#define DD_SCK DDB5
#define NUM_LEDS 31

void LEDInitialize();
void start();
void sendValue(uint8_t val);
void writeLED(uint8_t pos, uint8_t brightness, uint8_t R, uint8_t G, uint8_t B);
void writeLEDBlock(uint8_t pos, uint8_t R, uint8_t G, uint8_t B);
void resetLED();



#endif /* LEDSTRIP_H_ */