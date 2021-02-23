/**
  Wakes up every 30sec to read data from the water sensor, when water is
  detected, it connects to WiFi, then sends an alarm to an MQTT topic
  @author MecaHumArduino
  @version 1.0
*/

#include <Arduino.h>
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEBUG true // switch to "false" for production
WiFiClient espClient;
PubSubClient client(espClient);

#define NB_TRYWIFI 20 // WiFi connection retries

// **************
void loop();
void setup();
void connectToHass();
void connectToWiFi();
// **************

void tick()
{
    // toggle state
    int state = digitalRead(LED_BUILTIN); // get the current state of GPIO1 pin
    digitalWrite(LED_BUILTIN, !state);    // set pin to the opposite state
}

/**
 * Establishes WiFi connection
 * @return
 */
void connectToWiFi()
{
    int _try = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (DEBUG == true)
    {
        Serial.println("Connecting to Wi-Fi");
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        _try++;
        if (_try >= NB_TRYWIFI)
        {
            if (DEBUG == true)
            {
                Serial.println("Impossible to connect WiFi, going to deep sleep");
            }
        }
    }
    if (DEBUG == true)
    {
        Serial.println("Connected to Wi-Fi");
    }
}

/**
 * Establishes connection to Home Assistant MQTT Broker
 * @return
 */
void connectToHass()
{
    client.setServer(MQTT_SERVER, 1883);

    // Loop until we're reconnected
    while (!client.connected())
    {
        if (DEBUG == true)
        {
            Serial.print("Attempting MQTT connection...");
        }
        // Attempt to connect
        // If you do not want to use a username and password, change next line to
        // if (client.connect("ESP8266Client")) {
        if (client.connect("ESP8266Client", MQTT_USER, MQTT_PASSWORD))
        {
            if (DEBUG == true)
            {
                Serial.println("connected");
            }
        }
        else
        {
            if (DEBUG == true)
            {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
            }
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}


void setup()
{
    // only print debug messages to serial if we're in debug mode
    if (DEBUG == true)
    {
        Serial.print("Waking up ");
    }

    Serial.begin(115200);

}

void loop()
{
}
