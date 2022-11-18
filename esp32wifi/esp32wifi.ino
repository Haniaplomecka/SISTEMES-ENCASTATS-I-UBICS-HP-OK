#include <Arduino.h>
#include <WiFi.h>
#include <WiFiSTA.h>
#include <PubSubClient.h>

const char* ssid = "red";
const char* password = "f575edad";
const char* mqtt_server = "192.168.150.102";

const char* clientID = "ESP-32";
const char* clientUserName = "ESP-32";
const char* clientPassword = "ESP-32";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastmsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect(clientID, clientUserName, clientPassword);
  client.subscribe("broker/counter");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  client.loop();
  delay(10);
}
