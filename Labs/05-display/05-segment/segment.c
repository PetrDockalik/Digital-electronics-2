/***********************************************************************
 * 
 * Seven-segment display library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#define F_CPU 16000000
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
// Active-low digit 0 to 9
uint8_t segment_value[] = {
	// abcdefgDP
	0b00000011,		 // Digit 0
	0b10011111,      // Digit 1
	0b00100101,      // Digit 2
	0b00001101,		 // Digit 3
	0b10011001,      // Digit 4
	0b01001001,		 // Digit 5
	0b01000001,		 // Digit 6
	0b00011111,		 // Digit 7
	0b00000001,	     // Digit 8
	0b00001001		 // Digit 9
};

// Active-high position 0 to 3
uint8_t segment_position[] = {
	// p3p2p1p0....
	0b00010000,     // Position 0
	0b00100000,     // Position 1
	0b01000000,     // ...
	0b10000000
};

// uint8_t snake_value[] = {  //6 mo�nost�
// 	// snake
// 	0b01111111,			
// 	0b10111111,       
// 	0b11011111,      
// 	0b11101111,			
// 	0b11110111,          
// 	0b11111011,						
//};

/* Function definitions ----------------------------------------------*/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEGMENT_LATCH);
    GPIO_config_output(&DDRD, SEGMENT_CLK);
    GPIO_config_output(&DDRB, SEGMENT_DATA);
}

/*--------------------------------------------------------------------*/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
	uint8_t bit_number;
	segments = segment_value[segments];     // 0, 1, ..., 9
	//segments = snake_value[segments];   
	position = segment_position[position];  // 0, 1, 2, 3

	GPIO_write_low(&PORTD,SEGMENT_LATCH);
	GPIO_write_low(&PORTD,SEGMENT_CLK);
	GPIO_write_low(&PORTB,SEGMENT_DATA);
    // Pull LATCH, CLK, and DATA low
	_delay_us(1);
    // Wait 1 us

    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
		if ((segments & 1) == 0)
		{
			GPIO_write_low(&PORTB,SEGMENT_DATA);
		}
		else
		{
			GPIO_write_high(&PORTB,SEGMENT_DATA);
		}
        // Output DATA value (bit 0 of "segments")
		
		_delay_us(1);
        // Wait 1 us
		GPIO_write_high(&PORTD,SEGMENT_CLK);
        // Pull CLK high
		_delay_us(1);
        // Wait 1 us
		GPIO_write_low(&PORTD,SEGMENT_CLK);
        // Pull CLK low

        // Shift "segments"
        segments = segments >> 1;
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {	
		if ((position % 2) == 0)
		{
			GPIO_write_low(&PORTB,SEGMENT_DATA);
		}
		else
		{
			GPIO_write_high(&PORTB,SEGMENT_DATA);
		}
        // Output DATA value (bit 0 of "position")
		
		_delay_us(1);
        // Wait 1 us
		GPIO_write_high(&PORTD,SEGMENT_CLK);
        // Pull CLK high
		_delay_us(1);
        // Wait 1 us
		GPIO_write_low(&PORTD,SEGMENT_CLK);
        // Pull CLK low

        // Shift "position"
        position = position >> 1;
    }
	GPIO_write_high(&PORTD,SEGMENT_LATCH);
    // Pull LATCH high
	_delay_us(1);
    // Wait 1 us

}

/*--------------------------------------------------------------------*/
void SEG_clear(void)
{
	uint8_t bit_number;
	uint8_t bit_number2;
	uint8_t segments;
	uint8_t position=0;
	segments = segment_value[0];
	position = segment_position[position];
	
	GPIO_write_low(&PORTD,SEGMENT_LATCH);
	GPIO_write_low(&PORTD,SEGMENT_CLK);
	GPIO_write_low(&PORTB,SEGMENT_DATA);
	// Pull LATCH, CLK, and DATA low
	_delay_us(1);
	// Wait 1 us
	
	for (bit_number2 = 0; bit_number2 < 4; bit_number2++)
	{
		GPIO_write_low(&PORTD,SEGMENT_LATCH);
		GPIO_write_low(&PORTD,SEGMENT_CLK);
		GPIO_write_low(&PORTB,SEGMENT_DATA);
		// Pull LATCH, CLK, and DATA low
		_delay_us(1);
		// Wait 1 us

		// Loop through the 1st byte (segments)
		// a b c d e f g DP (active low values)
		for (bit_number = 0; bit_number < 8; bit_number++)
		{
			if ((segments & 1) == 0)
			{
				GPIO_write_low(&PORTB,SEGMENT_DATA);
			}
			else
			{
				GPIO_write_high(&PORTB,SEGMENT_DATA);
			}
			// Output DATA value (bit 0 of "segments")
		
			_delay_us(1);
			// Wait 1 us
			GPIO_write_high(&PORTD,SEGMENT_CLK);
			// Pull CLK high
			_delay_us(1);
			// Wait 1 us
			GPIO_write_low(&PORTD,SEGMENT_CLK);
			// Pull CLK low

			// Shift "segments"
			segments = segments >> 1;
		}

		// Loop through the 2nd byte (position)
		// p3 p2 p1 p0 . . . . (active high values)
		for (bit_number = 0; bit_number < 8; bit_number++)
		{	
			if ((position % 2) == 0)
			{
				GPIO_write_low(&PORTB,SEGMENT_DATA);
			}
			else
			{
				GPIO_write_high(&PORTB,SEGMENT_DATA);
			}
			// Output DATA value (bit 0 of "position")
		
			_delay_us(1);
			// Wait 1 us
			GPIO_write_high(&PORTD,SEGMENT_CLK);
			// Pull CLK high
			_delay_us(1);
			// Wait 1 us
			GPIO_write_low(&PORTD,SEGMENT_CLK);
			// Pull CLK low

			// Shift "position"
			position = position << 1;
		}
		GPIO_write_high(&PORTD,SEGMENT_LATCH);
		// Pull LATCH high
		_delay_us(1);
		// Wait 1 us
	
	}

}
/*--------------------------------------------------------------------*/
void SEG_clk_2us()
{
	
}