/*
 * DA5T3.c
 *
 * Created: 4/17/2016 12:07:05 AM
 * Author : sirfe
 */ 

 #define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void ADCstart(); // Function to initialize the ADC

volatile float ADCvalue; // Variable to save the voltage value
volatile int ANGLE; // Variable for what angle to move to

int main()
{
	//PORTD pins as input
	DDRD = 0x00;
	//Enable internal pull ups
	PORTD = 0xFF;
	//Set PORTB1 pin as output
	DDRB = 0xFF;
	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM ....1000 bottom
	//ICR1 = 20000 defines 50Hz pwm
	ICR1 = 40000;
	OCR1A = 1000;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10); //TCCR1A = 0x82
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10); //TCCR1B = 0x1A
	//start timer with prescaler 8
	/*while(1)
	{
		OCR1A += 50;
		_delay_ms(1000);
	}*/
	//Brown = Ground
	// Red = Power
	// Orange = Signal
	while (1)
	{
		ADCvalue = 0;

		ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
		while(ADCSRA & (1<<ADSC))// wait until conversion is complete
		{
		}
		ADCvalue = ADC;// save voltage
		//ADCvalue = ADCvalue/1024; // Calculate what percent of maximum voltage is received
		ADCvalue = ADCvalue * 3.5156;
		ANGLE = ADCvalue+1000;
		OCR1A = ANGLE;
	}

return 0;
}

void ADCstart()
{
	ADMUX = (0<<REFS1)| // Reference Selection Bits
	(1<<REFS0)| //use AVcc as reference
	(0<<ADLAR)|	//ADC Left adjust Result
	(0<<MUX2)|	// Analog Channel Selection Bits
	(1<<MUX1)|	// ADC2 (PC2, PIN25)
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|	//ADC Enable
	(0<<ADSC)|	//ADC Start Conversion
	(0<<ADIF)|	//ADC Auto Trigger Enable
	(0<<ADIE)|	//ADC Interrupt Flag
	(0<<ADATE)|	//ADC Interrupt Enable
	(1<<ADPS2)|	//ADC Prescaler Bits
	(0<<ADPS1)|
	(1<<ADPS0);

return;
}