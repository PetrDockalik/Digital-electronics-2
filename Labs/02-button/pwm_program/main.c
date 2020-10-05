/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED   PB5     // AVR pin where green LED is connected
#define DUTY100  PD3     // AVR pin where red LED is connected
#define DUTY75   PD2
#define DUTY50   PD1
#define DUTY25   PD0 
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed.
 */
int main(void)
{
    /* GREEN LED */
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED); //Led off active high
									 
	/* push button */
	DDRD= DDRD  & ~(1<<DUTY25);	//Input button - clear bit
	PORTD = PORTD | (1<<DUTY25);  //Pull-up - set a bit
    DDRD= DDRD  & ~(1<<DUTY50);	//Input button - clear bit
	PORTD = PORTD | (1<<DUTY50);  //Pull-up - set a bit
	DDRD= DDRD  & ~(1<<DUTY75);	//Input button - clear bit
	PORTD = PORTD | (1<<DUTY75);  //Pull-up - set a bit
	DDRD= DDRD  & ~(1<<DUTY100);	//Input button - clear bit
	PORTD = PORTD | (1<<DUTY100);  //Pull-up - set a bit

    // Infinite loop
    while (1)
    {
		if(bit_is_clear(PIND,DUTY25))
		{
			loop_until_bit_is_clear(PIND,DUTY25);
				{PORTB = PORTB ^ (1<<LED);
				_delay_ms(250);
				PORTB = PORTB ^ (1<<LED); 
				_delay_ms(750);}
		}
		
		else if(bit_is_clear(PIND,DUTY50))
		{
			loop_until_bit_is_clear(PIND,DUTY50);
			{PORTB = PORTB ^ (1<<LED); // Led ON
				_delay_ms(500);
				PORTB = PORTB ^ (1<<LED); //Led OFF
			_delay_ms(500);}
		}
		
		else if(bit_is_clear(PIND,DUTY75))
		{
			loop_until_bit_is_clear(PIND,DUTY75);
			{PORTB = PORTB ^ (1<<LED);
				_delay_ms(750);
				PORTB = PORTB ^ (1<<LED); 
			_delay_ms(250);}
		}
		
		else if(bit_is_clear(PIND,DUTY100))
		{
			loop_until_bit_is_clear(PIND,DUTY100);
			{PORTB = PORTB ^ (1<<LED); 		
			}
			PORTB = PORTB ^ (1<<LED); 
		}		
		
	}
    // Will never reach this
    return 0;
}

