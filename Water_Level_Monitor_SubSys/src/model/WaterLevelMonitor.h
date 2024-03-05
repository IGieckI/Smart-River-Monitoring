#ifndef __WATERLEVELMONITOR__
#define __WATERLEVELMONITOR__

#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"
#include "components/distantiometer/Sonar.h"
#include "components/light/Led.h"

class WaterLevelMonitor {
    private:
        Led *greenLed;
        Led *redLed;
        DistanceSensor *distanceSensor;
        WiFiClient espClient;
        PubSubClient client;

        void reconnect();
    
    public:
        WaterLevelMonitor();
        Led *getGreenLed();
        Led *getRedLed();
        int getWaterLevel();
        bool isConnectedToWifi();
        bool isConnectedToServer();
        void wifiSetup(const char *SSID, const char *password);
        void serverSetup(const char *domain, int port);
        void sendToServer(const char *topic, const char *message);
};

#endif
