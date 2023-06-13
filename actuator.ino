

#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#define WIFI_TIMEOUT_MS 20000
#define WIFI_NETWORK "Wifi Name"
#define WIFI_PASSWORD "Wifi Pass"
#include "ThingSpeak.h"
#include "Servo.h"
Servo myservo;
const char* ssid = "Divu";
const char* password = "Divyaraj@1234";
unsigned long channelID= 2165461;
char* readAPIkey="IK3JTEU3UIIKT7N3";
unsigned int dataFieldOne=1;
unsigned int dataFieldTwo=2;
unsigned int dataFieldThree=3;
unsigned int dataFieldFour=4;

const char* CurrentRoom;
const char* Status;
const char* Door;
unsigned long channelID2= 2165472;
char* readAPIkey2="N62FEX47OUTCEIL4";

unsigned long channelID3= 2165460;
char* readAPIkey3="RKJA7UOQVKL4SSU9";
char* writeAPIkey3="UGEDUGRFKOC59M9R";


const char* mqttServer = "mqtt.thingspeak.com";
const int mqttPort = 1883;
const char* mqttUsername = "FCoPExU9MAUQISkGCBEDCz0";
const char* mqttPassword = "OwEpA4RuSjFFfZNzg6fGoBXa";
// const char* mqttTopic = "channels/2165461/publish/fields/field2/GFLOH9S1XRN3NPPS";

const int ledPin1 = 32;
const int ledPin2= 26;
const int ledPin3= 27;
const int ledPin4= 25;
const int buzzerPin=14;
// const int ledPin2= 18;

char currentdoor='0';

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectToWiFi()
{
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);


unsigned long startAttemptTime = millis(); 

while(WiFi.status() != WL_CONNECTED && millis()-startAttemptTime < WIFI_TIMEOUT_MS){
  Serial.print(".");
  delay(1000);
}
if(WiFi.status() != WL_CONNECTED){
  Serial.println(" Failed!");

}


else
{
  Serial.print(" Connected!");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(wifiClient);
  // mqttClient.setServer(mqttServer, mqttPort);
  // mqttClient.setCallback(mqttCallback);
}

}

void setup() {
  // myservo.attach(5);
  myservo.attach(33);
  myservo.write(150);
  Serial.begin(9600);
  connectToWiFi();
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin4,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(buzzerPin,OUTPUT);
  digitalWrite(buzzerPin,HIGH);
  // ThingSpeak.begin(wifiClient);
  // int data1=readTSData(channelID,dataFieldOne);
  // int data2=readTSData(channelID,dataFieldTwo);
  // int data3=readTSData(channelID,dataFieldThree);
  // int data4=readTSData(channelID,dataFieldFour);
  // Serial.println(data1);
  // Serial.println(data2);
  // Serial.println(data3);
  // Serial.println(data4);

  // pinMode(buzzerPin,OUTPUT);

}

void loop() {

   HTTPClient http;

    http.begin("http://192.168.66.102:8000/endpoint");
    int httpCode = http.GET();                                        

    if (httpCode > 0) {

        String payload = http.getString();
        Serial.println(httpCode);
        JSONVar my_obj = JSON.parse(payload);
        CurrentRoom = my_obj["data"];
        Status = my_obj["status"];
        Door = my_obj["door"];
        Serial.print("Current Room: ");
        Serial.println(CurrentRoom);
        
        Serial.print("LED Status: ");
        Serial.println(Status);

        Serial.print("Door Status: ");
        Serial.println(Door);     
      }

    else {
      Serial.println(httpCode);
      Serial.println("Error on HTTP request");
    }

    http.end();    

    if(CurrentRoom[0]=='1' && Status[0]=='1')
    {
      digitalWrite(ledPin1,HIGH);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPin3,LOW);
      digitalWrite(ledPin4,LOW);
    }
    else if(CurrentRoom[0]=='2'&& Status[0]=='1')
    {
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,HIGH);
      digitalWrite(ledPin3,LOW);
      digitalWrite(ledPin4,LOW);
    }
    else if(CurrentRoom[0]=='3' && Status[0]=='1')
    {
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPin3,HIGH);
      digitalWrite(ledPin4,LOW);
    }
    else if(CurrentRoom[0]=='4'&& Status[0]=='1')
    {
      // Serial.println("Boobieboy mc");
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPin3,LOW);
      digitalWrite(ledPin4,HIGH);
    }
    else{
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPin3,LOW);
      digitalWrite(ledPin4,LOW);
    }

    Serial.print("Current door status is ");
    Serial.println(Door);

    if(Door[0]=='1')
    {
      myservo.write(150);
      delay(1500);
      Serial.println("Door Opened");
      currentdoor=Door[0];

      
    }
    else if(Door[0]=='0')
    {
      myservo.write(70);
      delay(1500);
      Serial.println("Door Closed");
      currentdoor=Door[0];
    }
    // else{
    //   if(currentdoor=='1'){
    //     myservo.write(180);
    //   delay(1500);
    //   }
    //   else{
    //     myservo.write(60);
    //   delay(1500);
    //   }
    // }
  // Serial.println("Fetching position Data:");
  // int data1=readTSData(channelID,dataFieldOne);
  // int data2=readTSData(channelID,dataFieldTwo);
  // int data3=readTSData(channelID,dataFieldThree);
  // int data4=readTSData(channelID,dataFieldFour);

  //   Serial.println(data1);
  //   Serial.println(data2);
  //   Serial.println(data3);
  //   Serial.println(data4);

  //   if(data1==1)
  //   {digitalWrite(ledPin1,HIGH);}
  //   else
  //   {
  //     digitalWrite(ledPin1,LOW);
  //   }

  //   if(data2==1)
  //   {
  //   digitalWrite(ledPin2,HIGH);
  //   }
  //  else
  //   {
  //     digitalWrite(ledPin2,LOW);
  //   }
  //   if(data3==1)
  //   {
  //   digitalWrite(ledPin3,HIGH);
  //   }
  //    else
  //   {
  //     digitalWrite(ledPin3,LOW);
  //   }
  //   if(data4==1)
  //   {
  //   digitalWrite(ledPin4,HIGH);
  //   }
  //    else
  //   {
  //     digitalWrite(ledPin4,LOW);
  //   }
      
    Serial.println("Fetching buzzer Data:");
   int data=readTSData2(channelID2,1);
    Serial.println(data);
    if(data==1)
    digitalWrite(buzzerPin,LOW);
    else
    {
      // Serial.println("madar");
    digitalWrite(buzzerPin,HIGH);
    }
//     Serial.println("Fetching door Data:");
//   data=readTSData3(channelID3,1);
//     Serial.println(data);
// //  ThingSpeak.writeField( channelID3 ,1, 1,writeAPIkey3);
//     // delay(16000);
//     // int dataashwin=1;
}

int readTSData(long TSChannel, unsigned int TSField)
{
  int data=ThingSpeak.readIntField(TSChannel,TSField,readAPIkey);
  Serial.println("Data read:"+ String(data,9));
  return data;
}
int readTSData2(long TSChannel, unsigned int TSField)
{
  int data=ThingSpeak.readIntField(TSChannel,TSField,readAPIkey2);
  Serial.println("Data read:"+ String(data,9));
  return data;
}

int readTSData3(long TSChannel, unsigned int TSField)
{
  int data=ThingSpeak.readIntField(TSChannel,TSField,readAPIkey3);
  Serial.println("Data read:"+ String(data,9));
  return data;
}
