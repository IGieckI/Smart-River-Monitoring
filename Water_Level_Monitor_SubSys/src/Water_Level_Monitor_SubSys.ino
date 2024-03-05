
#include <Config.h>
#include "model/WaterLevelMonitor.h"

WaterLevelMonitor model;

void setup() {
  	Serial.begin(115200);
	
	model.getRedLed()->switchLight(true);
	model.getGreenLed()->switchLight(false);

  	model.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
  	model.serverSetup(MQTT_SERVER, MQTT_PORT);
}

void loop() {
	if (!(model.isConnectedToServer() && model.isConnectedToWifi())) {
		model.getRedLed()->switchLight(true);
	  	model.getGreenLed()->switchLight(false);
  	} else {
		model.getGreenLed()->switchLight(true);
	  	model.getRedLed()->switchLight(false);
	}

	short waterLevel = model.getWaterLevel();

	model.sendToServer(MQTT_TOPIC, String(waterLevel).c_str());

	delay(UPDATE_FREQUENCY);
}
