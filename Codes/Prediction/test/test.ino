const int dirPin = 2;        // Direction pin for motor 1
const int stepPin = 3;       // Step pin for motor 1
const int dirPin1 = 4;       // Direction pin for motor 2
const int stepPin1 = 5;      // Step pin for motor 2
const int stepDelay = 1000;   // Delay between steps in microseconds
int count = 1;     // Initialize count as a mutable variable
int push = 0;     
int t = 0;   
int i = 0;  

const int sensorPinA4 = A4;   // Analog pin A4
const int sensorPinA5 = A5;   // Analog pin A5

void setup() {
  pinMode(dirPin, OUTPUT);    // Set direction and step pins as outputs for motor 1
  pinMode(stepPin, OUTPUT);
  
  pinMode(dirPin1, OUTPUT);   // Set direction and step pins as outputs for motor 2
  pinMode(stepPin1, OUTPUT);
  
  digitalWrite(dirPin1, HIGH); // Set initial direction for motor 2

  Serial.begin(9600);         // Initialize serial communication for debugging
}

void loop() {
  if (Serial.available() > 0) {
    char signal = Serial.read();
    if (signal == 'W') {
      push = 1;
    }
  }



    if (push == 1) {  // Respond to serial signal 'W'

      

      int sensorValueA4 = analogRead(sensorPinA4);
      int sensorValueA5 = analogRead(sensorPinA5);

      
   

      // Reset count based on sensor values
      if (sensorValueA4 > 1022 && t==1) {
        count = 0;  // Set count to 0 if sensorValueA4 > 1000
      } else if (sensorValueA5 > 1022) {
        count = 1;  // Set count to 1 if sensorValueA5 > 1000
        digitalWrite(dirPin, LOW);

         for (int i = 0; i < 50; i++) {
        digitalWrite(stepPin, HIGH);
        digitalWrite(stepPin1, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        digitalWrite(stepPin1, LOW);
        delayMicroseconds(stepDelay);
      }


        push = 0;
      }

      if (sensorValueA4 > 1022) {
        t=1; 
      }

      // Set dirPin LOW if count is 1, otherwise set it HIGH for motor 1
      if (count == 1) {
        digitalWrite(dirPin, LOW);
      } else {
        digitalWrite(dirPin, HIGH);
      }

      // Step both motors simultaneously
      digitalWrite(stepPin, HIGH);
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(stepDelay);
    }
   else {
    // Default behavior when no serial signal is received
    digitalWrite(stepPin1, HIGH);   // Step motor 2
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(stepDelay);
  }
}
