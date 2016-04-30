/* Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <string.h>


/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "TARAL SHAH1"
#define WLAN_PASS       "taralshah123"
const int PIR1 = D1;
const int PIR2 = D2;
const int REED = D3;
const int LIGHT = D4;
const int FAN = D5;
const int LOCK = D6;
const int ULTRA = D7;
float val = 0;
float voltage;
//const float voltage;
int flag = 0;
int flag1 = 0;
/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.1.102"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    ""
#define AIO_KEY         ""

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ AIO_USERNAME;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;
char Str1[15];

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED[] PROGMEM = AIO_USERNAME "topic";
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED);

// Setup a feed called 'topic1' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED1[] PROGMEM = AIO_USERNAME "topic1";
Adafruit_MQTT_Publish photocell1 = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED1);

// Setup a feed called 'topic1' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED2[] PROGMEM = AIO_USERNAME "count";
Adafruit_MQTT_Publish photocell2 = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED2);

// Setup a feed called 'topic2' for publishing. REED
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED3[] PROGMEM = AIO_USERNAME "topic2";
Adafruit_MQTT_Publish photocell3 = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED3);


// Setup a feed called 'onoff' for subscribing to changes.
const char ONOFF_FEED[] PROGMEM = AIO_USERNAME "/feeds/onoff";
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, ONOFF_FEED);

/*************************** Sketch Code ************************************/

void setup() {
  flag = 1;
  flag1 = 1;
  Serial.begin(115200);
  delay(10);
  pinMode(PIR1, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(REED, INPUT);
  pinMode(ULTRA, INPUT);
   
  pinMode(LIGHT, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(LOCK, OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // Try to ping the MQTT server
  /*
  if (! mqtt.ping(3) ) {
    // MQTT pings failed, lets reconnect
    Serial.println("Ping fail!");
  }
  */

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      if(strcmp((char *)onoffbutton.lastread, "LED40") == 0)
      {
        digitalWrite(LIGHT, LOW);
        Serial.println("LED4 OFF");
      }
      else if(strcmp((char *)onoffbutton.lastread, "LED41") == 0)
      {
        digitalWrite(LIGHT, HIGH);
        Serial.println("LED4 ON");
      }
      else if(strcmp((char *)onoffbutton.lastread, "LED50") == 0)
      {
        digitalWrite(FAN, LOW);
        Serial.println("LED5 OFF");
      }
      else if(strcmp((char *)onoffbutton.lastread, "LED51") == 0)
      {
        digitalWrite(FAN, HIGH);
        Serial.println("LED5 ON");
      }
      else if(strcmp((char *)onoffbutton.lastread, "LED60") == 0)
      {
        digitalWrite(LOCK, LOW);
        Serial.println("LED6 OFF");
      }
      else if(strcmp((char *)onoffbutton.lastread, "LED61") == 0)
      {
      digitalWrite(LOCK, HIGH);
      Serial.println("LED6 ON");
      }
      else if(strcmp((char *)onoffbutton.lastread, "false") == 0)
      {
      flag = 0;
      }
      else if(strcmp((char *)onoffbutton.lastread, "true") == 0)
      {
      flag = 1;
      }
      else if(strcmp((char *)onoffbutton.lastread, "false1") == 0)
      {
      flag1 = 0;
      }
      else if(strcmp((char *)onoffbutton.lastread, "true1") == 0)
      {
      flag1 = 1;
      }
    }
  }

  // Now we can publish stuff!
  if(flag)
  {
//  Serial.print(F("\nSending photocell val "));
//  Serial.print(x);
//  Serial.print("...");
//  if (! photocell.publish(x++)) {
//    Serial.println(F("Failed"));
//  }
    if(digitalRead(PIR1) == HIGH)
      {
      Serial.println(F("OK!"));
      photocell.publish("Touch");
      delay(15000);
      }
      else
      {
      Serial.println(F("NOT OK!"));
      photocell.publish("No_touch");
      }
    val = analogRead(A0);
    voltage = ((val*20)/1024);
    Serial.println((voltage));
    photocell1.publish(voltage);        
   if(voltage < 15 && digitalRead(PIR1) == HIGH)
      {
       digitalWrite(LIGHT,HIGH);
      }
      else
      {
      digitalWrite(LIGHT,LOW);
      }
  }
  if(flag1)
  {
//  Serial.print(F("\nSending photocell val "));
//  Serial.print(x);
//  Serial.print("...");
//  if (! photocell.publish(x++)) {
//    Serial.println(F("Failed"));
//  }
    if( digitalRead(ULTRA) == LOW)
      {
      //Serial.println(F("OK1!"));
      //photocell1.publish("ON");

      }
      else
      {
      //Serial.println(F("NOT OK1!"));
      //photocell1.publish("OFF");

       }
       if( digitalRead(REED) == HIGH)
      {
      //Serial.println(F("OK3!"));
      photocell3.publish("OPEN");

      }
      else
      {
      //Serial.println(F("NOT OK3!"));
      photocell3.publish("CLOSE");
      }
  }
  delay(350);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

