/*
 * DA5T2.c
 *
 * Created: 4/17/2016 12:05:59 AM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // Set clock to 8Mhz, Ignored by Xplained Mini Board

#include <avr/io.h>
#include <util/delay.h>

void ADCstart(); // Function to initialize the ADC
void DUTY_DELAY(int DELAY); // function to allow variable delay

volatile float ADCvalue; // Variable to save the voltage value

int main()
{
	int OFF_Time; // variable for cycle Off time
		
	// Black = PD0
	// Red = PD1
	// Green = PD2
	// Blue = PD3
	DDRD = 0xFF; // set PORTD as output
	
	ADCstart(); // initialize the ADC port

	while(1)
	{
		ADCvalue = 0;
		
		ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
		while(ADCSRA & (1<<ADSC))// wait until conversion is complete
		{
		}
		ADCvalue = ADC;// save voltage
		ADCvalue = (ADCvalue*5.0)/1024;
		ADCvalue = ((ADCvalue/5.0)*100); // Calculate what percent of maximum voltage is recieved
		OFF_Time = 100 - ADCvalue; // Calculate off time
		
		PORTD = 0X66;// Cycle for magnets
		DUTY_DELAY(OFF_Time+1);// how long to wait to change magnets
		PORTD = 0xCC;// Cycle for magnets
		DUTY_DELAY(OFF_Time+1);// how long to wait to change magnets
		PORTD = 0x99;// Cycle for magnets
		DUTY_DELAY(OFF_Time+1);// how long to wait to change magnets
		PORTD = 0x33;// Cycle for magnets
		DUTY_DELAY(OFF_Time+1);// how long ot wait to change magnets
	}
return 0;
}

void DUTY_DELAY(int DELAY)
{// Function to allow variable delay
	while(DELAY--)
	{
		_delay_ms(1);
	}
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