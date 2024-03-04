#include <WiFi.h>
#include <PubSubClient.h>

TaskHandle_t communication_task, data_task;

const char* ssid = "Antonelli";
const char* password = "b8R8!(z534NK}o~";
const char* mqtt_server = "192.168.1.53";
const int millis_data_frequency = 500;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(5000);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, subscribe to desired topics
      client.subscribe("esp8266/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void communication_task( void * parameter ){
  for(;;){

  }
}

void data_task( void * parameter ){
  for(;;){
    
    delay(millis_data_frequency);
  }
}
