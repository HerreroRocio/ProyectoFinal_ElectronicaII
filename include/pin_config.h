#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// Sensores ultrasónicos
#define TRIG_PIN2 26  // Sensor de egreso (TRIG)
#define ECHO_PIN2 25 // Sensor de egreso (ECHO)
#define TRIG_PIN1 19 // Sensor de ingreso (TRIG)
#define ECHO_PIN1 18 // Sensor de ingreso (ECHO)

// Relé (control de la luz)
#define RELAY_PIN 17

// Display OLED (I2C)
#define SDA_PIN 21 // Pin SDA
#define SCL_PIN 22 // Pin SCL

//--Hardware PINS definitions
#define ONBOARD_LED_PIN             2

//--Software constants
#define SERIAL_BAUDRATE             19200

#define WIFI_SSID "Fibertel598 2.4GHz"
#define WIFI_PASSWORD "00421413349"

//--Structures
struct Config {
  const char* device;
  const char* ssid;
  const char* ssid_pass;
  const char* ap_ssid;
  const char* ap_pass;
};

#endif // PIN_CONFIG_H