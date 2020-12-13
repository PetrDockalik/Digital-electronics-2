/*
 * scan_keys.h
 *
 * Created: 29.11.2020 22:23:21
 *  Author: Petr Doèkalík, xdocka13, 211544
 */ 

#ifndef SCAN_KEYS_H_
#define SCAN_KEYS_H_

/* Includes ----------------------------------------------------------*/
#include <avr/io.h> //using ports and pins

/* Defines -----------------------------------------------------------*/
#define COLUMN1 PC3 //inputs and outputs define
#define COLUMN2 PC4 
#define COLUMN3 PC5

#define ROW1 PD0
#define ROW2 PD1
#define ROW3 PD2
#define ROW4 PD3

#define BUZZER PB4
#define DOOR PB5

#define LOCK PC1
#define OPEN PC0

/* Function prototypes -----------------------------------------------*/
void KEYPAD_init(void);

void Device_init(void);

uint8_t SCAN_1COL(void);

uint8_t SCAN_2COL(void);

uint8_t SCAN_3COL(void);

#endif 