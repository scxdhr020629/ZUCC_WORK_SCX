#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

#define WLAN_SSID "scxwifi"
#define WLAN_PASS "cxcx629326"

#define AIO_SERVER "121.5.180.148"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME ""
#define AIO_KEY ""
#include <ArduinoJson.h>
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 2, 14);
DHT dht(5, DHT11);



WiFiClient client;


StaticJsonDocument<200> doc;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "espSCX");


Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "getSCX/1019");


void MQTT_connect();


// rgb µÆ 
const int pin_r = 12;
const int pin_g = 13;
const int pin_b = 15;


void rgbSetup()
{
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);
}


void drawRgb(int r, int g, int b)
{
  analogWrite(pin_r, r);
  analogWrite(pin_g, g);
  analogWrite(pin_b, b);
}

void setup()
{

  rgbSetup();
  Serial.begin(115200);
  delay(10);
  display.init();

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}

int strSame(char *str, char *str2)
{
  return strcmp(str, str2) == 0;
}
#include <string.h>
uint32_t x = 0;
void loop()
{

  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &onoffbutton)
    {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);

      char *read = (char *)onoffbutton.lastread;

      display.flipScreenVertically();
      display.clear();
      display.drawString(0, 10, "topic: espSCX");
      display.drawString(0, 20, (char *)onoffbutton.lastread);
      // display.display();

      StaticJsonDocument<200> ledJson;
      //   deserializeJson(ledJson,(char*)onoffbutton.lastread);
      deserializeJson(ledJson, read);
      drawRgb(ledJson["r"], ledJson["g"], ledJson["b"]);

      delay(2000);
    }
  }
  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");

  String output;

  doc["hum"] = (float)dht.readHumidity();
  doc["temp"] = (float)dht.readTemperature();
  serializeJson(doc, output);

  if (!photocell.publish(output.c_str()))
  {
    Serial.println(F("Failed"));
  }
  else
  {
    Serial.println(F("OK!"));

    display.flipScreenVertically();
    display.clear();
    display.drawString(0, 10, "topic: iot/1");
    display.drawString(0, 20, String(x));
    display.display();
    delay(2000);
  }


}


void MQTT_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}
