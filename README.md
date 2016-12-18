# thermo-kitchen
ESP8266 Arduino sketch to report two DS18B20 thermometers and also control two relays with MQTT

The main requirement for this application was to report the current thermometer readings to MQTT topics and also to listen to two MQTT topics to control an attached two channel relay board.

This sketch is running on a ESP 8266 12F, also known as a 'wispy' its a small square board with built in LDR, pushbutton and also a RGB LED, so some of the gpio pins are already taken up by the attached accessories.  The board comes with a plugin daughter board which enables programming.

The sketch can easily run on alternatives just by adjusting the gpio pins to whatever you have available. The NodeMCU board is also a favorite of mine.

At the top of the sketch you'll have to populate the wifi settings and also your mqtt authentication variables.

This version uses two hard coded thermometer sensors and their addresses are hard coded.  I have another sketch thermo-remote which uses one sensor and automatically gets the sensor address.

Hard coded lower down in this initial upload are the topic to publish and subscribe to, you'll have to adjust those to your needs.

To control the relays, publish a 1 to the topic, and anything else will turn the relay off.
