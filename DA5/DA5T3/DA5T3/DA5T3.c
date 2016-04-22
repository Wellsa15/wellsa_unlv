/*
 * DA5T3.c
 *
 * Created: 4/20/2016 9:40:20 AM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // Set clock to 8Mhz, ignored by Xplained Mini board

#include <avr/io.h>
#include <util/delay.h>

void ADCstart(); // Function to initialize the ADC
void TIMER1_INIT(); // Function to initialize Timer 1

volatile float ADCvalue; // Variable to save the voltage value

int main()
{
	int ANGLE;
	ADCstart(); // initialize the ADC port
	DDRB = (1<< PORTB1); // Set PORTB1 pin as output

	ICR1 = 40000;// 50Hz PWN at 16Mhz, TOP = ICR1
	OCR1A = 1000;// 1000 = 0 degrees, 4600 = 180 degrees
	TIMER1_INIT();
	//Brown = Ground
	// Red = Power
	// Orange = Signal
	while(1)
	{
		ADCvalue = 0;
		
		ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
		while(ADCSRA & (1<<ADSC))// wait until conversion is complete
		{
		}
		ADCvalue = ADC;// save voltage
		ADCvalue = (ADCvalue*5.0)/1024;// Calculate Vin
		ADCvalue = ((ADCvalue/5.0)*3600); // Calculate how far the angle is above 0 degrees
		ANGLE = ADCvalue + 1000; // Add value for 0 degree 
		OCR1A = ANGLE; // Save value for PWN duty cycle
		
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

void TIMER1_INIT()
{
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM ....1000 bottom
	// Prescaler = 8
	TCCR1A |= (0<<COM1A0)| // TCCR1A = 0x82
			(1<<COM1A1)|
			(0<<COM1B1)|
			(0<<FOC1A)|
			(0<<FOC1B)|
			(1<<WGM11)|
			(0<<WGM10);
			
	TCCR1B |= (0<<ICNC1)|// TCCR1B = 0x1A
			(0<<ICES1)|
			(1<<WGM13)|
			(1<<WGM12)|
			(0<<CS12)|
			(1<<CS11)|
			(0<<CS10);
}