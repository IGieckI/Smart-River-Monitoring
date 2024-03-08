#include "WaterLevelMonitor.h"

WaterLevelMonitor::WaterLevelMonitor(uint8_t redLedPin, uint8_t greenLedPin, uint8_t echoPin, uint8_t trigPin, int maxDistanceTime) {
    greenLed = new Led(greenLedPin);
    redLed = new Led(redLedPin);
    distanceSensor = new Sonar(echoPin, trigPin, maxDistanceTime);
}

void WaterLevelMonitor::switchGreenLed(bool state) {
    greenLed->switchLight(state);
}

void WaterLevelMonitor::switchRedLed(bool state) {
    redLed->switchLight(state);
}

int WaterLevelMonitor::getWaterLevel(int minDistance) {
    int distance = distanceSensor->getDistance();
    
    if (distance < minDistance) {
        return 0;
    } else {
        return distance;
    }
}

void WaterLevelMonitor::wifiSetup(const char *SSID, const char *password) {
    Serial.print("Connecting to ");
	Serial.println(SSID);

	WiFi.begin(SSID, password);

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
	client = PubSubClient();
	
	Serial.println("---------------------");
	Serial.println("Server setup\n");
	Serial.println(domain);
	Serial.println(port);

	Serial.println("---------------------");
	delay(100);
    client.setServer(domain, port);
	while(!client.connect("ESP8266Client")) {
		Serial.println("Emmh");
		Serial.println(isConnectedToWifi() ? "WiFi connected" : "WiFi not connected");
		Serial.print("Server connection failed, state: ");
		Serial.println(client.state());
		delay(500);
	}

	Serial.println("Nice");
}

void WaterLevelMonitor::reconnect() {
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");

		if (client.connect("ESP8266Client")) {
			Serial.println("connected");
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

void WaterLevelMonitor::sendToServer(const char *topic, int waterLevel) {
	JsonDocument doc;
	doc["water_level"] = waterLevel;
	char mqtt_message[128];
	serializeJson(doc, mqtt_message);

    Serial.println(client.publish(topic, mqtt_message) ? "Message sent" : "Message failed");
	Serial.println(client.state());
}