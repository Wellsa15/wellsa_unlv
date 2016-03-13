/*
 * DA2T2.c
 *
 * Created: 3/12/2016 7:18:08 PM
 * Author : sirfe
 */ 

#include <avr/io.h>


int DELAY(int);


int main(void)
{
	DDRB = 0xFF;
	DDRC = 0x31;
	int b = 0x00;
	int num = 0x0;
	int byfive = 0x0;
	//unsigned char k;
 
    while (1) 
    {		
		b = DELAY(b);
		if (b == 1)
		{
			num++;
			byfive++;
			PORTB = num;
			
			if (byfive == 5)
			{
				//k = 0x20;
				//k = k | b;	
				PORTC4 << 1;
				PORTC5 << 0;			
			}			
			if (byfive == 10)
			{
				//k = 0x10;
				//k = k | b;
				PORTC4 << 0;
				PORTC5 << 1;
			}
			if (byfive == 15)
			{
				//k = 0x30;
				//k = k | b;
				PORTC4 << 1;
				PORTC5 << 1;				
			}
			if (byfive == 20)
			{
				byfive = 0;
				//k = 0x0;
				//k = k| b;
				PORTC4 << 0;
				PORTC5 << 0;
			}
	}
		
		
    }
return 0;
}

int DELAY(int b)
{
	
	int a = 0x01;
	TCNT1 = 0xF862;
	TCCR1A = 0x0;
	TCCR1B = 0x05;
	
	while ((TIFR1 & (1<<TOV1)) == 0)
	{		
	}
	
	b = a ^ b;
	TIFR1 |= 1 << 0;
	if (b == 0)
	{
		
		PORTC0 << 0;
	}else
	{
		PORTC1 << 1;
	}
return b;	
}