#include <HTTPClient.h>
#include <WiFi.h>

char ssid[] = "moto g22_9515";
char password[] = "3b5bvrygds7irdj";

#define CSE_IP "192.168.207.133"
#define CSE_PORT 5089
#define OM2M_ORGIN "admin:admin"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "User-Patterns"
#define OM2M_DATA_CONT "What_Room_Is_Grandma_In"

const int PirPin1 = 2;
const int PirPin2 = 4;    
const int PirPin3 = 5;
const int PirPin4 = 18;
const int PirPin5 = 19;

HTTPClient http;

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

  // pinMode(PirPin1, INPUT);
  // pinMode(PirPin2, INPUT);
  pinMode(PirPin3, INPUT);
  // pinMode(PirPin4, INPUT);
  // pinMode(PirPin5, INPUT_PULLUP);
  // Serial.println("Dual PIR Sensor Control for Left and Right Motion Detection");

  // digitalWrite(LedPin, LOW);   
}

void loop() {

  Serial.println();
  Serial.println();

  int Current_Room = 0;
  int Room1 = 0;
  int Room2 = 0;
  int Room3 = 0;
  int Room4 = 0;
  int Room5 = 0;

  // if (motionDetected(PirPin1)) {
  //   // digitalWrite(LedPin, HIGH);
  //   Serial.println("Motion detected in Room 1");
  //   Room1 = 1;
  //   Current_Room = 1;
  // }
  // else {
  //   Room1 = 0;
  // }

  // else if (motionDetected(PirPin2)) {
  //   Serial.println("Motion detected in Room 2");
  //   Room2 = 1;
  //   Current_Room = 2;
  // }
  // else {
  //   Room2 = 0;
  // }

  if (motionDetected(PirPin3)) {
    Serial.println("Motion detected in Room 3");
    Room3 = 1;
    Current_Room = 3;
  }
  // else {
  //   Room3 = 0;
  // }

  // else if (motionDetected(PirPin4)) {
  //   Serial.println("Motion detected in Room 4");
  //   Room4 = 1;
  //   Current_Room = 4;
  // }
  // else {
  //   Room4 = 0;
  // }

  // else if (motionDetected(PirPin5)) {
  //   Serial.println("Motion detected in Room 5");
  //   Room5 = 1;
  //   Current_Room = 5;
  // }
  // else {
  //   Room5 = 0;
  // }

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
  return digitalRead(pirPin)==HIGH;
}