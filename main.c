#include <avr/io.h>  // the header file is required for using avr input output functionaluty
#include <stdlib.h>   // contains the itoa() function required for converting integer into array of characters 
#include <util/delay.h>  // contains the delay functions 

void initialize_serial_communication(){
	 
	 int baud_value = 25;          //   it sets the speed of comunication as 2400 bits per second with the serial monitor  
	 DDRD |= 1 << PORTD1;  // setting tx pin as output
     UBRRL = (unsigned char) baud_value;         // setting the baud rate to the register
     UCSRB =  (1 << TXEN);       // we need to enable the uart transmission
     UCSRC = (1 << USBS) | (3 << UCSZ0);     //Set 2 stop bits and data bit length is 8-bit
}

/*
using the uart_byte() function we can only send one byte of data to the
serial monitor
*/

void uart_byte(unsigned char c){
	while (! (UCSRA & (1 << UDRE)) );
    UDR  =  c;       //  the UDR register holds the value being sent to through the uart communication
}

/*
the send_string can send multiple bytes of data at a time. It can send a complete string utilizing the uart_byte() function
*/

void send_string(char *string){      // pass the string directly to the function
	int i;
	for(i=0; string[i] != '\0'; i++)     //  this loop will run until the end of the string
	{
		uart_byte(string[i]);   // sending each character of the string individually
	}

}

unsigned int read_sensor(){
	ADCSRA  |= (1<<ADSC);	             // it will start analog to digital conversion
	while (ADCSRA &  (1<<ADSC));   // wait until the conversion is complete
	return ADCW;	// this will return the stored ADC value to the main function
}

int main()
 { 
	initialize_serial_communication();    // this function would setup the necessary things for serial communication
	char num_to_show[4];   // integer can not be directly displayed on the serial monitor. this function will be used to store converted data
	
	int currentValue = 0;
	DDRD |= 1 << PD2;                    // we are defining port D pin 2 as output. We operate the motor using this pin
	PORTD  &= ~(1 << PD2);          // we are turning the motor off initialy

 	ADCSRA  = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);    // setting division factor of 32 for the ADC
	ADMUX = 0x01;     // setting ADC1 as the input pin
	_delay_ms(100);
	
   while (1){
		currentValue = read_sensor();
		itoa(currentValue, num_to_show, 10);  // this function converts the integer into character array
		send_string(" Sensor Value = ");
		send_string(num_to_show);
		send_string("\n\r");  // this will print a new line
		_delay_ms(250);
			
		
	}
 
   return 0;
 }
