#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

Servo myservo;  // create servo object to control a servo

const char* ssid     = "RPi3";      // SSID
const char* password = "rpi3hackathon";      // Password
const char* host = "10.42.0.1";
const int  port = 3000;
const int   watchdog = 2000;
unsigned long previousMillis = millis();
String oldpayload = "2";
 
HTTPClient http;
 
void setup() {
  Serial.begin(115200);
  delay(10);
  
  myservo.attach(D1);  // attaches the servo on D1 to the servo object
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.setDebugOutput(true);
  
}
 
int value = 0;
 
void loop() {
unsigned long currentMillis = millis();
 
 if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    } else {  
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());

      String url = "/status";
      String urllock = "/boxlocked";
      String urlunlock = "/boxunlocked";
      
      Serial.print("connecting to ");
      Serial.println(host);
      Serial.print("Requesting URL: ");
      Serial.println(url);
     
      http.begin(host,port,url);
      int httpCode = http.GET();
      if (httpCode) {
        String payload = http.getString();
        if (httpCode == 200) {
          Serial.println(payload);
          

          if(payload == "0" && oldpayload != "0") {
            myservo.write(173);
            http.begin(host,port,urllock);
            http.POST("Locked");
            Serial.println("Closed");
          }

          if(payload == "1" && oldpayload != "1") {
            myservo.write(75);
            http.begin(host,port,urlunlock);
            http.POST("Unlocked");
            Serial.println("Opened");
          }

          Serial.println(".......................................................");

        }
        oldpayload = payload;
      }

      }

    }
  }

