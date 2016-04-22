/*
 * Final.c
 *
 * Created: 4/18/2016 9:35:35 PM
 * Author : sirfe
 */ 

#define F_CPU 8000000UL // Set clock to 8MHz
#define UBRR_115200 8	//For 8Mhz

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "i2c.h"

void USARTstart(unsigned int ubrr); // Function to initialize the USART port
void USART_tx_string(char *data); // Function to transmit the value over the serial port
// void i2c_init();// initialize sensor
// void i2c_start();// tell sensor to start sending data
// void i2c_write(unsigned char data); //write data to sensor
// void i2c_read(unsigned char	isLast);// read data from sensor
// void i2c_stop(); // stop sensor

char Xout[10]; // output buffer for X-axis
char Yout[10]; // output buffer for y-axis
char Zout[10];// output buffer for z-axis
char SSID[]= "\"XXX\", ";
char PASSWORD[]= "\"xxx\"\r\n";
char ADAFRUITIP[]= "\"io.adafruit.com\"";
char PORTNUM[]= "\",80\r\n";
char APIKEY[]="00af961cb3c84d5f840c2a60390fd37a";
char DATA_STR[70];
char WIFI_COMMAND[] = "AT+CWJAP=";;
char CONNECT_COMMAND[] = "AT+CIPSTART=\"TCP\",\"";
char SEND_COMMAND[20];
// Wifi connect:  AT+CWJAP="SSID", "PASSWORD"
// Connect to Adafruit: AT+CIPSTART= "TCP","%s(IP)" , 80 *Add \ in front of " for it to be read properly 
// Send data: AT+CIPSEND=(string.length())
// Data string to send: "GET /update?api_key=%s&field1=(sensor value)\r\n\r\n",APIKEY,Sensor)

int main(void)
{
	sei (); // enable global interrupts
	USARTstart(UBRR_115200); // initialize the USART port
	i2c_init();// initialize I2C	
	
	DDRB |= (1<<5);

	uint8_t ret;
	int16_t accel_buff[3];
	double accelX, accelY, accelZ;
	//Build command connect to Wifi and transmit
	strcat(WIFI_COMMAND,SSID);
	strcat(WIFI_COMMAND,PASSWORD);
	USART_tx_string(WIFI_COMMAND);// Connect to IO.Adafruit	
	//initialize & test MPU5060 availability
	ret = i2c_start(MPU6050_ADDRESS+I2C_WRITE);
	USART_tx_string("HELLO2\r\n");
 	if(~ret)
 		{
 			PORTB |= (1<<5);
 			_delay_ms(200);
 			PORTB &= ~(1<<5);
 		}
	mpu6050_init();// Initialize MPU
	USART_tx_string("HELLO3\r\n");
	// Build Command to connect to Cloud
	strcat(CONNECT_COMMAND,ADAFRUITIP);
	strcat(CONNECT_COMMAND,PORTNUM);

	USART_tx_string(CONNECT_COMMAND);// Connect to IO.Adafruit
	USART_tx_string("HELLO5\r\n");
	while (1) // infinite loop to transmit the sensor value over serial port
	{
		USART_tx_string("HELLO6\r\n");
		mpu6050_read_accel_ALL(accel_buff);
			
		// acceleration (m/s^2)
		accelX = accel_buff[0]*9.8*2/32768;
		accelY = accel_buff[1]*9.8*2/32768;
		accelZ = accel_buff[2]*9.8*2/32768;
		
		snprintf(Xout, sizeof(Xout),"%3f", accelX); // convert value to a string
		snprintf(Yout, sizeof(Yout),"%3f", accelY); // convert value to a string
		snprintf(Zout, sizeof(Zout),"%3f", accelZ); // convert value to a string
		// Build Data string to send to cloud
		snprintf(DATA_STR, sizeof(DATA_STR),"GET /update?api_key=%s&field1=%.2f&field2=%.2f&field3=%.2f\r\n",APIKEY,accelX,accelY,accelZ);
//  		strcat(&DATA_STR,APIKEY);// Add API key to string
//  		strcat(&DATA_STR,"&field1="); // Identify what field to send
//  		strcat(&DATA_STR,Xout);// add sensor value
//  		strcat(&DATA_STR,"&field2=");
//  		strcat(&DATA_STR,Yout);
//  		strcat(&DATA_STR,"&field3=");
//  		strcat(&DATA_STR,Zout);
//  		strcat(&DATA_STR,"\r\n");// add line feed
		
		// Build Command to Send
		snprintf(SEND_COMMAND, sizeof(SEND_COMMAND),"AT+CIPSEND=%d\r\n",sizeof(DATA_STR));
//  		strcat(&SEND_COMMAND,sizeof(DATA_STR));// Length of string to be sent
//  		strcat(&SEND_COMMAND,"\r\n");// add line feed
		USART_tx_string(CONNECT_COMMAND);// Connect to IO.Adafruit
		_delay_ms(10);
		USART_tx_string(SEND_COMMAND);// Transmit command to send data
		_delay_ms(10);
		USART_tx_string(DATA_STR); // transmit data over serial connection
		_delay_ms(10);
	}
	
	return 0;
}

void USARTstart(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);// set baud rate to 115200
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
/*
void i2c_init()
{
	TWSR= 0x00; // set prescaler bits to zero
	TWBR= 0x0C;	// SCL Freq is 400k for XTAL = 16M
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
}*/