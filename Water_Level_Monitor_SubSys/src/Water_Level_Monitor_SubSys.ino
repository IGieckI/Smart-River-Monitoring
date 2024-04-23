#include <Config.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "model/WaterLevelMonitor.h"

#define MSG_BUFFER_SIZE  50
#define MAX_WATER_LEVEL 4000

WaterLevelMonitor model(RED_LED_PIN, GREEN_LED_PIN, DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];


void setup_wifi() {
	Serial.println(String("Connecting to ") + WIFI_SSID);

	WiFi.mode(WIFI_STA);
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
    
    String clientId = String("esiot-2122-client-") + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
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

void setup()	{
	Serial.begin(115200);
    delay(1000);

	model.switchRedLed(true);
	model.switchGreenLed(false);

	setup_wifi();
	randomSeed(micros());

	client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {

	if (!client.connected()) {
		model.switchGreenLed(false);
		model.switchRedLed(true);
		reconnect();
	}

	model.switchGreenLed(true);
	model.switchRedLed(false);
	
	client.loop();

	unsigned short waterLevel = model.getWaterLevel(MIN_DISTANCE);

	unsigned long now = millis();
	if (now - lastMsgTime > UPDATE_FREQUENCY && waterLevel < MAX_WATER_LEVEL) {
		lastMsgTime = now;

		snprintf (msg, MSG_BUFFER_SIZE, "{ \"water_level\": %ld }", waterLevel);

		Serial.println(String("Sending message: ") + msg);

		client.publish(MQTT_TOPIC, msg);
	}
}