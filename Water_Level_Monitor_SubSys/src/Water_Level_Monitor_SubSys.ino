#include <WiFi.h>
#include <PubSubClient.h>
#include <Config.h>
#include "model/WaterLevelMonitor.h"

TaskHandle_t communication_task, data_task;

WiFiClient espClient;
PubSubClient client(espClient);

WaterLevelMonitor model;

int cont = 0;

void setup() {
  Serial.begin(115200);

  setup_wifi();

  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  	client.loop();
	if (!client.connected()) {
		model.getRedLed()->switchLight(true);
	  	model.getGreenLed()->switchLight(false);
		reconnect();
		model.getGreenLed()->switchLight(true);
	  	model.getRedLed()->switchLight(false);
  	}

	short waterLevel = cont++;//model.getWaterLevel();

	client.publish(MQTT_TOPIC, String(waterLevel).c_str());

	delay(UPDATE_FREQUENCY);
}

void setup_wifi() {
	delay(5000);
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

void reconnect() {
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
