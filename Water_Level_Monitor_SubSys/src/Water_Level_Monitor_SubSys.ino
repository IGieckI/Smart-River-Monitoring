
#include <Config.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "model/WaterLevelMonitor.h"

WaterLevelMonitor model(RED_LED_PIN, GREEN_LED_PIN, DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
	delay(1000);
  	Serial.begin(115200);

	model.switchRedLed(true);
	model.switchGreenLed(false);

	wifiSetup(WIFI_SSID, WIFI_PASSWORD);
  	serverSetup(MQTT_SERVER, MQTT_PORT);
}

void loop() {
	if (isConnectedToServer() && isConnectedToWifi()) {
		model.switchGreenLed(true);
	  	model.switchRedLed(false);
  	} else {
		model.switchRedLed(true);
	  	model.switchGreenLed(false);
	}

	short waterLevel = model.getWaterLevel(MIN_DISTANCE);

	Serial.print("Water level: ");
	Serial.println(waterLevel);

	sendToServer(MQTT_TOPIC, waterLevel);

	delay(UPDATE_FREQUENCY);
}

void wifiSetup(const char *SSID, const char *password) {
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

void serverSetup(const char *domain, int port) {
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

void reconnect() {
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

void sendToServer(const char *topic, int waterLevel) {
	JsonDocument doc;
	doc["water_level"] = waterLevel;
	char mqtt_message[128];
	serializeJson(doc, mqtt_message);

    Serial.println(client.publish(topic, mqtt_message) ? "Message sent" : "Message failed");
	Serial.println(client.state());
}

bool isConnectedToWifi() {
    return WiFi.status() == WL_CONNECTED;
}

bool isConnectedToServer() {
    client.loop();
    return client.connected();
}