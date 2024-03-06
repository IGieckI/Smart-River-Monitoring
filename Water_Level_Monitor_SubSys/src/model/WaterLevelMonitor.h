#ifndef __WATERLEVELMONITOR__
#define __WATERLEVELMONITOR__

#include <WiFi.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "components/distantiometer/Sonar.h"
#include "components/light/Led.h"

enum ConnectionType { NONE, WIFI, ETHERNET };

class WaterLevelMonitor {
    private:
        Led *greenLed;
        Led *redLed;
        DistanceSensor *distanceSensor;
        WiFiClient wifiClient;
        EthernetClient ethClient;
        PubSubClient client;

        ConnectionType connectionType;

        void reconnect();
    
    public:
        WaterLevelMonitor();
        Led *getGreenLed();
        Led *getRedLed();
        int getWaterLevel();
        bool isConnectedToWifi();
        bool isConnectedToServer();
        void wifiSetup(const char *SSID, const char *password);
        void ethernetSetup(const uint8_t *mac, const int *ip);
        void serverSetup(const char *domain, int port);
        void sendToServer(const char *topic, int waterLevel);
};

#endif
