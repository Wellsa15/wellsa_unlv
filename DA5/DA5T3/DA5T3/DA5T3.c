/*
 * DA5T3.c
 *
 * Created: 4/17/2016 12:07:05 AM
 * Author : sirfe
 */ 

 #define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
int main()
{
	int x;
	//PORTD pins as input
	DDRD = 0x00;
	//Enable internal pull ups
	PORTD = 0xFF;
	//Set PORTB1 pin as output
	DDRB = 0xFF;
	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1 = 20000 defines 50Hz pwm
	ICR1 = 20000;
	OCR1A = 500;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10); //TCCR1A = 0x82
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10); //TCCR1B = 0x1A
	//start timer with prescaler 8
	//for(;;)
	//{  // Discover Lower and Upper limits
	/*	if(PIND == 0)
		{
			//increase duty cycle
			OCR1A+=10;
			while(PIND == 0)
		}
		if(PIND == 1)
		{
			//decease duty cycle
			OCR1A-=10;
			while(PIND == 1);
		}*/
	//}
	while (1)
	{
		OCR1A= 500;
		for(x = 0; x<500;x++)
			_delay_ms(5);
		OCR1A = 1400;
		for(x = 0; x<500;x++)
		_delay_ms(15);
		OCR1A = 850;
		for(x = 0; x<500;x++)
		_delay_ms(5);
		OCR1A = 1750;
		for(x = 0; x<500;x++)
		_delay_ms(15);
		OCR1A = 2300;
		for(x = 0; x<500;x++)
		_delay_ms(5);
		OCR1A = 500;
		for(x = 0; x<500;x++)
		_delay_ms(10);
	}

	return 0;
}
