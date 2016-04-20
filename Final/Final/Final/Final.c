/*
 * Final.c
 *
 * Created: 4/18/2016 9:35:35 PM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // Set clock to 8MHz
#define UBRR_9600 51	//For 8Mhz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void ADCstart(); // Function to initialize the ADC
void read_adc(); // Function to read from the ADC
void USARTstart(unsigned int ubrr); // Function to initialize the USART port
void USART_tx_string(char *data); // Function to transmit the value over the serial port
void TIMER1_init(); // Function to initialize timer 1
void i2c_init();// initialize sensor
void i2c_start();// tell sensor to start sending data
void i2c_write(unsigned char data); //write data to sensor
void i2c_read(unsigned char	isLast);// read data from sensor
void i2c_stop(); // stop sensor


volatile float ADCvalue; // Variable to save the sensor value
volatile unsigned int X; // variable to keep track of how many values have been read
char outs[20]; // output buffer


int main(void)
{
	unsigned char i = 0;
	
	i2c_init(); 
	ADCstart(); // initialize the ADC port
	USARTstart(UBRR_9600); // initialize the USART port
	TIMER1_init(); // Initialize timer 1

	sei (); // enable global interrupts
	while (1) // infinite loop to transmit the sensor value over serial port
	{
		i2c_start();
		X = 4; // number of values taken before transmission
		ADCvalue = 0; // reset temperature value
		while(X) // stay here until 4 reading are taken
		{
		}
		ADCvalue = ((ADCvalue*5)/1024);// 
		
		snprintf(outs, sizeof(outs),"%3f\r\n", ADCvalue); // convert value to a string
		USART_tx_string(outs); // transmit value over serial connection
	}
	
	return 0;
}

void read_adc() // function to read the value from the sensor
{
	ADCSRA |= (1<<ADSC);// write one to ADSC bit to start conversion
	while(ADCSRA & (1<<ADSC))// wait until conversion is complete
	{
	}
	ADCvalue = ADC;// save temperature
	
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

	return;
}

void TIMER1_init()
{
	OCR1A = 0x7A12;
	TIMSK1 |= (1<<OCIE1A);  //enable Compare A interrupt
	
	TCCR1B |=(1<<WGM12)|// CTC Mode
	(1<<CS11)|	// Prescaler = 64
	(1<<CS10);
	
}

void USARTstart(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);// set baud rate to 9600
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // Enable Transmit and Receive
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00); // Set Frame: 8bit, 1 Stop
	
	return;
}

void USART_tx_string(char *data)// transmit value function
{
	while ((*data != '\0'))// loop to send each character over the serial connection
	{
		while (!(UCSR0A & (1<<UDRE0)))// wait until transmit buffer is clear
		{
		}
		UDR0 = *data; // load character into transmit buffer
		data++; // go to next character
	}
}

ISR (TIMER1_COMPA_vect)// Timer 1 compare interrupt
{
	read_adc();// read value from sensor
	X--; // decrease X
}

void i2c_init()
{
	TWSR= 0x00; // set prescaler bits to zero
	TWBR= 0x47;	//SCL Freq is 50k for XTAL = 8M
	TWCR= 0x04; // Enable the TWI module	
}

void i2c_start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR &(<<TWINT)) == 0);
}

void i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR&(1<<TWINT)) ==0);
}

void i2c_read(unsigned char	isLast)
{
	if (isLast == 0) // if want to read more than 1 byte
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	else
		TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR &(1<<TWINT)) == 0);
return TWDR;
}

void i2c_stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(TWSTO);
}