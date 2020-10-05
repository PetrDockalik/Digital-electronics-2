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
#define LED1   PB5     // AVR pin where green LED is connected
#define LED2   PC0     // AVR pin where red LED is connected
#define LED3   PC1
#define LED4   PC2
#define LED5   PC3
#define BUTTON   PD0 
#define BLINK_DELAY 800
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
    DDRB = DDRB | (1<<LED1);
    // ...and turn LED off in Data Register
    PORTB = PORTB | (1<<LED1); //Led off active high

    /* second LED */
	//DDRC= DDRC | (1<<LED2);	//Output
	//DDRC= DDRC | (1<<LED3);	//Output
	//DDRC= DDRC | (1<<LED4);	//Output
	DDRC= DDRC | (1<<LED5);	//Output
    PORTC = PORTC |(1<<LED2); //Led off Active low
	PORTC = PORTC |(1<<LED3); //Led off Active low
	PORTC = PORTC |(1<<LED4); //Led off Active low
	PORTC = PORTC & ~(1<<LED5); //Led off Active low
									 
	/* push button */
	DDRD= DDRD  & ~(1<<BUTTON);	//Input button - clear bit
	PORTD = PORTD | (1<<BUTTON);  //Pull-up - set a bit
    // WRITE YOUR CODE HERE

    // Infinite loop
    while (1)
    {
				PORTB = PORTB ^ (1<<LED1); //Invert LED value
				_delay_ms(BLINK_DELAY);
				// Pause several milliseconds
				PORTB = PORTB ^ (1<<LED1); //Invert LED value
				PORTC = PORTC ^ (1<<LED2);
				_delay_ms(BLINK_DELAY);
				PORTC = PORTC ^ (1<<LED2);
				PORTC = PORTC ^ (1<<LED3);
				_delay_ms(BLINK_DELAY);
				PORTC = PORTC ^ (1<<LED3);
				PORTC = PORTC ^ (1<<LED4);
				_delay_ms(BLINK_DELAY);
				PORTC = PORTC ^ (1<<LED4);
				PORTC = PORTC ^ (1<<LED5);
				_delay_ms(BLINK_DELAY);
				PORTC = PORTC ^ (1<<LED5);
		
    }

    // Will never reach this
    return 0;
}
