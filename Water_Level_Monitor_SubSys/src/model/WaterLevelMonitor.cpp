#include "WaterLevelMonitor.h"

WaterLevelMonitor::WaterLevelMonitor() {
    greenLed = new Led(GREEN_LED_PIN);
    redLed = new Led(RED_LED_PIN);
    distanceSensor = new Sonar(DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);
}

Led *WaterLevelMonitor::getGreenLed() {
    return greenLed;
}

Led *WaterLevelMonitor::getRedLed() {
    return redLed;
}

int WaterLevelMonitor::getWaterLevel() {
    int distance = distanceSensor->getDistance();
    
    if (distance < MIN_DISTANCE) {
        return 0;
    } else {
        return distance;
    }
}

void WaterLevelMonitor::wifiSetup(const char *SSID, const char *password) {
    Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void WaterLevelMonitor::serverSetup(const char *domain, int port) {
    client = PubSubClient(espClient);
    client.setServer(domain, port);
}

void WaterLevelMonitor::reconnect() {
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");

		if (client.connect("ESP8266Client")) {
			Serial.println("connected");
			client.subscribe(MQTT_TOPIC);
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			delay(5000);
		}
	}
}

bool WaterLevelMonitor::isConnectedToWifi() {
    return WiFi.status() == WL_CONNECTED;
}

bool WaterLevelMonitor::isConnectedToServer() {
    client.loop();
    return client.connected();
}

void WaterLevelMonitor::sendToServer(const char *topic, const char *message) {
    client.publish(topic, message);
}