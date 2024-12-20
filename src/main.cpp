#include <Arduino.h>
#include "sensor_handler.h"
#include "display_manager.h"
#include "pin_config.h"
#include "wifi.h"

// Global variable
int vehicle_count = 0;
bool relay_active = false;
unsigned long relay_timer_start = 0;
const long interval = 500;        // Time between sensor readings (in ms)

// Define the relay activation time
#define RELAY_DURATION 120000 // 2 minutes en miliseconds

// WiFi connection function
void connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);

    // Initialize ultrasonic sensors
    initSensors();

    // Initialize OLED Display
    initDisplay();

    // Set the relay pin as an output and turn it off at startup
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Relay initially off

     // Initialize Wifi connection
    connectToWiFi();
}

enum WiFiState { CONNECTING, CONNECTED, DISCONNECTED };
WiFiState wifiState = CONNECTING;

enum SystemState { IDLE, CHECK_ENTRY, CHECK_EXIT, UPDATE_DISPLAY, RELAY_CONTROL };
SystemState currentState = IDLE;

unsigned long lastStateChange = 0;

void loop() {
    unsigned long currentMillis = millis();
    switch (wifiState) {
        case CONNECTING:
            if (WiFi.status() == WL_CONNECTED) {
                wifiState = CONNECTED;
                Serial.println("WiFi Connected");
            }
            break;
        case CONNECTED:
            // Perform actions related to the counter
            break;
        case DISCONNECTED:
            connectToWiFi();
            wifiState = CONNECTING;
            break;
    }
    switch (currentState) {
        case IDLE:
            if (currentMillis - lastStateChange >= interval) {
                currentState = CHECK_ENTRY;
                lastStateChange = currentMillis;
            }
            break;

        case CHECK_ENTRY:
            if (checkVehicle(TRIG_PIN1, ECHO_PIN1, 0)) {
                vehicle_count++;
                Serial.println("Vehicle entered");
                digitalWrite(RELAY_PIN, HIGH);
                relay_active = true;
                relay_timer_start = millis();
            }
            currentState = CHECK_EXIT;
            break;

        case CHECK_EXIT:
            if (checkVehicle(TRIG_PIN2, ECHO_PIN2, 1)) {
                vehicle_count = max(0, vehicle_count - 1);
                Serial.println("Vehicle exited");
            }
            currentState = UPDATE_DISPLAY;
            break;

        case UPDATE_DISPLAY:
            updateDisplay(vehicle_count);
            currentState = RELAY_CONTROL;
            break;

        case RELAY_CONTROL:
            if (relay_active && (millis() - relay_timer_start >= RELAY_DURATION)) {
                digitalWrite(RELAY_PIN, LOW);
                relay_active = false;
                Serial.println("Relay turned off after 2 minutes");
            }
            currentState = IDLE;
            break;
    }
}
