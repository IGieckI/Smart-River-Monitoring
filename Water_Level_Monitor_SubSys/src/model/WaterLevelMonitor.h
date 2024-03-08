#ifndef __WATERLEVELMONITOR__
#define __WATERLEVELMONITOR__

#include <WiFi.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "components/distantiometer/Sonar.h"
#include "components/light/Led.h"

class WaterLevelMonitor {
    private:
        Led *greenLed;
        Led *redLed;
        DistanceSensor *distanceSensor;
        WiFiClient wifiClient;
        PubSubClient client;

        void reconnect();
    
    public:
        WaterLevelMonitor(uint8_t redLedPin, uint8_t greenLedPin, uint8_t echoPin, uint8_t trigPin, int maxDistanceTime);
        void switchRedLed(bool state);
        void switchGreenLed(bool state);
        int getWaterLevel(int minDistance);
        bool isConnectedToWifi();
        bool isConnectedToServer();
        void wifiSetup(const char *SSID, const char *password);
        void ethernetSetup(const uint8_t *mac, const int *ip);
        void serverSetup(const char *domain, int port);
        void sendToServer(const char *topic, int waterLevel);
};

#endif
