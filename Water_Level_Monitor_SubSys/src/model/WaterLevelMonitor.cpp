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
	Serial.println(SSID);

	WiFi.begin(SSID, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	connectionType = ConnectionType::WIFI;
	
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void WaterLevelMonitor::ethernetSetup(const byte *mac, const int *ipArray) {
	byte currentMac[6];

	for (int i=0; i<6; i++) {
		currentMac[i] = mac[i];	
	}	
	
	IPAddress ip(ip[0], ip[1], ip[2], ip[3]);
	Ethernet.begin(currentMac, ip);
	

	connectionType = ConnectionType::ETHERNET;
	Serial.println(ethClient.connected());
}

void WaterLevelMonitor::serverSetup(const char *domain, int port) {
	if (connectionType == WIFI) {
		client = PubSubClient(wifiClient);
	} else {
		client = PubSubClient(ethClient);
	}
	
	Serial.println("---------------------");
	
	Serial.println(domain);
	Serial.println(port);

	Serial.println("---------------------");

    client.setServer(domain, port);
	Serial.println(domain);
	Serial.println(port);
	
	if (client.connect("ESP8266Client")) {
		Serial.println("Nice");
	} else {
		Serial.print("Server connection failed, state: ");
		Serial.println(client.state());
	}
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

void WaterLevelMonitor::sendToServer(const char *topic, int waterLevel) {
	JsonDocument doc;
	doc["water_level"] = waterLevel;
	char mqtt_message[128];
	serializeJson(doc, mqtt_message);

    Serial.println(client.publish("water_level", "{ \"level\": 10 }") ? "Message sent" : "Message failed");
	Serial.println(client.state());
}