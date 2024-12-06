#include <Arduino.h>
#include "sensor_handler.h"
#include "display_manager.h"
#include "pin_config.h"
#include "wifi.h"

// Variable global para el contador de autos
int vehicle_count = 0;
bool relay_active = false;
unsigned long relay_timer_start = 0;
const long interval = 500;        // Time between sensor readings (in ms)

//Definimos el tiempo de activacion del relé
#define RELAY_DURATION 120000 // 2 minutos en milisegundos

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

    // Inicializa conexion Wifi
    connectToWiFi();

    // Inicializa los sensores ultrasónicos
    initSensors();

    // Inicializa el display OLED
    initDisplay();

    // Configurar el pin del relé como salida y apagarlo al inicio
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Relé apagado inicialmente
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
            // Realiza acciones relacionadas con el contador
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
