#include "WiFi.h"
#include "ThingSpeak.h"
#define LPG_ID 2163504
#define LPG_API_KEY "187CSQURHUJGTFM9"
#define TEMP_ID 2151439
#define TEMP_API_KEY "LF9Z2ZSVFXM5NT9Y"
#define FLAME_ID  2165795
#define FLAME_API_KEY "UAR8XNTOZDVUVM18"
#define SMOKE_ID 2165803
#define SMOKE_API_KEY "ZCXLSEK4TJQEJ7UT"

#define WIFI_TIMEOUT_MS 20000
char ssid[]="Divu";
char password[]="Divyaraj@1234";

#define temp_pin 33
#define ADC_VREF_mV 900.0
#define ADC_RESOLUTION 4096.0

#define lpgSensorDigitalPin 2
#define lpgSensorAnalogPin 36
#define flameDigital 32
#define flameAnalog 39
#define smokeDigital 35
#define smokeAnalog 34
#define ALERT_ID 2165472
#define ALERT_API_KEY "CB1XBMIGOF1UPLXR"
int counter=0;
float x_sum=0;
float nloop=0;
WiFiClient client;
void connectToWiFi(){
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Wifi...");
  }
  Serial.println("Connected");
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  ThingSpeak.begin(client);
  pinMode(lpgSensorDigitalPin, INPUT);
  pinMode(lpgSensorAnalogPin, INPUT);
  pinMode(flameDigital, INPUT);
  pinMode(flameAnalog, INPUT);
  pinMode(smokeDigital, INPUT);
  pinMode(smokeAnalog, INPUT);
  pinMode(temp_pin, INPUT);
}

void loop() {    
  int lpgValueDigital = digitalRead(lpgSensorDigitalPin);
  int lpgValueAnalog = analogRead(lpgSensorAnalogPin);
    
  Serial.print("Digital of LPG: ");
  Serial.println(lpgValueDigital);
  Serial.print("Analog of LPG: ");
  Serial.println(lpgValueAnalog);//base value: 2100-2200
    
  int flameValueDigital = digitalRead(flameDigital);
  int flameValueAnalog = analogRead(flameAnalog);
    
  Serial.print("Digital of Flame: ");
  Serial.println(flameValueDigital);
  Serial.print("Analog of Flame: ");//base value: 4095
  Serial.println(flameValueAnalog);
    

  int smokeValueDigital = digitalRead(smokeDigital);
  int smokeValueAnalog = analogRead(smokeAnalog);
 
  Serial.print("Digital of Smoke: ");//unrealiable
  Serial.println(smokeValueDigital);
  Serial.print("Analog of Smoke: ");//base value: 200-250
  Serial.println(smokeValueAnalog);
  // Serial.println(" ");
    
  ThingSpeak.writeField( LPG_ID ,1, lpgValueAnalog, LPG_API_KEY);
  ThingSpeak.writeField( LPG_ID ,2, lpgValueDigital, LPG_API_KEY);
  ThingSpeak.writeField( FLAME_ID ,1, flameValueAnalog, FLAME_API_KEY);
  ThingSpeak.writeField( FLAME_ID ,2, flameValueDigital, FLAME_API_KEY);
  ThingSpeak.writeField( SMOKE_ID ,1, smokeValueAnalog, SMOKE_API_KEY);
  ThingSpeak.writeField( SMOKE_ID ,2, smokeValueDigital, SMOKE_API_KEY);

  if(lpgValueDigital==0||flameValueDigital==0||smokeValueAnalog>300)
    ThingSpeak.writeField( ALERT_ID ,1, 1, ALERT_API_KEY);
  else
    ThingSpeak.writeField( ALERT_ID ,1, 0, ALERT_API_KEY);


  // ThingSpeak.writeField()
  //  ThingSpeak.writeField( CHANNEL_ID ,1, tempr/nloop, CHANNEL_API_KEY);

  Serial.print("Temperature measured is: ");
  float temp=analogRead(temp_pin);//base value- 19-30(room temp) 30-35(body-temp)
  temp *= (ADC_VREF_mV/ADC_RESOLUTION);
  temp /= 10;
  Serial.println(temp);//threshold temp =>50
  Serial.println(" ");
  ThingSpeak.writeField( TEMP_ID ,1, temp, TEMP_API_KEY);

 delay(3000);

}