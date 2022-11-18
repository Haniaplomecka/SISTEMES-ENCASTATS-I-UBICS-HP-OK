/*
 * MASTER CONFIG
 */

#include "Wire.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"

#define SLAVE_ADDR 8
const char* ssid="red";
const char* password= "f575edad";
const char* mqtt_server="192.168.76.102";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg=0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
const char* label = "";

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
  // for(int i=0; i<length; i++){
  //   Serial.print((char)payload[i]);
  // }
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(2,0);
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

    delay(2000);
    Wire.requestFrom(SLAVE_ADDR,10);
    unsigned long now =millis();
    if(now-lastMsg>2000 and Wire.available()){
      lastMsg=now;
      //snprintf(msg,MSG_BUFFER_SIZE,"HELLO WORLD #%ld", value);
      Wire.readBytes(msg, Wire.available());
      client.publish("broker/NanoBLE", msg);
    }
}
