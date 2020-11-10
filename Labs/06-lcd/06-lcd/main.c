/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function


uint8_t zobak[8] = {
	0b00111,
	0b01110,
	0b11100,
	0b11000,
	0b11100,
	0b01110,
	0b00111,
	0b00011
};
	
uint8_t neco[8] = {
	0b11000,
	0b11000,
	0b11000,
	0b11111,
	0b11111,
	0b11000,
	0b11000,
	0b11000
};

uint8_t beziciobdelnik[] = { //Progress bar podle vás
	// addr 2:
	0b00000, 
	0b00000, 
	0b00000, 
	0b00000, 
	0b00000, 
	0b00000, 
	0b00000, 
	0b00000,
	
	// addr 3: 
	0b10000, 
	0b10000, 
	0b10000, 
	0b10000, 
	0b10000, 
	0b10000, 
	0b10000, 
	0b10000,
	
	// addr 4:
	0b11000, 
	0b11000, 
	0b11000, 
	0b11000, 
	0b11000, 
	0b11000, 
	0b11000, 
	0b11000,
	
	// addr 5: 
	0b11100, 
	0b11100, 
	0b11100, 
	0b11100, 
	0b11100, 
	0b11100, 
	0b11100, 
	0b11100,
	
	// addr 6: 
	0b11110, 
	0b11110, 
	0b11110, 
	0b11110, 
	0b11110, 
	0b11110, 
	0b11110, 
	0b11110,
	
	// addr 7: 
	0b11111, 
	0b11111, 
	0b11111, 
	0b11111, 
	0b11111, 
	0b11111, 
	0b11111, 
	0b11111
};
	uint8_t position = 1;
/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

	lcd_gotoxy(8, 0); //Mé dva znaky
	lcd_putc(0xFF);
	lcd_gotoxy(9, 0);
	lcd_putc(0x01);
	
     // Set pointer to beginning of CGRAM memory
     lcd_command(1 << LCD_CGRAM);
     for (uint8_t i = 0; i < 8; i++)
     {
	     // Store all new chars to memory line by line
	     lcd_data(zobak[i]);
     }
	 
	 for (uint8_t i = 0; i < 8; i++)
	 {
		 // Store all new chars to memory line by line
		 lcd_data(neco[i]);
	 }
	 
	 for (uint8_t i = 0; i < 45; i++)
	 {
		 // Store all new chars to memory line by line
		 lcd_data(beziciobdelnik[i]);
	 }
     // Set DDRAM address
     lcd_command(1 << LCD_DDRAM);
	 
	 
    // Put string(s) at LCD display
    //lcd_gotoxy(1, 0);
    //lcd_puts("LCD Test");
    //lcd_putc('!');

    // Configure 8-bit Timer/Counter2 for Stopwatch
	TIM2_overflow_16ms();
    // Enable interrupt and set the overflow prescaler to 16 ms
    TIM2_overflow_interrupt_enable();


	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();    
	// Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
ISR(TIMER1_OVF_vect)
{
    //uint8_t running_text[] = "Digital electronics 2"; //Na to jsem nepøišel.
	
	//lcd_gotoxy(11, 1);
	//lcd_puts(running_text);
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 2; //Uložen první znak progress baru

	lcd_gotoxy(position, 1); //Pozice, kde progress bar zaène
	lcd_putc(symbol); //Na displej
	
	symbol++; //Další znak
	if (symbol>7) //Použití našich 5 znakù
	{
		symbol=2;
	}
}

/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;        // Tenths of a second
	static uint8_t secs = 0;        // Seconds
	static uint8_t mins = 0;        // Minutes
	static uint16_t mocnina = 0;     // Second mocnina seconds
	char lcd_string[2] = "  ";      // String for converting numbers by itoa()

    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
		tens++; //Inkrementace vteøin
		position++; //Inkrementace pozice pro progress bar
		if (tens >9)
		{
			tens=0;
			position=1; //Zaèátek progress baru
			lcd_clrscr(); //Mazání displeje kvuli progress baru
			secs++; //Inkrementace sekund
			if (secs >9)
			{
				if (secs >59)
				{
					secs=0;
					mins++; //Inkrementace minut
					if (mins >59)
					{
						secs=0; //Vynulování stopek pøi pøeteèení minut
						mins=0;
						
					}
				}
			}
		}
		
		
		
		itoa(tens,lcd_string,10); //Zobrazení vteøin na displej
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		
		
		//lcd_gotoxy(tens+1, 1); //Lehèí možnost progress baru z obdelníèkù v ASCII
		//lcd_putc(0xFF);
		
		
		
		if (secs <10) //Podmínka pro zobrazení sekund na displeji a aby jsme nemuseli èistit stále displej
		{ 
			lcd_gotoxy(5, 0);
			itoa(secs,lcd_string,10);
			lcd_puts(lcd_string);
			
			lcd_gotoxy(4, 0);
			lcd_putc('0');
		} 
		else
		{
			lcd_gotoxy(4, 0);
			itoa(secs,lcd_string,10);
			lcd_puts(lcd_string);
		}
		
		
		
		if (mins <10) //Podmínka pro zobrazení minut na displeji a aby jsme nemuseli èistit stále displej
		{
			lcd_gotoxy(2, 0);
			itoa(mins,lcd_string,10);
			lcd_puts(lcd_string);
			
			lcd_gotoxy(1, 0);
			lcd_putc('0');
		}
		else
		{
			lcd_gotoxy(1, 0);
			itoa(mins,lcd_string,10);
			lcd_puts(lcd_string);
			
		}
		
		
		
		
		
		mocnina=secs*secs; //Pozice písmena b,-druhá mocnina sekund
		itoa(mocnina,lcd_string,10);
		lcd_gotoxy(11, 0);
		lcd_puts(lcd_string);
		
		lcd_gotoxy(3, 0); //Pøi èistìní displeje nám zmizí tyto znaky
		lcd_putc(':');
		lcd_gotoxy(6, 0);
		lcd_putc('.');	
    }
}