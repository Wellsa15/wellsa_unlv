/*
 * DA4T1.c
 *
 * Created: 4/5/2016 7:02:42 PM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // Set clock speed to 8MHz

#include <avr/io.h>
#include <util/delay.h>

void DUTY_CYCLE(); // Function to change duty cycle
void TIMER0_INIT(); // Function to Initialize timer 0
void TIMER1_INIT(); // Function to Initialize timer 1
void TIMER2_INIT(); // Function to Initialize timer 2


int main(void)
{
	DDRD = (1<<6);
	DDRB = (1<<1)|(1<<3);
	TIMER0_INIT(); // Initialize timer 0 
	TIMER1_INIT(); // Initialize timer 1
	TIMER2_INIT(); // Initialize timer 2
 
	TCCR1B = (1<<WGM12) | // Prescaler to 1
			 (1<<CS10);	// Timer Starts
	TCCR0B = (1<<CS00); //Prescaler to 1, Timer starts
	TCCR2B = (1<<CS20); // Prescaler to 1, Timer starts
   
    while (1) 
    {
		while (TCCR1B != 0x08)
		{
			while (TCCR0B != 0x00)
			{
				while (TCCR2B != 0x00)
				{
					DUTY_CYCLE(); // Function to cycle through duty cycles
					if (TCCR2B < 0x07)
						TCCR2B++; // increase prescaler value
					else
						TCCR2B = 0x00; // Set prescaler to 0 to exit loop
				}
				TCCR2B = 0x01; // Reset Timer 2 prescaler to 1
				if (TCCR0B < 0x05)
					TCCR0B++; // increase prescaler value
				else
					TCCR0B = 0x00; // Set prescaler to 0 to exit loop
			}
			TCCR0B = 0x01; // Reset Timer 0 prescaler to 1
			if (TCCR0B < 0x0D)
				TCCR0B++; // Increase prescaler value
			else
				TCCR0B = 0x08;	// Reset timer 1 to exit loop					
		}
	TCCR1B = 0x09; // Reset Timer 1 prescaler to 1
    }
return 0;
}

void DUTY_CYCLE()
{
	OCR1A = 25; // Initialize Compare value to 10% of 255
	OCR0A = 25; // Initialize Compare value to 10% of 255
	OCR2A = 25; // Initialize Compare value to 10% of 255
	while(OCR1A < 249)
	{
		while(OCR0A < 249)
		{
			while(OCR2A < 249)
			{
				_delay_ms(150);
				OCR2A = OCR2A + 25; // Increase compare value for timer 2 by 25 or 10%
			}
			OCR2A = 25; // Reset Compare value for timer 2 to 25 or 10%
			OCR0A = OCR0A +25; // Increase compare value for timer 0 by 25 or 10%
		}
		OCR0A = 25; // Reset Compare value for timer 0 to 25 or 10%
		OCR1A = OCR1A + 25; // Increase compare value for timer 1 by 25 or 10%
	}
	
return;
}

void TIMER0_INIT()
{
	TCCR0A = (1<<COM0A1) | // Non Inverting Mode
			 (0<<COM0A0) | // Clear OC0A on Compare match, set OC0A at Bottom
			 (0<<COM0B1) | // Normal port operation
			 (0<<COM0B0) | // OC0B disconnected
			 (1<<WGM01)	 | // Fast PWM mode
			 (1<<WGM00);
return;
}

void TIMER2_INIT()
{
	TCCR2A = (1<<COM2A1) | // Non Inverting Mode
			 (0<<COM2A0) | // Clear OC2A on Compare match, set OC2A at Bottom
			 (0<<COM2B1) | // Normal port operation
			 (0<<COM2B0) | // OC2B disconnected
			 (1<<WGM21)	 | // Fast PWM mode
			 (1<<WGM20);
return;
}

void TIMER1_INIT()
{
	TCCR1A = (1<<COM1A1) | // Non Inverting Mode
			 (0<<COM1A0) | // Clear OC2A on Compare match, set OC2A at Bottom
			 (0<<COM1B1) | // Normal port operation
			 (0<<COM1B0) | // OC2B disconnected
			 (0<<WGM11)	 | // Fast PWM mode
			 (1<<WGM10);
return;
}