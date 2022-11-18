#include <Wire.h>

#include "heartRate.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include <MAX30105.h>

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

const char* ssid="red";
const char* password= "f575edad";
const char* mqtt_server="192.168.150.102";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg=0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value=0;

void setup_wifi(){
  delay(10);

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status()!= WL_CONNECTED){
    delay(500);
    //Serial.print(".");
  }

  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  //Serial.print ("Message arrived[");
  //Serial.print(topic);
  //Serial.print(" ] ");
  for(int i=0; i<length; i++){
    //Serial.print((char)payload[i]);
  }
  //Serial.println("]");
}

void reconnect(){
  while(!client.connected()){
    String clientId="";
    clientId+= String(random(0xffff),HEX);
    if(client.connect("ESP-01 Heartrate")){
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");

    }else{
      delay(5000);
    }

  }
}


void setup()
{
  //Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);

}

void loop()
{
  
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  unsigned long now =millis();
  if(now-lastMsg>2000){
    lastMsg=now;
    ++value;
    snprintf(msg,MSG_BUFFER_SIZE,"HELLO WORLD #%ld", value);
    client.publish("broker/counter","msg");
  }

}
