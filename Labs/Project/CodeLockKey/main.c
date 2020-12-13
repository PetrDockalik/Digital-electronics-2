/*
 * CodeLockKey.c
 *
 * Created: 29.11.2020 22:14:37
 * Author : Petr Doèkalík, xdocka13, 211544
 */ 

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif
#include <util/delay.h>     //using _delay functions

#include "timer.h"			//timers and counters using
#include "lcd.h"            //lcd devide using
#include "uart.h"           //uart device using
#include "scan_keys.h"		//my library with function of scanning keypad
#include "gpio.h"           //input and outputs using


/* Variables ---------------------------------------------------------*/
uint8_t key; //key from First Column
uint8_t key2; //key from Second Column
uint8_t key3; //key from Third Column
uint8_t sum_key; //key from all column

char lcd_string[2]="  "; //Convert int to string
char lcd_string2[2]="  "; //Convert int to string to uart

uint8_t Password1[] = {3, 5, 6, 3}; //First user
uint8_t Password2[] = {4, 9, 6, 5}; //Second user
uint8_t Password3[] = {1, 2, 3, 1}; //Third user
uint8_t Newpassword[4] = {}; //variable to the entered password
	
uint8_t number_of_password=0; //4-digit password
uint8_t number_of_overflows2=0; //variable for a cycle after entering 4 numbers
uint8_t i=0; //variable for buzzer cycle

