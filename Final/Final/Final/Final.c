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

char Xout[10]; // output buffer for Acceleration X-axis
char Yout[10]; // output buffer for Acceleration y-axis
char Zout[10];// output buffer for Acceleration z-axis
char GyroXout[10]; // output buffer for Gyro X axis
char GyroYout[10]; // output buffer for Gyro Y axis
char GyroZout[10]; // output buffer for Gyro Z axis
char DATA_STR[120]; // buffer for string that sends data to cloud
char WIFI_COMMAND[] = "AT+CWJAP=\"XXX\",\"xxx\"\r\n\0"; // String to connect to Wifi
char CONNECT_COMMAND[] = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n\0";// Command to connect to Thingspeak.com
char SEND_COMMAND[20]; // Buffer for string to tell wifi how many characters will be sent
// Wifi connect:  AT+CWJAP="SSID", "PASSWORD"
// Connect to Thinkspeak: AT+CIPSTART= "TCP","%s(IP)" , 80 *Add \ in front of " for it to be read properly 
// Send data: AT+CIPSEND=(string.length())
// Data string to send: "GET /update?api_key=%s&field1=(sensor value)\r\n\r\n",APIKEY,Sensor)

int main(void)
{
	int counter; // Counter for number of characters to be sent
	uint8_t ret; 
	int16_t accel_buff[3];// buffer to hold acceleration raw data
	int16_t gyro_buff[3]; // buffer to hold gyroscope raw data
	double accelX, accelY, accelZ; // Holds calculated acceleration values 
	double gyroX, gyroY, gyroZ; // Holds calculated gyroscope values
	sei (); // enable global interrupts
	USARTstart(UBRR_115200); // initialize the USART port
	i2c_init();// initialize I2C
	
//	Build command connect to Wifi and transmit
// 	strcat(WIFI_COMMAND,SSID);
// 	strcat(WIFI_COMMAND,PASSWORD);
//	USART_tx_string(WIFI_COMMAND);// Connect to Wifi

	DDRB |= (1<<5);
		
	//initialize & test MPU5060 availability
	ret = i2c_start(MPU6050_ADDRESS+I2C_WRITE);
	if(~ret)
	{
  		PORTB |= (1<<5);
  		_delay_ms(200);
  		PORTB &= ~(1<<5);
	}

	mpu6050_init();// Initialize MPU
	


	for (int b=0; b<10;b++) // Delay to give wifi time to connect and be ready
		_delay_ms(1000);
		
	while (1) // infinite loop to transmit the sensor value over serial port
	{
		counter = 0;
		mpu6050_read_accel_ALL(accel_buff);// Read acceleration data
		mpu6050_read_gyro_ALL(gyro_buff);// Read gyroscopic data
			
		// acceleration (m/s^2)
		accelX = accel_buff[0]/16384.0; // Calculate Acceleration X-axis value
		accelY = accel_buff[1]/16384.0; // Calculate Acceleration Y-axis value
		accelZ = accel_buff[2]/16384.0; // Calculate Acceleration Z-axis value
		//
		gyroX = gyro_buff[0]/32.8; // Calculate Gyroscopic X-axis value
		gyroY = gyro_buff[1]/32.8; // Calculate Gyroscopic Y-axis value
		gyroZ = gyro_buff[2]/32.8; // Calculate Gyroscopic Z-axis value
		
		//Convert Acceleration X,Y, and Z values to Strings
		if (accelX < 0)
			dtostrf(accelX, 6, 3, Xout);
		else
		{
			dtostrf(accelX, 6, 3, Xout);
			memmove(Xout, Xout+1,sizeof(Xout)); 
		}
		if (accelY < 0)
			dtostrf(accelY, 6, 3, Yout);
		else
		{
			dtostrf(accelY, 6, 3, Yout);
			memmove(Yout, Yout+1,sizeof(Yout));
		}
		if (accelZ < 0)
			dtostrf(accelZ, 6, 3, Zout);
		else
		{
			dtostrf(accelZ, 6, 3, Zout);
			memmove(Zout, Zout+1,sizeof(Zout));
		}
		
		// Convert Gyroscopic X,Y, and Z values to strings		
		if (gyroX < 0)
			dtostrf(gyroX, 6, 3, GyroXout);
		else
		{
			dtostrf(gyroX, 6, 3, GyroXout);
			memmove(GyroXout, GyroXout+1,sizeof(GyroXout));
		}
		if (gyroY < 0)
		dtostrf(gyroY, 6, 3, GyroYout);
		else
		{
			dtostrf(gyroY, 6, 3, GyroYout);
			memmove(GyroYout, GyroYout+1,sizeof(GyroYout));
		}
		if (gyroZ < 0)
		dtostrf(gyroZ, 6, 3, GyroZout);
		else
		{
			dtostrf(gyroZ, 6, 3, GyroZout);
			memmove(GyroZout, GyroZout+1,sizeof(GyroZout));
		}
		
		// Build Data string to send to cloud
		snprintf(DATA_STR, sizeof(DATA_STR),"GET /update?api_key=U6KEQ2CNJNAK85WX&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s\r\n", Xout, Yout, Zout, GyroXout, GyroYout, GyroZout);
	
		while (DATA_STR[counter] != '\0') // Count how many characters in the output string
		{
			counter++;
		}
		// Build Command to Send the length of the string to follow
		snprintf(SEND_COMMAND, sizeof(SEND_COMMAND),"AT+CIPSEND=%d\r\n",counter);

		USART_tx_string(CONNECT_COMMAND);// Connect to Thinkspeak.com
		_delay_ms(400);
		USART_tx_string(SEND_COMMAND);// Transmit command to send data
		_delay_ms(100);
		USART_tx_string(DATA_STR); // transmit data over serial connection
		for (int i = 0; i<16; i++)// Wait 15sec to upload next dataset
			_delay_ms(1000);
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