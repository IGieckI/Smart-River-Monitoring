
#include <Config.h>
#include "model/WaterLevelMonitor.h"

WaterLevelMonitor model;

void setup() {
	delay(3000);
  	Serial.begin(115200);
	
	model.getRedLed()->switchLight(true);
	model.getGreenLed()->switchLight(false);

  	model.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
	//model.ethernetSetup(MAC, IP);
  	model.serverSetup(MQTT_SERVER, MQTT_PORT);
}

void loop() {
	if (model.isConnectedToServer() && model.isConnectedToWifi()) {
		model.getGreenLed()->switchLight(true);
	  	model.getRedLed()->switchLight(false);
  	} else {
		model.getRedLed()->switchLight(true);
	  	model.getGreenLed()->switchLight(false);		
	}

	short waterLevel = model.getWaterLevel();

	Serial.print("Water level: ");
	Serial.println(waterLevel);

	model.sendToServer(MQTT_TOPIC, waterLevel);

	delay(UPDATE_FREQUENCY);
}
