/*
 * DA2T2.c
 *
 * Created: 3/12/2016 7:18:08 PM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // XTAL= 8MHZ 	
#include <avr/io.h>

void DELAY();// Delay function

unsigned char a = 1;// Set a to 1 for XOR with PORTC.0
unsigned char num = 0;// Set num to 0, counting Variable
unsigned char byfive = 0;// Set byfive to 0, counting Variable

int main(void)
{
	DDRB = 0xFF;// Activate all PORTB Pins
	DDRC = 0x31;// Activate PORTC Pins 0, 4, 5
	 
    while (1) //Infinite loop
    {		
		DELAY();// Run Delay function
		if ((PORTC % 2) == 1) // 
		{
			num++;// Increase num by 1
			byfive++;// Increase byfive by 1
			PORTB = num;// Copy the value in num into PORTB
			
			if (byfive == 5)// Compare byfive with 5
			{
				PORTC |= 1 << PORTC4;// Set PORTC.4 to 1
				PORTC &= 0 << PORTC5;// Set PORTC.5 to 0			
			}else			
			if (byfive == 10)// Compare byfive with 10
			{
				PORTC &= 0 << PORTC4;// Set PORTC.4 to 0
				PORTC |= 1 << PORTC5;// Set PORTC.5 to 1
			}else
			if (byfive == 15)// Compare byfive with 15
			{
				PORTC |= 1 << PORTC4;// Set PORT.4 to 1
				PORTC |= 1 << PORTC5;// Set PORT.5 to 1				
			}else
			if (byfive == 20)// Compare byfive with 20
			{
				byfive = 0;// Reset byfive to 0
				PORTC &= 0 << PORTC4;// Set PORT.4 to 0
				PORTC &= 0 << PORTC5;// Set PORT.5 to 0
			}
	}
    }
return 0;
}

void DELAY()//  Function to cause delay
{
	TCNT1 = 0xF0BE;// When using chip on breadboard: TCNT1 to 0xF862, 63586
				// When using Xplained Mini board: TCNT1 = 0xF0BE, 61630	
	TCCR1A = 0x0;//Normal Mode
	TCCR1B = 0x05;//1024 Prescaler
	
	while ((TIFR1 & (1<<TOV1)) == 0)// Stay in loop while TOV1 = 0
	{		
	}
	
	TIFR1 |= 1 << 0;// Clear TOV1
	PORTC ^= a << PORTC0;// XOR PORTC.0 with a to toggle on and off
	
return;	// Return to main program
}