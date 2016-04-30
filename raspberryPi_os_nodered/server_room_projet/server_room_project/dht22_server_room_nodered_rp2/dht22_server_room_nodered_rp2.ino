/* 19/10/15
 * Final working for MQTT+IR+PIR+Ultrasonic
 * AP config to be added later 
  IR1
  /* flag name: tval = temperature value flag
   *  mail_flag = flag to send mail 
   *  flag2 = humidity
   *  
 */
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
#include "DHT.h"
#define DHTPIN D2   
#define DHTTYPE DHT22  // DHT 22  (AM2302)
// Update these with values suitable for your network.

//const char* ssid = "Mi4";
//const char* password = "1234567890";
//const char* mqtt_server = "192.168.43.214";
const char* ssid = "TARAL SHAH1";
const char* password = "taralshah123";
const char* mqtt_server = "192.168.1.109";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
unsigned int flag2 = 0;
int value = 0;
int Pin = 14; //D5 IR
int indicator = 2; //D4 LED
int Pin1 = 12 ;// D6 IR
//Ultrasonic
const int trigPin = 4;  // D2
const int echoPin = 5;  // D1
unsigned int tval = 0;
int flag = 0;
int flag1 = 0;
int mail_flag = 0;
int j = 0;
int loopcount = 0; // it will check after  every 1 min
void setd();
void humidity();
// defines variables
long duration;
int distance;
int fdistance;
int dis[5];
int i = 0;
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(Pin, INPUT);
  pinMode(Pin1,INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(indicator, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  digitalWrite(indicator, HIGH);
  Serial.print("Distance: ");
  Serial.print("setd ");
  Serial.println("DHTxx test!");
   
   dht.begin();
//  setd();

//  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
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
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
  //  Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
   //   Serial.println("connected");
      // Once connected, publish an announcement...
//      client.publish("topic", "hello world");
      // ... and resubscribe
      client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2) {
 lastMsg = now;
  if(flag2)
  {
      if(tval)
      {
       if(!mail_flag)
        {
          client.publish("temperature_critical", "temp");
          mail_flag = 1;
          digitalWrite(indicator, LOW);
        }
      }
      else
      {
      client.publish("temperature_critical", "temp1");
      mail_flag = 0;
      digitalWrite(indicator, HIGH);
      }
      Serial.println("Published"); 
      flag2 = 0;     
  }
//   if( digitalRead(Pin1) == HIGH ) // IR
// {
//      client.publish("topic", "IR Sensor2");
//     Serial.println("IR sensor2"); 
//     digitalWrite(indicator, LOW);
//       delay(3200);
//     digitalWrite(indicator, HIGH);
//  }
  
    loopcount++;
    if (loopcount >= 10)
    {
      Serial.print("ok");
      delay(5000);
    humidity();
    loopcount = 0;
    flag2 = 1;
    }
  }
}

void humidity()
{
   delay(2000);
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

    if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

      // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  if(t > 28)
  tval = 1;
  else
  tval = 0; 
  snprintf (msg, 75, "%ld",t);
//  client.publish("topic", msg);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
 
 }


