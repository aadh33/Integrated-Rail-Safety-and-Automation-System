#include <Servo.h>

// Create servo objects
Servo servoMotor1;
Servo servoMotor2;

// Define the IR sensor pin
const int IR_SENSOR_PIN = A0;  // Analog pin A0 for IR sensor input

// Define the servo motor pins
const int servoPin1 = 9;  // PWM pin 9 for servo control (Servo 1)
const int servoPin2 = 10; // PWM pin 10 for servo control (Servo 2)

// Variables to track object detection and servo positions
bool wasObjectDetected = false;
int servoPosition1 = 0;  // Initial position for Servo 1 (0 degrees)
int servoPosition2 = 0;  // Initial position for Servo 2 (0 degrees)

// Variables for tracking time and servo states
unsigned long lastObjectDetectedTime = 0;  // Time when object was last detected
const unsigned long detectionTimeout = 4000;  // Timeout period in milliseconds (4 seconds)
bool servoMoved = false;  // Flag to track if servo was moved

void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Attach the servo motors to the pins
  servoMotor1.attach(servoPin1);
  servoMotor2.attach(servoPin2);

  // Set up the IR sensor pin as an input
  pinMode(IR_SENSOR_PIN, INPUT);

  // Initialize servo motor positions to 0 degrees
  servoMotor1.write(servoPosition1);
  servoMotor2.write(servoPosition2);
}

void loop() {
  // Read the analog value from the IR sensor
  int sensorValue = analogRead(IR_SENSOR_PIN);

  // Determine if an object is detected based on the sensor reading
  bool isObjectDetected = (sensorValue > 500);  // Adjust threshold as needed

  // Check if object detection state has changed
  if (isObjectDetected != wasObjectDetected) {
    // Print detection status to Serial Monitor
    if (isObjectDetected) {
      Serial.println("Object Detected!");
      // Reset the timer when an object is detected
      lastObjectDetectedTime = millis();

      // If servos were previously moved, return them to initial positions
      if (servoMoved) {
        servoPosition1 = 0;
        servoPosition2 = 0;
        servoMotor1.write(servoPosition1);
        servoMotor2.write(servoPosition2);
        Serial.println("Returning servos to initial positions");
        servoMoved = false;  // Reset servo moved flag
      }
    } else {
      Serial.println("No Object Detected");
    }

    // Update previous detection state
    wasObjectDetected = isObjectDetected;
  }

  // Check for continuous timeout (No object detected for the entire timeout period)
  if (!isObjectDetected && (millis() - lastObjectDetectedTime >= detectionTimeout) && !servoMoved) {
    // Move the servo motors to new positions (e.g., 90 degrees)
    servoPosition1 = 90;
    servoPosition2 = 90;
    servoMotor1.write(servoPosition1);
    servoMotor2.write(servoPosition2);

    // Print a message indicating servo movement
    Serial.println("Moving servos due to timeout (No Object Detected for 4 seconds)");

    // Set servo moved flag to true
    servoMoved = true;
  }

  // Add a delay to control the rate of sensor readings (optional)
  delay(100);  // Adjust delay as needed
}

