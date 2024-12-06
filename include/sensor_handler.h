#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

// Funciones para la inicialización y control de los sensores ultrasónicos
void initSensors();
// Generalized function to check vehicle detection
bool checkVehicle(int trigPin, int echoPin, int sensorIndex);
long measureDistance(int trigPin, int echoPin);

#endif // SENSOR_HANDLER_H
