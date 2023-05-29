// #include <HTTPClient.h>
#include <Wire.h>
#include "max32664.h"
// #include <stdlib.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ThingSpeak.h>

#define mqttport 1883

char ssid[] = "Wifi Name";
char password[] = "Wifi Pass";
const char* server = "mqtt3.thingspeak.com";
char mqttUserName[] = "FCoPExU9MAUQISkGCBEDCz0";
char mqttClientID[] = "FCoPExU9MAUQISkGCBEDCz0";
char mqttpass[] = "OwEpA4RuSjFFfZNzg6fGoBXa";
int writeChannelID = 2151521;
char writeAPIKey[] = "XVEHO8NYR0DRP5KQ";


WiFiClient client;
PubSubClient mqttClient(server, mqttport, client);

// #define CSE_IP "192.168.207.133"
// #define CSE_PORT 5089
// #define OM2M_ORGIN "admin:admin"
// #define OM2M_MN "/~/in-cse/in-name/"
// #define OM2M_AE "Health-Data"
// #define OM2M_DATA_CONT "Pulse_Oximeter"

#define RESET_PIN 4
#define MFIO_PIN 5
#define RAWDATA_BUFFLEN 250

max32664 MAX32664(RESET_PIN, MFIO_PIN, RAWDATA_BUFFLEN);

// HTTPClient http;

// int i = 0;

void mfioInterruptHndlr(){
  //Serial.println("i");
}

void enableInterruptPin(){

  //pinMode(mfioPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MAX32664.mfioPin), mfioInterruptHndlr, FALLING);

}

void loadAlgomodeParameters(){

  algomodeInitialiser algoParameters;
  /*  Replace the predefined values with the calibration values taken with a reference spo2 device in a controlled environt.
      Please have a look here for more information, https://pdfserv.maximintegrated.com/en/an/an6921-measuring-blood-pressure-MAX32664D.pdf
      https://github.com/Protocentral/protocentral-pulse-express/blob/master/docs/SpO2-Measurement-Maxim-MAX32664-Sensor-Hub.pdf
  */

  algoParameters.calibValSys[0] = 120;
  algoParameters.calibValSys[1] = 122;
  algoParameters.calibValSys[2] = 125;

  algoParameters.calibValDia[0] = 80;
  algoParameters.calibValDia[1] = 81;
  algoParameters.calibValDia[2] = 82;

  algoParameters.spo2CalibCoefA = 1.5958422;
  algoParameters.spo2CalibCoefB = -34.659664;
  algoParameters.spo2CalibCoefC = 112.68987;

  MAX32664.loadAlgorithmParameters(&algoParameters);
}



void setup(){

  Serial.begin(115200);

  WiFi.begin(ssid ,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(server, mqttport);

  Wire.begin();

  loadAlgomodeParameters();

  int result = MAX32664.hubBegin();
  if (result == CMD_SUCCESS){
    Serial.println("Sensorhub begin!");
  }else{
    //stay here.
    while(1){
      Serial.println("Could not communicate with the sensor! please make proper connections");
      delay(5000);
    }
  }

  bool ret = MAX32664.startBPTcalibration();
  while(!ret){

    delay(10000);
    Serial.println("failed calib, please retsart");
    //ret = MAX32664.startBPTcalibration();
  }

  delay(1000);

  //Serial.println("start in estimation mode");
  ret = MAX32664.configAlgoInEstimationMode();
  while(!ret){

    //Serial.println("failed est mode");
    ret = MAX32664.configAlgoInEstimationMode();
    delay(10000);
  }

  //MAX32664.enableInterruptPin();
  Serial.println("Getting the device ready..");
  delay(1000);
}

//MQTT

void mqttPublish(long writeChannelID, char* pubWriteAPIKey, float systole, float diastole, float hr, float spo2){
  // int index = 0;
  String dataString = "";

  // while(fieldArray[index] == 1){
  dataString = "field1="+String(hr)+"&field2="+String(systole)+"&field3="+String(diastole)+"&field4="+String(spo2);
  // index++;
  // Serial.println(dataString);
  String topicString = "channels/" + String( writeChannelID ) + "/publish";
  // Serial.println(topicString);
  mqttClient.publish(topicString.c_str(), dataString.c_str());
  // Serial.println(writeChannelID);
}

void loop(){

  while (!mqttClient.connected()){
    if(mqttClient.connect(mqttClientID, mqttUserName, mqttpass)){
      Serial.print("MQTT to ");
      Serial.print( server);
      Serial.print(" at port ");
      Serial.print( mqttport );
      Serial.println(" successful.");
    }
    else {
      Serial.print("MQTT connection failed, rc = ");
      Serial.print( mqttClient.state() );
      Serial.println(" Will try again in a second.");
      delay(1000);
    }
  }

  mqttClient.loop();

  uint8_t num_samples = MAX32664.readSamples();

  if(num_samples){

    Serial.print("sys = ");
    float systole = MAX32664.max32664Output.sys;
    // Serial.print(MAX32664.max32664Output.sys);
    Serial.print(systole);

    Serial.print(", dia = ");
    float diastole = MAX32664.max32664Output.dia;
    // Serial.print(MAX32664.max32664Output.dia);
    Serial.print(diastole);

    Serial.print(", hr = ");
    float hr = MAX32664.max32664Output.hr;
    // Serial.print(MAX32664.max32664Output.hr);
    Serial.print(hr);

    Serial.print(" spo2 = ");
    float spo2 = MAX32664.max32664Output.spo2;
    // Serial.println(MAX32664.max32664Output.spo2);
    Serial.println(spo2);

    //MQTT

    mqttPublish(writeChannelID, writeAPIKey, systole, diastole, hr, spo2);

  
    // OM2M

    // String label = "Label";
    // String data;
    // String server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

    // http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

    // http.addHeader("X-M2M-Origin", OM2M_ORGIN);
    // http.addHeader("Content-Type", "application/json;ty=4");
    // http.addHeader("Content-Length", "100");

    // data = "[" + String(systole) + ", " + String(diastole) + ", " + String(hr) + ", " + String(spo2) + "]"; 
    // String req_data = String() + "{\"m2m:cin\": {"

    //   +
    //   "\"con\": \"" + data + "\","

    //   +
    //   "\"lbl\": \"" + label + "\","

    //   // + "\"rn\": \"" + "Entry "+String(i++) + "\","

    //   // +
    //   // "\"cnf\": \"text\""

    //   +
    //   "}}";
    // int code = http.POST(req_data);
    // http.end();
    // Serial.println(code);
  }

  delay(100);
}