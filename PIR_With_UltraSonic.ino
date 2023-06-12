#include <HTTPClient.h>
#include <WiFi.h>
#include <stdlib.h>
#include "ThingSpeak.h"
#define CHANNEL_ID 2165461
#define API_KEY "CQA08TED4R0B70XX"
char ssid[] = "Arya Topale";
char password[] = "dp12wifi";


#define CSE_IP "192.168.66.102" //CHANGE THIS IP ADDRESS FOR EACH COMPUTER "ifconfig command"
#define CSE_PORT 5089
#define OM2M_ORGIN "admin:admin"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "AE-TEST/User-Patterns"
#define OM2M_DATA_CONT "What_Room_Is_Grandma_In"


const int PirPin1 = 4;
const int PirPin2 = 13;    
const int PirPin3 = 14;
const int PirPin4 = 16;
// const int PirPin5 = 17;

const int trigPin1 = 18;
const int echoPin1 = 19;

const int trigPin2 = 21;
const int echoPin2 = 22;

const int trigPin3 = 23;
const int echoPin3 = 25;

const int trigPin4 = 26;
const int echoPin4 = 27;

long duration;
long distance;
long threshold = 7;

HTTPClient http;
WiFiClient client;
int US1;
int US2;
int US3;
int US4;

int Current_Room=0;
// int i = 0;

// const int LedPin = 13;    // Left LED pin, optional if you want led to light up when motion is detected
// int flag = 0;
// int count++;            

void setup() {

  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid ,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected!!");

  pinMode(PirPin1, INPUT);
  pinMode(PirPin2, INPUT);
  pinMode(PirPin3, INPUT);
  pinMode(PirPin4, INPUT);
  // pinMode(PirPin5, INPUT_PULLUP);
  // Serial.println("Dual PIR Sensor Control for Left and Right Motion Detection");

  // digitalWrite(LedPin, LOW);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT); 

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  ThingSpeak.begin(client);
}

void loop() {

  Serial.println();
  // Serial.println();

  // int Current_Room_US = 0;

  // int Room1;
  // int Room2;
  // int Room3;
  // int Room4;

  if (thresholdBroken(trigPin1, echoPin1)){
    // Serial.println("Grandma is moving);
    
    US1 = 1;
    US2 = 0;
    US3 = 0;
    US4 = 0;
    if(Current_Room==1){
      Serial.println("Grandma 2");
      Current_Room=2;
    }
    
    else{
      Serial.println("Grandma 1");
      Current_Room=1;
    }
  }
  else if (thresholdBroken(trigPin2, echoPin2)){
    // Serial.println("Grandma is moving);
    US1 = 0;
    US2 = 1;
    US3 = 0;
    US4 = 0;
    if(Current_Room==2){
      Serial.println("Grandma 3");
      Current_Room=3;

    }
    else{
      Serial.println("Grandma 2");
      Current_Room=2;
    }
  }
  else if (thresholdBroken(trigPin3, echoPin3)){
    // Serial.println("Grandma is moving);
    US1 = 0;
    US2 = 0;
    US3 = 1;
    US4 = 0;
    if(Current_Room==3){
      Serial.println("Grandma 4");
      Current_Room=4;
    }
    else{
      Serial.println("Grandma 3");
      Current_Room=3;
    }
  }
  else if (thresholdBroken(trigPin4, echoPin4)){
    // Serial.println("Grandma is moving);
    US1 = 0;
    US2 = 0;
    US3 = 0;
    US4 = 1;
    if(Current_Room==4){
      Serial.println("Grandma 3");
      Current_Room=4;
    }
    else{
      Serial.println("Grandma 4");
      Current_Room=3;
    }
  }

 
  // if (motionDetected(US1 == 1) ){
  //   // digitalWrite(LedPin, HIGH);
  //   Serial.println("Grandma has left the house");

  //   Current_Room = 0;
  // }
  // else if (motionDetected(US1 == 1)) {
  //   Serial.println("Grandma has entered the house");

  //   Current_Room = 1;
  // }
  // else if (motionDetected(US2 == 1)) {
  //   Serial.println("Grandma is going from room 2 to room 1");
    
  //   Current_Room = 1;
  // }
  // else if (motionDetected(US2 == 1)) {
  //   Serial.println("Grandma is going from room 1 to room 2");
    
  //   Current_Room = 2;
  // }
  // else if (motionDetected(US3 == 1)) {
  //   Serial.println("Grandma is going from room 3 to room 2");
    
  //   Current_Room = 2;
  // }
  // else if (motionDetected(US3 == 1)) {
  //   Serial.println("Grandma is going from room 2 to room 3");
    
  //   Current_Room = 3;
  // }
  // else if (motionDetected(US4 == 1)) {
  //   Serial.println("Grandma is going from room 4 to room 3");
    
  //   Current_Room = 3;
  // }
  // else if (motionDetected(US4 == 1)) {
  //   Serial.println("Grandma is going from room 3 to room 4");
    
  //   Current_Room = 4;
  // }

Serial.println(Current_Room);
  const int NUM_FIELDS = 4;

// Create an array to hold the field values
int fieldValues[NUM_FIELDS] = {0, 0, 0, 0};

if (Current_Room == 1) {
  fieldValues[0] = 1;
} else if (Current_Room == 2) {
  fieldValues[1] = 1;
} else if (Current_Room == 3) {
  fieldValues[2] = 1;
} else if (Current_Room == 4) {
  fieldValues[3] = 1;
}

// Set the field values
for (int i = 0; i < NUM_FIELDS; i++) {
  ThingSpeak.setField(i + 1, fieldValues[i]);
}

// Write the fields to ThingSpeak
int writeSuccess = ThingSpeak.writeFields(CHANNEL_ID, API_KEY);

// if (writeSuccess)
//  {
//   Serial.println("Data sent to ThingSpeak successfully.");
// } else 
// {
//   Serial.println("Error sending data to ThingSpeak.");
// } 
if (writeSuccess) {
  Serial.println("Data sent to ThingSpeak successfully.");
} else {
  Serial.println("Error sending data to ThingSpeak.");
}

// if (ThingSpeak.getLastWriteStatus() == 200) {
//   Serial.println("Data sent to ThingSpeak successfully.");
// } else {
//   Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
//   Serial.println(ThingSpeak.getLastWriteStatus());
// }


  
  // else if (motionDetected(PirPin5)) {
  //   Serial.println("Motion detected in Room 5");
  //   Room5 = 1;
  //   Current_Room = 5;
  // }

  // delay(100);

  // OM2M

  String label = "Label";
  String data;
  String server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  data = "[" + String(Current_Room) + "]"; 
  String req_data = String() + "{\"m2m:cin\": {"

    +
    "\"con\": \"" + data + "\","

    +
    "\"lbl\": \"" + label + "\","

    // + "\"rn\": \"" + "Entry "+String(i++) + "\","

    +
    "\"cnf\": \"text\""

    +
    "}}";
  int code = http.POST(req_data);
  http.end();
  Serial.println(code);
  
  // Serial.println("updated count");
  // Serial.print("count");

  delay(1000);

}

bool motionDetected(int pirPin) {
  if(digitalRead(pirPin) == HIGH){
    return 1;
  }
  else{
    return 0;
  }
}

int thresholdBroken(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034/2;

  Serial.print("distance = ");
  Serial.println(distance);

  if (distance < threshold){
    // Serial.println("THE GRANDMA IS MOVING!!");
    return 1;
  }
  else {
    return 0;
  }
}
