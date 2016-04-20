/*
 * DA6T1.c
 *
 * Created: 4/19/2016 10:01:31 PM
 * Author : sirfe
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "nokia5110.h"

void ADCstart(); // Function to initialize the ADC

volatile unsigned int ADCvalue; // Variable to save the temperature value
volatile unsigned int X; // variable to keep track of how many values have been read
float Ratio; // variable for calculating the temperature
char outs[8]; // output buffer


int main(void)
{
	int Y = 0; // Change Y
	Ratio = 500.0/1024.0; // Ratio to calculate the actual temperature
	ADCstart(); // initialize the ADC port
	nokia_lcd_init();// Initiaize LCD
while(1)
{
	X = 4; // number of values taken before transmission
	ADCvalue = 0; // reset temperature value
	while(X) // stay here until 4 reading are taken
	{
		ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
		while(ADCSRA & (1<<ADSC))// wait until conversion is complete
		{
		}
		ADCvalue += (Ratio*ADC);// save temperature
		X--;
		_delay_ms(250); //Delay to give time between readings
	}
	ADCvalue = ADCvalue/4;// average of values received
	snprintf(outs, sizeof(outs),"%3d C", ADCvalue); // convert value to a string
		

	nokia_lcd_set_cursor(0, Y);
	nokia_lcd_write_string(outs,1); // Send Temperature to LCD
	nokia_lcd_render();// Display Temperature on LCD
	if (Y < 25)
		Y+=8;
	else
		{
		Y = 0;
		nokia_lcd_clear(); // Clear screen
		}
	for (int z = 0; z< 1000;z++)
		_delay_ms(4);
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