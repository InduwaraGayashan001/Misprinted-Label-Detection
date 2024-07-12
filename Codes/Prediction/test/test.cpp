#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL // Define CPU frequency for delay.h

const int stepDelay = 1000; // Delay between steps in microseconds

int count = 1; // Initialize count as a mutable variable
int push = 0;
int t = 0;

// Function to read a character from UART
char serial_read() {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data to be received
    return UDR0; // Get and return received data from buffer
}

// Function to read ADC value from given channel
int ADC_Read(uint8_t channel) {
    // Select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Start single conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    return ADC;
}

int main() {
    // Setup
    
    // Set direction and step pins as outputs for motor 1 and motor 2
    DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
    
    // Set initial direction for motor 2
    PORTD |= (1 << PD4); // Set PD4 high
    
    // Initialize serial communication for debugging
    UBRR0H = 0;
    UBRR0L = 103; // Baud rate 9600
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
    
    // ADC Initialization
    
    // AVcc with external capacitor at AREF pin
    ADMUX = (1 << REFS0);
    
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Main loop
    while (1) {
        // Check if data is received
        if (UCSR0A & (1 << RXC0)) {
            char signal = serial_read();
            if (signal == 'W') {
                push = 1;
            }
        }
        
        // Execute motor control if push is 1
        if (push == 1) {
            int sensorValueA4 = ADC_Read(4); // Read analog pin A4
            int sensorValueA5 = ADC_Read(5); // Read analog pin A5
            
            // Reset count based on sensor values
            if (sensorValueA4 > 1022 && t == 1) {
                count = 0; // Set count to 0 if sensorValueA4 > 1022
            } else if (sensorValueA5 > 1022) {
                count = 1; // Set count to 1 if sensorValueA5 > 1022
                PORTD &= ~(1 << PD2); // Set PD2 low
                
                // Step motor 1 for 50 steps
                for (int i = 0; i < 50; i++) {
                    PORTD |= (1 << PD3) | (1 << PD5); // Set PD3 and PD5 high
                    _delay_us(stepDelay);
                    PORTD &= ~((1 << PD3) | (1 << PD5)); // Set PD3
