/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdlib.h>

// Update these with values suitable for your network.

const char* ssid = "LocalSSID";
const char* password = "LocalPW";
const char* mqtt_server = "VPS_IP";
const int sensor_pin = A0;

WiFiClient espClient;
PubSubClient client(espClient);
// long lastMsg = 0;
// char msg[50];
// int value = 0;
float sensorReading = 0.0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to Wifi - ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //   Serial.println(WiFi.status());   // Checks Wifi.status value for testing
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting remote MQTT connection...");   // Connect to remote network
 
      // MQTT Connection Test Section
   if (client.connect("ESP8266Client")) {
      Serial.println("Connected to remote network ");
   }
    
    
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
     // setup_wifi();
    }

 /*     if (WiFi.status() = 6  {     // 6 = connected
          Serial.println("Connected to remote network ");
      }

      else {
            setup_wifi();
            Serial.println("Connecting to remote network "); 
      }
 */
  }

      Sensor();
}

void Sensor()  {
// Sensor Section  (Setup here for moisture sensor)
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  sensorReading = ( 100 - ( (sensor_analog/1023.00) * 100 ) )/10;
  Serial.print("Published outTopic - "); 
  Serial.print(sensorReading);
  Serial.println(" %");
  Serial.println("");
  delay(5000); // delay 5 seconds before next write  
  

 // It is necessary to convert reading to a 1 point precision string in order to publish.
  char MS_String[20];
  dtostrf(sensorReading, 1, 1, MS_String);
  client.publish("outTopic", MS_String);
     
   }


void loop() {

  if (!client.connected()) {
    reconnect();
  }
// client.loop();
}
