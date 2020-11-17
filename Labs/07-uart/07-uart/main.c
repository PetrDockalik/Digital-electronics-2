/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
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
#include "uart.h"           // Peter Fleury's UART library

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 0); lcd_puts("value:");
    lcd_gotoxy(2, 1); lcd_puts("key:");
	lcd_gotoxy(14, 0); lcd_putc('O');
	lcd_gotoxy(15, 0); lcd_putc('E');
    //lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    //lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    //lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
    // Set input channel to ADC0
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1)  | (1 << MUX0));
    // Enable ADC module
	ADCSRA |= (1 << ADEN);
    // Enable conversion complete interrupt
	ADCSRA |= (1 << ADIE);
    // Set clock prescaler to 128
	ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

    // Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,16000000)); 
	uart_puts("Parita:           Sudá  Lichá\n");

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
/**
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= (1 << ADSC); //Start ADC
}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{	
	static uint16_t value;
	static uint16_t value2;
	static uint8_t number_of_ones; 
	static char lcd_string[10];      // String for converting numbers by itoa()
    
	
	
	value=ADC; //Load ADC value
	value2=ADC;
	
	lcd_gotoxy(7, 1); //Case: Keys to LCD and UART 
	if (value == 0)  //Right key
	{
		lcd_puts("Right");
		uart_puts("Key: Right");
	} 
	else if (value == 101) //Up key
	{
		lcd_puts("Up");
		uart_puts("Key: Up");
	} 
	else if (value == 245) //Down key
	{
		lcd_puts("Down");
		uart_puts("Key: Down");
	}
	else if (value == 402) //Left key
	{
		lcd_puts("Left");
		uart_puts("Key: Left");
	}
	else if (value == 650) //Select key
	{
		lcd_puts("Select");
		uart_puts("Key: Select");
	}
	else //Nothing pressed
	{
		lcd_puts("      ");
	}

	
	
	for(uint16_t i=512;i==1;i=i/2) //Test ADC value- finding the number of ones
	{
		if (value2>i) 
		{
			number_of_ones++; //We find One
			value2=value2-i; //Number reduction
		}
	}
	number_of_ones=number_of_ones%2; //Parity- division with the remainder



	if (value>1015)	//Nothing pressed
	{
		lcd_gotoxy(6, 0);
		lcd_puts("    ");
		lcd_gotoxy(10,0);
		lcd_puts("   ");
		lcd_gotoxy(14,1);
		lcd_puts("  ");
	}
	else //Pressed one of those buttons
	{	//value ADC to LCD and UART
		itoa(value,lcd_string,10); 
		lcd_gotoxy(6, 0);
		lcd_puts(lcd_string);
		uart_puts("  Value: ");
		uart_puts(lcd_string);	
		
		uart_puts("  ");
		value=(value/1024)*5;
		itoa(value,lcd_string,10);
		uart_puts(lcd_string);
		uart_puts(" mV");
		
		value=ADC;
		itoa(value,lcd_string,16);
		lcd_gotoxy(10,0);
		lcd_puts(lcd_string);
		
	}


	if (value<1015) //Pressed one of those buttons
	{
		if (number_of_ones==0) //Odd number of ones
		{
			lcd_gotoxy(14, 1); //Odd
			lcd_putc('0');
			lcd_gotoxy(15, 1); //Even
			lcd_putc('1');
			uart_puts("  0  1\n"); //Parity bits to Serial
		
		
		
			//uart_puts("S paritou:     ");	Pøi tomto dalším pøidávání se mi zasekne asi program v SimulIDE, zkoušel jsem toho dost i s bufferem, nìco se mu asi nelíbí.
			//itoa(value,lcd_string,10); //Odd
			//uart_puts(lcd_string);
			//uart_puts("  ");
			//itoa(2048+value,lcd_string,10); //Even
			//uart_puts(lcd_string);
			//uart_puts("\n\r");
		}
		else //Even number of ones
		{
			lcd_gotoxy(14, 1);
			lcd_putc('1');
			lcd_gotoxy(15, 1);
			lcd_putc('0');
			uart_puts("  1  0\n");
		}
	}

	ADCSRA &= ~(1 << ADSC); //Stop ADC
	ADC=0; 
}
