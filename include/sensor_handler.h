#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

// Functions for the initialization and control of the ultrasonic sensors
void initSensors();
// Generalized function to check vehicle detection
bool checkVehicle(int trigPin, int echoPin, int sensorIndex);
long measureDistance(int trigPin, int echoPin);

#endif // SENSOR_HANDLER_H
