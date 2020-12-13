/*
 * scan_keys.c
 *
 * Created: 29.11.2020 22:23:32
 *  Author: Petr Doèkalík, xdocka13, 211544
 */ 

/* Includes ----------------------------------------------------------*/
#include "scan_keys.h"
#include "gpio.h"

/* Function definitions ----------------------------------------------*/
/*Initializing Keypad device*/
void KEYPAD_init(void)
{
	GPIO_config_input_nopull(&DDRD,ROW1);
	GPIO_config_input_nopull(&DDRD,ROW2);
	GPIO_config_input_nopull(&DDRD,ROW3);
	GPIO_config_input_nopull(&DDRD,ROW4);
	GPIO_config_output(&DDRC,COLUMN1);
	GPIO_config_output(&DDRC,COLUMN2);
	GPIO_config_output(&DDRC,COLUMN3);
	
	GPIO_write_low(&PORTC,COLUMN1);
	GPIO_write_low(&PORTC,COLUMN2);
	GPIO_write_low(&PORTC,COLUMN3);
}

/*--------------------------------------------------------------------*/
/*Initializing other devices*/
void Device_init(void)
{
	GPIO_config_output(&DDRC,LOCK);
	GPIO_config_output(&DDRC,OPEN);
	GPIO_config_output(&DDRB,BUZZER);
	GPIO_config_output(&DDRB,DOOR);
	
	GPIO_write_low(&PORTC,OPEN);
	GPIO_write_high(&PORTC,LOCK);
	
	GPIO_write_low(&PORTB,BUZZER);
}

/*--------------------------------------------------------------------*/
/*Testing First Column*/
uint8_t SCAN_1COL(void) //scanning first column
{	
	static uint8_t result; //variable for send press key to main.c
 	GPIO_write_high(&PORTC,COLUMN1); //high column to scan
 	
 	if (GPIO_read(&PIND,ROW1)) //find press key
 	{
 		result=1;
 	}
 	else if (GPIO_read(&PIND,ROW2))
 	{
 		result=4;
 	}
 	else if (GPIO_read(&PIND,ROW3))
 	{
 		result=7;
 	}
 	else if (GPIO_read(&PIND,ROW4))
 	{
 		result=11;
 	}
 	else
 	{
 		result=11; //no press
 	}
	 
	 
 	GPIO_write_low(&PORTC,COLUMN1); //low column to end scan
	return result; //send to main.c
}

 
/*Testing Second Column*/	
uint8_t SCAN_2COL(void)
{
	static uint8_t result;
	GPIO_write_high(&PORTC,COLUMN2);
	
	
  	if (GPIO_read(&PIND,ROW1))
  	{
  		result=2;
  	}
  	else if (GPIO_read(&PIND,ROW2))
  	{
  		result=5;
  	}
  	else if (GPIO_read(&PIND,ROW3))
  	{
  		result=8;
  	}
  	else if (GPIO_read(&PIND,ROW4))
  	{
  		result=0;
  	}
  	else
  	{
  		result=11;
  	}	
 	
 	GPIO_write_low(&PORTC,COLUMN2);
	return result;
}
	 
	 
/*Testing Third Column*/
uint8_t SCAN_3COL(void)
{
	static uint8_t result;
	GPIO_write_high(&PORTC,COLUMN3);
 	
 	
  	if (GPIO_read(&PIND,ROW1))
  	{
  		result=3;
  	}
  	else if (GPIO_read(&PIND,ROW2))
  	{
  		result=6;
  	}
  	else if (GPIO_read(&PIND,ROW3))
  	{
  		result=9;
  	}
  	else if (GPIO_read(&PIND,ROW4))
  	{
  		result=11;
  	}
  	else
  	{
  		result=11;
  	}
 	
 	GPIO_write_low(&PORTC,COLUMN3);
	return result;
}