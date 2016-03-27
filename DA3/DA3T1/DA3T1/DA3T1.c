/*
 * DA3T1.c
 *
 * Created: 3/27/2016 2:26:49 PM
 * Author : sirfe
 */ 

#define F_CPU 1000000UL
#define UBRR_2400 25	//For 1Mhz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void ADCstart();
void read_adc();
void USARTstart(unsigned int ubrr);
void USART_tx_string(char *data);

volatile char ReceivedChar;
volatile unsigned int ADCvalue;
char outs[20];

int main(void)
{
	
	ADCstart();
	USARTstart(UBRR_2400);
	sei ();
    while (1) 
    {
		read_adc();
		
		snprintf(outs, sizeof(outs),"%3d\r\n", ADCvalue); // print
		USART_tx_string(outs);
		
	_delay_ms(125);  //wait 125ms
    }
	
return 0;
}

void read_adc()
{
	unsigned char i=4;
	ADCvalue = 0;
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC))
		{			
		}
		ADCvalue += ADC;
		_delay_ms(50);
	}
	ADCvalue = ADCvalue;
	
return;
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
	
	ADCSRB = 0; // 0 for free running mode
		
	TIMSK1 |= (1<<TOIE1);  //enable overflow interrupt
	
	TCCR1B |=(1<<CS11)|	// Native clock
			 (1<<CS10);
return;
}

void USARTstart(unsigned int ubrr)
{
	    UBRR0H = (unsigned char)(ubrr>>8);
	    UBRR0L = (unsigned char)ubrr;
	    
	    UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // Enable Transmit and Receive
	    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00); // Set Frame: 8bit, 1 Stop
		
return;
}

void USART_tx_string(char *data)
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1<<UDRE0)))
		{			
		}
		UDR0 = *data;
		data++;
	}
}