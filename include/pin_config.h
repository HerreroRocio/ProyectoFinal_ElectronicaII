#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// Ultrasonic sensors
#define TRIG_PIN2 26  // Exit sensor (TRIG)
#define ECHO_PIN2 25 // Exit sensor (ECHO)
#define TRIG_PIN1 19 // Entry sensor (TRIG)
#define ECHO_PIN1 18 // Entry sensor (ECHO)

// Relay (light control)
#define RELAY_PIN 17

// OLED Display (I2C)
#define SDA_PIN 21 // Pin SDA
#define SCL_PIN 22 // Pin SCL

//--Hardware PINS definitions
#define ONBOARD_LED_PIN             2

//--Software constants
#define SERIAL_BAUDRATE             19200

#endif // PIN_CONFIG_H