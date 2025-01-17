#include <Arduino.h>
#include "sensor_handler.h"
#include "pin_config.h"
#include "wifi_manager.h"
#include "display_manager.h"
#include "fsm_manager.h"
#include "wifi_manager.h"

void setup() {
    Serial.begin(115200);

    // Initialize ultrasonic sensors
    initSensors();

    // Initialize OLED Display
    initDisplay();

    // Set the relay pin as an output and turn it off at startup
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Relay initially off

    // Initialize WiFi
    initWiFi();
}

unsigned long lastStateChange = 0;

void loop() {

     // Ensure WiFi stays connected
    handleWiFiConnection();
    
       // Ejecutar la FSM
    runFSM();
}
