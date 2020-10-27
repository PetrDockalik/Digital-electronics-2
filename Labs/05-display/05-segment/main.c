/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
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
#include "segment.h"        // Seven-segment display library for AVR-GCC

// uint8_t jednotky=0;  // deklarace pomocných proměnných
// uint8_t desitky=0;
// uint8_t sto=0;
//uint8_t tisic=0;

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Display decimal 
 * counter values on SSD (Seven-segment display) when 16-bit 
 * Timer/Counter1 overflows.
 */
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Test of SSD: display number '3' at position 0
    //SEG_update_shift_regs(0b00001101, 0b00010000);
	//SEG_update_shift_regs(3, 0);

	//SEG_update_shift_regs(3, 2);
	//SEG_update_shift_regs(1, 3);

    /* Configure 16-bit Timer/Counter1*/
    /* Set prescaler and enable overflow interrupt */
	TIM1_overflow_interrupt_enable();
	TIM1_overflow_262ms();
	
	/* Configure 8-bit Timer/Counter0*/
	/* Set prescaler and enable overflow interrupt */
	//TIM0_overflow_4ms();
	//TIM0_overflow_interrupt_enable();
	
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
 * ISR starts when Timer/Counter1 overflows. Increment decimal counter
 * value and display it on SSD.
 */
ISR(TIMER1_OVF_vect)
{
	static uint8_t snake = 0;
	SEG_update_shift_regs(snake, 0);
    snake++;
	if (snake>5)
	{
		snake=0;
	}
}

// ISR(TIMER0_OVF_vect)
// {	
// 	static uint8_t pos = 0;
// 	if (pos==0)
// 	{
// 		SEG_update_shift_regs(jednotky,0);
// 		pos++;
// 	}
// 	else if (pos==1)
// 	{
// 		SEG_update_shift_regs(desitky,1);
// 		pos++;
// 	}
// 	else if (pos==2)
// 	{
// 		SEG_update_shift_regs(sto,2);
// 		pos++;
// 	}
// 	else if (pos==3)
// 	{
// 		SEG_update_shift_regs(tisic,3);
// 		pos=0;
// 	}
// }
