#include <Arduino.h>
#include "secrets.h"
#include <PubSubClient.h>
#include "WiFi.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define DEBUG true       // switch to "false" for production
#define NB_TRYWIFI 20    // WiFi connection retries
#define durationSleep 30 // seconds

float GyroX, GyroY, GyroZ;
Adafruit_MPU6050 mpu;

WiFiClient espClient;
PubSubClient client(espClient);

// **************
void loop();
void setup();
void doAction();
void goToSleep();
void connectToWiFi();
void connectToHass();
void initGyroComponent();
void printWakeupReason();
// void publishAlarmToHass(String msg);
// **************

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

            ESP.deepSleep(durationSleep * 1000000);
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
        // if (client.connect("ESP32Client")) {
        if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD))
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

/**
 * Publishes notification to MQTT topic
 * @return
 */
void publishAlarmToHass(String msg)
{
    // publish the reading to Hass through MQTT
    client.publish(MQTT_PUBLISH_TOPIC, msg.c_str(), true);
    client.loop();
    if (DEBUG == true)
    {
        Serial.println("Alarm sent to Hass!");
    }
}

void setup(void)
{
    Serial.begin(115200);

    // only print debug messages to serial if we're in debug mode
    if (DEBUG == true)
    {
        Serial.println("Waking up ");
        printWakeupReason(); // Print the wakeup reason for ESP32
    }

    while (!Serial)
    {
        delay(10); // will pause Zero, Leonardo, etc until serial console opens
    }

    initGyroComponent();

    // connectToWiFi();
    // connectToHass();
    // publishAlarmToHass("do this");

    /* wait for the cube to be put down */
    delay(2000);
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    GyroX = g.gyro.x;
    GyroY = g.gyro.y;
    GyroZ = g.gyro.z;
    if (DEBUG == true)
    {
        Serial.println("");
        Serial.print("Rotation X: ");
        Serial.print(GyroX);

        Serial.print(", Y: ");
        Serial.print(GyroY);

        Serial.print(", Z: ");
        Serial.print(g.gyro.z);

        Serial.println(" rad/s");
        Serial.println("");
    }

    // doAction();
    delay(5000); // stay awake for 5 seconds
    goToSleep();
}

void doAction()
{
    // ...
}

void goToSleep()
{
    if (DEBUG == true)
    {
        Serial.println("Going to sleep");
    }

    // Configure GPIO33 as ext0 wake up source for HIGH logic level
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);

    // Go to sleep now
    esp_deep_sleep_start();
}
/**
 * Function that prints the reason by which ESP32 has been awaken from sleep
 *
**/
void printWakeupReason()
{
    esp_sleep_wakeup_cause_t wakeupReason;
    wakeupReason = esp_sleep_get_wakeup_cause();

    switch (wakeupReason)
    {
    case 1:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case 2:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case 3:
        Serial.println("Wakeup caused by timer");
        break;
    case 4:
        Serial.println("Wakeup caused by touchpad");
        break;
    case 5:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.println("Wakeup was not caused by deep sleep");
        break;
    }
}

void initGyroComponent()
{
    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }

    Serial.println("");
    Serial.println("MPU6050 Found!");
    Serial.println("");

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    delay(100);
}

void loop() {}
