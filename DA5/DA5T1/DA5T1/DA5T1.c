/*
 * DA5T1.c
 *
 * Created: 4/17/2016 12:03:42 AM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // Set clock to 8Mhz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define ENABLE 6

void ADCstart(); // Function to initialize the ADC
void read_adc(); // Function to read from the ADC
void DUTY_DELAY(int DELAY);// Function to allow variable delay

volatile float ADCvalue; // Variable to save the voltage value

int main(void)
{
	int On_Time;// variable for the On part of the duty cycle
	int OFF_Time;// variable for the Off part of the duty cycle
	
	DDRD |= (1<<6); // Turn on PORTD.6
	PORTD = (1<<ENABLE); // Power signal to the DC motor
	
	ADCstart(); // initialize the ADC port
	
	while (1)
	{
		ADCvalue = 0; // reset potentiometer value

		ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
		while(ADCSRA & (1<<ADSC))// wait until conversion is complete
		{
		}
		ADCvalue = ADC;// save voltage
		ADCvalue = (ADCvalue*5.0)/1024; // Calculate Vin
		ADCvalue = ((ADCvalue/5.0)*100); // Calculate what percent of maximum voltage is received
		OFF_Time = 100 - (int)ADCvalue; // Calculate Off part of duty cycle
		On_Time = 100 - OFF_Time; // Calculate On part of duty cycle
		
		PORTD = PORTD | (1<<ENABLE); // Motor off
		DUTY_DELAY(OFF_Time-1);// How long the motor is off
		PORTD = PORTD & (~(1<<ENABLE)); // Motor on
		DUTY_DELAY(On_Time-1);// How long the motor is on
		
	}
return 0;
}

void DUTY_DELAY(int DELAY)
{// function to allow variable delays
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

void read_adc() // function to read the value from the potentiometer
{
	ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
	while(ADCSRA & (1<<ADSC))// wait until conversion is complete
	{
	}
	ADCvalue = ADC;// save voltage
	
	return;
}