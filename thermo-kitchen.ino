#include <Adafruit_MQTT.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>

const int ESP8266_LED = 16;
const int DSPIN = 5;
const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
const int RELAYA = 2;
const int RELAYB = 14;

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const char* mqtt_id = "";

unsigned long nowtime = millis();

OneWire ds(DSPIN);
DallasTemperature sensors(&ds);

DeviceAddress insideThermometer = { 0x28, 0xFF, 0xED, 0x80, 0x74, 0x16, 0x04, 0xF1 };
DeviceAddress outsideThermometer = { 0x28, 0xFF, 0xCB, 0xE8, 0x61, 0x16, 0x03, 0xE5 };

void callback(char* topic, byte* payload, unsigned int length) {

  String strTopic = String((char*) topic);

  Serial.println(strTopic);

  if (strTopic == "kitchen/cupboard") {
    Serial.println("cupboard message");
    if ((char)payload[0] == '1') {
      Serial.println("on");
      digitalWrite (RELAYA, LOW);
    } else {
      Serial.println("off");
      digitalWrite (RELAYA, HIGH);
    }
  }

  if (strTopic == "kitchen/worktop") {
    Serial.println("worktop message");
    if ((char)payload[0] == '1') {
      Serial.println("on");
      digitalWrite (RELAYB, LOW);
    } else {
      Serial.println("off");
      digitalWrite (RELAYB, HIGH);
    }
  }

}

WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, callback, espClient);

void setup() 
{
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  pinMode(LDR, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RELAYA, OUTPUT);
  pinMode(RELAYB, OUTPUT);

  digitalWrite(RELAYA, HIGH);
  digitalWrite(RELAYB, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  sensors.begin();

  // insideThermometer = { 0x28, 0xFF, 0xED, 0x80, 0x74, 0x16, 0x04, 0xF1 };
  // outsideThermometer = { 0x28, 0xFF, 0xCB, 0xE8, 0x61, 0x16, 0x03, 0xE5 };

  sensors.setResolution(insideThermometer, 9);
  sensors.setResolution(outsideThermometer, 9);
}

void reconnect() {

  while (!client.connected()) {
    if (client.connect(mqtt_id, mqtt_user, mqtt_pass)) {
      client.subscribe ("kitchen/cupboard");
      client.subscribe ("kitchen/worktop");
    } else {
      delay (5000);
    }
  }
}

void loop() 
{
  if (!client.connected()) {
    analogWrite (RED, 800);
    reconnect();
    analogWrite (RED, 0);
  }

  client.loop();

  if (millis() - nowtime > 10000) {
  
    sensors.requestTemperatures();
    String inC = String(sensors.getTempC(insideThermometer), 1);
    String outC = String(sensors.getTempC(outsideThermometer), 1);
    String inLight = String(analogRead(LDR), DEC);
  
    Serial.print("/temp/kitchen = ");
    Serial.print(inC);
    Serial.println();
    Serial.print("/temp/outside = ");
    Serial.print(outC);
    Serial.println();
    Serial.print("/ldr/kitchen = ");
    Serial.print(inLight);
    Serial.println();

    analogWrite(BLUE, 600);
  
    client.publish((char*) "thermometer/kitchen", (char*) inC.c_str() );
    client.publish((char*) "thermometer/outside", (char*) outC.c_str() );
    client.publish((char*) "ldr/kitchen", (char*) inLight.c_str() );

    analogWrite(BLUE, 0);
  
    nowtime = millis();
  }
    
}
