// Aug 25, 2024
// Pin Definitions
int IN1pin = 7;  // Motor driver IN1 pin
int IN2pin = 8;  // Motor driver IN2 pin
int ENApin = 9;  // Motor driver ENA pin (PWM)
int RelayPin = A1;  // Relay control pin
int analogPin = A0;  // Current sensor output pin
int potPin = A2;  // Potentiometer input pin
int buttonPin = 3;  // Tactile button input pin

// Variables
long int sensorValue = 0;  // Variable to store the current sensor value
int potValue = 0;  // Variable to store the potentiometer value
int motorSpeed = 0;  // Variable to store the motor speed (PWM value)
bool motorStopped = false;  // Flag to indicate if the motor is stopped

void setup() {
  // Configure Arduino pins as outputs
  pinMode(IN1pin, OUTPUT);
  pinMode(IN2pin, OUTPUT);
  pinMode(ENApin, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor

  // Set Timer1 to Fast PWM mode for the motor driver
  TCCR1B = TCCR1B & 0b11111000 | 0x01;

  // Ensure the relay is initially allowing power (NC state)
  digitalWrite(RelayPin, HIGH);

  // Initialize serial communication for debugging
  Serial.begin(9600);
  delay(100); // Small delay to stabilize serial communication
}

void loop() {
  // Read the potentiometer value
  potValue = analogRead(potPin);

  // Map the potentiometer value (0-1023) to the desired PWM range (100-255)
  motorSpeed = map(potValue, 0, 1023, 100, 255);

  // Read the current sensor value
  sensorValue = analogRead(analogPin);

  // Output the sensor value to the Serial Monitor
  Serial.print("ADC Value: ");
  Serial.println(sensorValue);

  // Output the motor speed to the Serial Monitor
  Serial.print("Motor Speed: ");
  Serial.println(motorSpeed);

  // Check if the motor is currently stopped
  if (motorStopped) {
    // Check if the button is pressed (LOW state)
    if (digitalRead(buttonPin) == LOW) {
      motorStopped = false;  // Reset the flag to allow motor operation
      delay(200);  // Debounce delay
    }
  } else {
    // Overload protection: check if the sensor value exceeds the threshold
    if (sensorValue > 600) {  // Adjust threshold based on your observed readings
      digitalWrite(RelayPin, LOW);  // Activate relay to open the circuit and stop the motor
      motorStopped = true;  // Set the flag to indicate the motor is stopped
    } else {
      digitalWrite(RelayPin, HIGH);  // Deactivate relay to close the circuit and allow motor operation

      // Turn the motor in one direction with the PWM value set by the potentiometer
      digitalWrite(IN1pin, HIGH);
      digitalWrite(IN2pin, LOW);
      analogWrite(ENApin, motorSpeed);  // Set motor speed based on potentiometer input
    }
  }

  // Set a delay to allow time for readings
  delay(200);
}
