#include "sensor_handler.h"
#include <Arduino.h>
#include "pin_config.h"

// Define thresholds and constants
#define DETECTION_THRESHOLD 10 // Threshold in cm to detect an object
#define MAX_DISTANCE 400       // Maximum distance the sensor can detect
#define NO_OBJECT -1           // Value when no object is detected

// Variables to store the current and previous state of sensors
bool sensorActive[] = {false, false}; // States for both sensors

// Initialize the pins for the ultrasonic sensors
void initSensors() {
    pinMode(TRIG_PIN1, OUTPUT);
    pinMode(ECHO_PIN1, INPUT);
    pinMode(TRIG_PIN2, OUTPUT);
    pinMode(ECHO_PIN2, INPUT);
}

// Measure the distance using ultrasonic sensors
long measureDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000); // Timeout of 30 ms
    if (duration == 0) {
        return NO_OBJECT; // No response from sensor
    }

    long distance = (duration / 2) * 0.0343; // Calculate distance in cm
    if (distance > MAX_DISTANCE) {
        return NO_OBJECT; // Distance out of range
    }

    return distance;
}

// Generalized function to check vehicle entry or exit
bool checkVehicle(int trigPin, int echoPin, int sensorIndex) {
    long distance = measureDistance(trigPin, echoPin);

    if (distance < DETECTION_THRESHOLD && distance != NO_OBJECT) {
        if (!sensorActive[sensorIndex]) { // Detect only if previously inactive
            sensorActive[sensorIndex] = true; // Mark sensor as active
            return true;                      // Vehicle detected
        }
    } else {
        sensorActive[sensorIndex] = false; // Reset when object moves away
    }

    return false;
}

