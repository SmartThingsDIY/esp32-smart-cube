# Smart Desktop Cube DIY with ESP32

This DIY smart cube sits on your desk and provides ability to automate everything around you via quick shortcuts.

⚡️ COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/esp-32.jpg?raw=true" style="max-width:100%;" height="200">

*   [ESP32 Board](https://amzn.to/3jmCpqx)
*   [MPU-6050](https://www.amazon.ca/gp/product/B07JPK26X2) 3 Axis Accelerometer Gyroscope
*   [SW-420](https://www.amazon.ca/gp/product/B01FDGTTY8) Vibration Sensor
*   [5 LiPo Batteries And Charger](https://www.amazon.ca/gp/product/B0795F139D)
*   [Breadboard](https://amzn.to/2Ei40tP)
*   [Solder Kit](https://www.amazon.ca/-/fr/gp/product/B01N46T138/)
*   [Helping Hands for soldering](https://www.amazon.ca/gp/product/B002PIA6Z4)
*   [Breadboard](https://amzn.to/2Ei40tP) - [Jumper Wires](https://amzn.to/2Ehh2ru) - [Male to Male Jumper Wires + Tweezer](https://amzn.to/3jcf9eX)

🚀APPS
------

*   [VSCode](https://code.visualstudio.com/)
*   [Fritzing](https://fritzing.org/)
*   [PlatformIO](https://platformio.org/)

📦 Libraries
=====
*   [Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050)
*   [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)
*   [Wire](https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire)
*   [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)

Hardware Overview: MPU-6050 3 Axis Accelerometer Gyroscope
----------------------

<img align="center" src="https://github.com/MecaHumArduino/esp32-smart-cube/blob/main/doc/img/mpu6050.jpeg?raw=true">

The MPU-6050 is a serious little piece of motion processing tech! By combining both a 3-Axis accelerometer and a 3-Axis gyroscope on a single chip, the MPU-6050 is capable of processing complex 9-axis MotionFusion algorithms, the MPU-6050 does away with the cross-axis alignment problems that can creep up on discrete parts.

Hardware Overview: SW-420 Vibration Sensor
----------------------

<img align="right" src="https://github.com/MecaHumArduino/esp32-smart-cube/blob/main/doc/img/SW-420-sensor.jpeg?raw=true" style="max-width:100%;" height="300">



Schematic Diagram
-----------------

Wire the [DHT11](https://amzn.to/2Qs9fcV) or [DHT22](https://amzn.to/31t7P8l) sensor to the [ESP32](https://amzn.to/3jmCpqx) development board as shown in the following schematic diagram.

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/schematic.png?raw=true" style="max-width:100%;" height="600">

Installing Libraries
--------------------

To read from the DHT sensor, we'll use the [DHT library from Adafruit](https://github.com/adafruit/DHT-sensor-library). To use this library you also need to install the [Adafruit Unified Sensor library](https://github.com/adafruit/Adafruit_Sensor). Follow the next steps to install those libraries.

Open your Arduino IDE and go to **Sketch > Include Library > Manage Libraries**. The Library Manager should open.

Search for **"DHT"** on the Search box and install the DHT library from Adafruit.

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/adafruit_dht_library.png?raw=true" style="max-width:100%;" height="600">

After installing the DHT library from Adafruit, type **"Adafruit Unified Sensor"** in the search box. Scroll all the way down to find the library and install it.

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/adafruit_unified_sensor_library.png?raw=true" style="max-width:100%;" height="600">

After installing the libraries, restart your Arduino IDE.

THE CODE
--------

Rename the file [secret.h.public](https://github.com/isbkch/esp32-aws-weather-station/blob/master/secrets.h.public) to **secret.h** and edit it with your information:

*   1\. Select your thing from [this page](https://console.aws.amazon.com/iot/home?region=us-east-1#/thinghub)
*   2\. From the left menu, select **Security** and click the button "**Create Certificate**"
*   3\. Now download both files *.cert.pem and *.private.key and replace their content in the **secret.h**. And don't forget to activate your Certificate.
*   4\. Back to the left menu, click **Interact** and copy the HTTPS link then paste it into **secret.h** as well.

Finally, open the file [esp32-aws-weather-station.ino](https://github.com/isbkch/esp32-aws-weather-station/blob/master/esp32-aws-weather-station.ino) with Arduino IDE

PART TWO
========

AWS SETUP
---------

Now that parts are connected and the libraries are installed, it's time to setup the AWS part.

As you see in the diagram below, the ESP board will be sending data through an MQTT topic to AWS IoT Core, we will be applying rules on the raw data and invoking a Lambda that writes the data to a dynamoDB table. Afterwards, we can read that data and display it on a webpage

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/aws-architecture.png?raw=true" style="max-width:100%;" height="350">
