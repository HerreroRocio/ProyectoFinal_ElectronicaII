#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#define WIFI_SSID "Fibertel598 2.4GHz"
#define WIFI_PASSWORD "00421413349"

void initWiFi();
bool isWiFiConnected();
void handleWiFiConnection();

#endif // WIFI_MANAGER_H