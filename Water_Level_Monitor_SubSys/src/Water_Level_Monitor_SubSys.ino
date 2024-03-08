
#include <Config.h>
#include "model/WaterLevelMonitor.h"

WaterLevelMonitor model(RED_LED_PIN, GREEN_LED_PIN, DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);

void setup() {
	delay(1000);
  	Serial.begin(115200);

	model.switchRedLed(true);
	model.switchGreenLed(false);

  	model.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
	//model.ethernetSetup(MAC, IP);
  	model.serverSetup(MQTT_SERVER, MQTT_PORT);
}

void loop() {
	if (model.isConnectedToServer() && model.isConnectedToWifi()) {
		model.switchGreenLed(true);
	  	model.switchRedLed(false);
  	} else {
		model.switchRedLed(true);
	  	model.switchGreenLed(false);
	}

	short waterLevel = model.getWaterLevel(MIN_DISTANCE);

	Serial.print("Water level: ");
	Serial.println(waterLevel);

	model.sendToServer(MQTT_TOPIC, waterLevel);

	delay(UPDATE_FREQUENCY);
}