int main(void)
{
	/*Initializing Keypad device*/
	KEYPAD_init();
	
	/*Initializing other devices*/
	Device_init();
	
	/*Initializing LCD device*/
	lcd_init(LCD_DISP_ON);
	
	/*Initializing UART device*/
	uart_init(UART_BAUD_SELECT(9600,F_CPU)); //9600 speed, F_CPU
	
	/*Initializing Timer/Counter2 and interrupt*/
	TIM2_overflow_1ms();
	TIM2_overflow_interrupt_enable();	

	/*Initializing Timer/Counter1*/
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();

	/*Initializing Timer/Counter0*/
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();

	/*Global interrupts enable*/
	sei();

	
	lcd_gotoxy(1,0); //Start text
	lcd_puts("Password: ");
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

ISR(TIMER2_OVF_vect) //Timer2 interrupt to scanning keypad and limit and uart
{
	key=SCAN_1COL(); //my functions on scanning
	key2=SCAN_2COL();
	key3=SCAN_3COL();
	
	if (key!=11) //key 11 indicates no press
	{
		itoa(key,lcd_string,10); //convert int to string
	}
	else if (key2!=11)
	{
		itoa(key2,lcd_string,10);
	}
	else if (key3!=11)
	{
		itoa(key3,lcd_string,10);
	}
	else //no press and nothing write on display
	{
		lcd_string[0]='1';
		lcd_string[1]='1';
	}
}

ISR(TIMER1_OVF_vect) //Timer1 interrupt have limit 4 seconds to lock door
{
	static uint8_t time=60; //variable for limit 60s
	number_of_overflows2++;

		if ((number_of_overflows2 >= 6) && (number_of_password>=4))
		{
			// Do this 6x 1 s = 6s after correct password
			
			GPIO_write_low(&PORTB,DOOR); //Lock door
			GPIO_write_high(&PORTC,LOCK); //Turn on RED LED
			GPIO_write_low(&PORTC,OPEN); //Turn off GREEN LED
			number_of_overflows2 = 0; 
			number_of_password = 0; //4 numbers user entered
			
			lcd_gotoxy(0,1); //clear display
			lcd_puts("                ");
			lcd_gotoxy(11,0);
			lcd_puts("    ");
			time=60; //start password limit
		}
	
	/*60 second password limit*/
	if (number_of_password!=4)
	{
		time--; // - second each time the timer overflows
		if (time ==0)
		{
			for (i=0;i<=100;i++) //100x 10ms = 1s
			{
				GPIO_toggle(&PORTB,BUZZER); //toggle output of buzzer
				_delay_ms(10); //10ms delay
			}
			GPIO_write_low(&PORTB,BUZZER); //close the transistor
			
			lcd_gotoxy(1,1);
			lcd_puts("  ");
			uart_puts("Vypršel ti limit.\r\n"); //Information to uart
			time=60;		
		}
	
		itoa(time,lcd_string,10); //convert time int to string
		if (time>9)	//60-10 seconds
		{
			lcd_gotoxy(1,1);
			lcd_puts(lcd_string);
		}
		else //9-0 seconds
		{
			lcd_gotoxy(1,1);
			lcd_putc(' ');
			lcd_gotoxy(2,1);
			lcd_puts(lcd_string);
		}
	}

}


ISR(TIMER0_OVF_vect) //Timer0 interrupt to writing all to LCD and UART with conditions to correct password
{
	static uint8_t number_of_overflows=0; //overflow to do all
	static uint8_t number_of_overflows3=0; //overflow to send uart

	
	number_of_overflows++;
	number_of_overflows3++;
	
	sum_key=atoi(lcd_string); //Get key from all columns
	
	if (number_of_overflows3 >= 12) //Sending info of keys to uart
	{
		itoa(sum_key,lcd_string2,10); //Convert key to string
		if (sum_key!=11)
		{
			uart_puts("Key: ");
			uart_puts(lcd_string2);
			uart_puts("\r\n");
			number_of_overflows3=0;
		}
		else
		{
			uart_puts("No press\r\n");
			number_of_overflows3=0;
		}
	}
	
	
	
	if (number_of_overflows >= 6) //Write all to LCD every 100 ms
	{
		// Do this every 6 x 16 ms = 100 ms
		number_of_overflows = 0;
 
 		if (sum_key!=11) //Some key press
 		{	 
			 
			for (i=0;i<=30;i++) //enough time for sound
			{
				GPIO_toggle(&PORTB,BUZZER); //toggle output of buzzer 
				_delay_ms(1); //1ms delay
			}
			GPIO_write_low(&PORTB,BUZZER); //close the transistor
			
			number_of_password++; //user entered the number
		
 			//First number of Lock Code
 			if (number_of_password==1)
 			{
				lcd_gotoxy(11,1); //position on display
				lcd_putc(0xFF); //Rectangle for user
				Newpassword[0]=sum_key; //Save press key
				
				itoa(sum_key,lcd_string,10); //int to string
				lcd_gotoxy(11,0);
				lcd_puts(lcd_string); //Press key to LCD
 			
 			}
 			//Second number of Lock Code
 			else if (number_of_password==2)
 			{
				lcd_gotoxy(12,1);
				lcd_putc(0xFF);
 				Newpassword[1]=sum_key;
			 
 				itoa(sum_key,lcd_string,10);
 				lcd_gotoxy(12,0);
 				lcd_puts(lcd_string);
 			
 			}
 			//Third number of Lock Code
 			else if (number_of_password==3)
 			{
				lcd_gotoxy(13,1);
				lcd_putc(0xFF);
 				Newpassword[2]=sum_key;
				
 				itoa(sum_key,lcd_string,10);
 				lcd_gotoxy(13,0);
 				lcd_puts(lcd_string);
 			
 			}
 			//Four number of Lock Code
 			else if (number_of_password==4)
 			{
				lcd_gotoxy(14,1);
				lcd_putc(0xFF);
 				Newpassword[3]=sum_key; 
			 
				itoa(sum_key,lcd_string,10);
				lcd_gotoxy(14,0);
				lcd_puts(lcd_string);
			
				/*Condition for finding the correct password*/
				if (((Newpassword[0]==Password1[0]) && (Newpassword[1]==Password1[1]) && (Newpassword[2]==Password1[2]) && (Newpassword[3]==Password1[3])) || ((Newpassword[0]==Password2[0]) && (Newpassword[1]==Password2[1]) && (Newpassword[2]==Password2[2]) && (Newpassword[3]==Password2[3])) || ((Newpassword[0]==Password3[0]) && (Newpassword[1]==Password3[1]) && (Newpassword[2]==Password3[2]) && (Newpassword[3]==Password3[3])))
				{
					GPIO_write_high(&PORTB,DOOR); //Harry, I'm going to the house!
					GPIO_write_low(&PORTC,LOCK); //Turn off RED LED
					GPIO_write_high(&PORTC,OPEN); //Turn on GREEN LED
			
					for (i=0;i<=230;i++) //enough time for sound
					{
						GPIO_toggle(&PORTB,BUZZER); //toggle output of buzzer
						_delay_us(800); //800us delay
						GPIO_write_low(&PORTB,BUZZER); //close the transistor
						GPIO_toggle(&PORTB,BUZZER); //toggle output of buzzer
						_delay_us(150); //150us delay
					}
					GPIO_write_low(&PORTB,BUZZER); //close the transistor
			
					lcd_gotoxy(0,1);
					lcd_puts("Pristup povolen!");
					number_of_overflows2=0; //Start overflow of correct password
					TCNT1=0; //Reset timer1 register
					uart_puts("Odemknuto!\r\n"); //Information to uart
				}
				else //User entered wrong password
				{
					lcd_gotoxy(0,1);
					lcd_puts("Pristup odepren.");
					number_of_overflows2=0; 
					number_of_overflows2=5; //Start overflow of wrong password
					TCNT1=0; //Reset timer1 register
					uart_puts("Spatne heslo.\r\n"); //Information to uart
				}
				
 			}
			 
		}
		 
	}
	 
}

