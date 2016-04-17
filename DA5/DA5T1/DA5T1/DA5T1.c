/*
 * DA5T1.c
 *
 * Created: 4/17/2016 12:03:42 AM
 * Author : sirfe
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#define ENABLE 6

int main(void)
{
	int num;
	DDRD |= (1<<6);
	PINC = 0;
	PORTD = (1<<ENABLE);
	while (1)
	{
		
		if (PINC==0x04)
		{
			PORTD = PORTD | (1<<ENABLE);
			_delay_ms(50);
			PORTD = PORTD & (~(1<<ENABLE));
			_delay_ms(50);
		}
		else if (PINC == 0x02)
		{
			PORTD = PORTD | (1<<ENABLE);
			_delay_ms(25);
			PORTD = PORTD & (~(1<<ENABLE));
			_delay_ms(75);
		}
		else if (PINC == 0x06)
		{
			PORTD = PORTD & (~(1<<ENABLE));
			_delay_ms(1000);
		}else
		{
			PORTD = (1<<ENABLE);
		}
	}
	
	return 0;
}

