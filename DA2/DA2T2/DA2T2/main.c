/*
 * DA2T2.c
 *
 * Created: 3/12/2016 7:18:08 PM
 * Author : sirfe
 */ 

#include <avr/io.h>



void DELAY();

unsigned char a = 1;
//unsigned char b = 0;
unsigned char num = 0;
unsigned char byfive = 0;

int main(void)
{
	DDRB = 0xFF;
	DDRC = 0x31;
	//unsigned char num = 0;
	//int byfive = 0;
	 
    while (1) 
    {		
		DELAY();
		if ((PORTC % 2) == 1)
		{
			num++;
			byfive++;
			PORTB = num;
			
			if (byfive == 5)
			{
				PORTC |= 1 << PORTC4;
				PORTC &= 0 << PORTC5;			
			}			
			if (byfive == 10)
			{
				PORTC &= 0 << PORTC4;
				PORTC |= 1 << PORTC5;
			}
			if (byfive == 15)
			{
				PORTC |= 1 << PORTC4;
				PORTC |= 1 << PORTC5;				
			}
			if (byfive == 20)
			{
				byfive = 0;
				PORTC &= 0 << PORTC4;
				PORTC &= 0 << PORTC5;
			}
	}
    }
return 0;
}

void DELAY()
{
	TCNT1 = 0xF862;
	TCCR1A = 0x0;
	TCCR1B = 0x05;
	
	while ((TIFR1 & (1<<TOV1)) == 0)
	{		
	}
	
	//b = a ^ b;
	
	TIFR1 |= 1 << 0;
	PORTC ^= a << PORTC0;
	
return;	
}