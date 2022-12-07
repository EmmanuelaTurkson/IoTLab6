#include "DHT.h"
#include <EasyUltrasonic.h>
#include "PubSubClient.h" // Connect and publish to the MQTT broker

// Code for the ESP32
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)

#define DHTPIN 4  // Pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT11 or DHT22
#define TRIGPIN 22 // Digital pin connected to the trig pin of the ultrasonic sensor
#define ECHOPIN 23 // Digital pin connected to the echo pin of the ultrasonic sensor
#define MPIN 18


unsigned long previousMillisloop=0;

// different intervals for each LED
int intervalDistance = 2500;
int intervalTemp = 2000;
int intervalloop = 15000;

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  15        /* Time ESP32 will go to sleep (in seconds) */

DHT dht(DHTPIN, DHTTYPE);
EasyUltrasonic ultrasonic; // Create the ultrasonic object

// WiFi
const char* ssid = "Turkson";                 // Your personal network SSID
const char* wifi_password = "emmanuella"; // Your personal network password
const char* command1_topic="motor_command";

// MQTT
const char* mqtt_server = "192.168.137.52";  // IP of the MQTT broker
const char* distance_topic = "distance/2";
const char* temperature_topic = "temperature/2";
const char* mqtt_username = "nanaAElla"; // MQTT username
const char* mqtt_password = "nanaAElla"; // MQTT password
const char* clientID = "NEFA_client"; // MQTT client ID

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient);

// Custom function to connet to the MQTT broker via WiFi
void connect_MQTT() {

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
    client.subscribe(command1_topic);   // subscribe the topics here
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  ultrasonic.attach(TRIGPIN, ECHOPIN);
  pinMode(MPIN, OUTPUT);   

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("ESP32 slept for " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883 );//mqtt_port
  client.setCallback(callback);

}

void loop() {
  unsigned long currentMillis = millis();
  connect_MQTT();
  Serial.setTimeout(2000);

  float distanceIN = ultrasonic.getDistanceIN(); // Read the distance in inches
  float h= convertToCM(distanceIN); // Convert the distance to centimeters
  //float h = dht.readHumidity();
  float t = dht.readTemperature();
  String dataRead = "Node 1 published" +

  Serial.print("Distance: ");
  Serial.print(h);
  Serial.println(" cm");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  // MQTT can only transmit strings
  String hs = "Dis: " + String((float)h) + " cm ";
  String ts = "Temp: " + String((float)t) + " C ";

  if ((unsigned long)(currentMillis - previousMillisloop) >= intervalTemp) {
    // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
    if (client.publish(temperature_topic, String(t).c_str())) {
      Serial.println("Temperature sent!");
    }
    // Again, client.publish will return a boolean value depending on whether it succeded or not.
    // If the message failed to send, we will try again, as the connection may have broken.
    else {
      Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
      client.publish(temperature_topic, String(t).c_str());
    }
  }

  if ((unsigned long)(currentMillis - previousMillisloop) >= intervalDistance) {
    // PUBLISH to the MQTT Broker (topic = Humidity, defined at the beginning)
    if (client.publish(distance_topic, String(h).c_str())) {
      Serial.println("Distance sent!");
    }
    // Again, client.publish will return a boolean value depending on whether it succeded or not.
    // If the message failed to send, we will try again, as the connection may have broken.
    else {
      Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
      client.publish(distance_topic, String(h).c_str());
    }
  }

  client.disconnect();  // disconnect from the MQTT broker
  delay(1000 * 10);     // print new values every 1 Minute

  // time to read temperature and humidity values
  if ((unsigned long)(currentMillis - previousMillisloop) >= intervalloop) {
    Serial.println("Going to sleep now");
    Serial.flush(); 
    esp_deep_sleep_start();
    previousMillisloop = currentMillis;
    
  }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  
  //--- check the incomming message
    if( strcmp(topic,command1_topic) == 0){
     if (incommingMessage.equals("on")){
      digitalWrite(MPIN, LOW);   // Turn the LED on 
      Serial.println("Received on");
     }
     else{
      digitalWrite(MPIN, HIGH);  // Turn the LED off 
      Serial.println("Received off");
     }
  }

}
