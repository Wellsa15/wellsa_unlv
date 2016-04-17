/*
 * DA5T2.c
 *
 * Created: 4/17/2016 12:05:59 AM
 * Author : sirfe
 */ 

#define F_CPU 1000000UL //XTAL = 8MHZ
#include <avr/io.h>
#include <util/delay.h>
int main()
{
	// Black = PB0
	// Red = PB1
	// Green = PB2
	// Blue = PB3
	DDRC = 0x00;
	DDRB = 0xFF;
	while(1)
	{
		if (PINC == 0x08)
		{
			PORTB = 0X66;
			_delay_ms(5);
			PORTB = 0xCC;
			_delay_ms(5);
			PORTB = 0x99;
			_delay_ms(5);
			PORTB = 0x33;
			_delay_ms(5);
		}
		else
		{
			PORTB = 0X66;
			_delay_ms(30);
			PORTB = 0x33;
			_delay_ms(30);
			PORTB = 0x99;
			_delay_ms(30);
			PORTB = 0xCC;
			_delay_ms(30);
		}
	}// opposite direction
	return 0;
}

