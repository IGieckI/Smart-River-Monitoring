#include <Config.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "model/WaterLevelMonitor.h"

#define MSG_BUFFER_SIZE  50

WaterLevelMonitor model(RED_LED_PIN, GREEN_LED_PIN, DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];

// Current state of the connection
enum ConnectionState {
    DISCONNECTED,
    CONNECTED_TO_WIFI,
    CONNECTED_TO_SERVER,
    EXCHANGING_WATER_DATA
};

ConnectionState connectionState = DISCONNECTED;

int warning_limit = 0, f1 = 0, f2 = 0;
String water_topic;

// Connect to the WiFi network using credentials in the Config.h file
void setup_wifi() {
	Serial.println(String("Connecting to ") + WIFI_SSID);

	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

    connectionState = CONNECTED_TO_WIFI;

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

// Reconnect to the MQTT server, and exchange initialization data
void reconnect() {

    // Connection to the MQTT server
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        String clientId = String("esiot-2122-client-") + String(random(0xffff), HEX);
        delay(3000);

        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(MQTT_INITIALIZATION_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
    connectionState = CONNECTED_TO_SERVER;
    
    // Exchanging initialization data
    client.subscribe(MQTT_INITIALIZATION_TOPIC);

    while (connectionState == CONNECTED_TO_SERVER) {
        delay(100);
        Serial.println("Waiting for initialization data");
        client.loop();
    }

    client.subscribe(water_topic.c_str());
}

// Callback function to handle messages received, currently only used for the protocol exchange
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print("Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        messageTemp += (char)payload[i];
    }
    Serial.println("Message arrived: " + messageTemp);

    // Check if the message is a JSON message and if it contains the expected fields
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, messageTemp);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    } else if (doc.containsKey("message_type") && doc.containsKey("f1") && doc.containsKey("f2") && doc.containsKey("warning_limit") && doc.containsKey("water_topic") && doc["message_type"] == "INITIALIZATION_DATA") {
        warning_limit = doc["warning_limit"].as<int>();
        f1 = doc["f1"].as<int>();
        f2 = doc["f2"].as<int>();
        water_topic = doc["water_topic"].as<String>();
        Serial.println("Initialization data received");
        connectionState = EXCHANGING_WATER_DATA;
    } else {
        Serial.println("Unricognized message type");
    }
}

void setup()	{
	Serial.begin(115200);
    delay(100);

	model.switchRedLed(true);
	model.switchGreenLed(false);
    
	randomSeed(micros());

	client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
}

void loop() {
    // Check if the client is connected to the MQTT server
	if (!client.connected()) {
		model.switchGreenLed(false);
		model.switchRedLed(true);
        
        setup_wifi();
		reconnect();
	}

    // Transmit water level data
	model.switchGreenLed(true);
	model.switchRedLed(false);

	unsigned short waterLevel = model.getWaterLevel();
    Serial.println(String("Water level: ") + waterLevel + String("warning limit: ") + warning_limit + String("f1: ") + f1 + String("f2: ") + f2);
	
    unsigned long now = millis();
    if (now - lastMsgTime > (waterLevel < warning_limit ? f1 : f2) && waterLevel < MAX_ERROR_DISTANCE) {
		lastMsgTime = now;

		snprintf (msg, MSG_BUFFER_SIZE, "{ \"water_level\": %ld }", waterLevel);

		Serial.println(String("Sending message: ") + msg);

		client.publish(water_topic.c_str(), msg);
	}

    client.loop();
}