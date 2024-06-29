void setup() {
    Serial.begin(9600);  // Start serial communication at 9600 baud
    pinMode(LED_BUILTIN, OUTPUT);  // Use the built-in LED for demonstration
}

void loop() {
    if (Serial.available() > 0) {
        char signal = Serial.read();
        if (signal == 'W') {  // Assuming 'W' stands for "wrong"
            digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED
            delay(100);  // Keep it on for 0.1 second
            digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
            Serial.println("Wrong label detected!");  // Print message to serial monitor
        }
    }
}
