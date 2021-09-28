/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>  
#include <GP2YDustSensor.h>
#include <ArduinoJson.h>

const uint8_t SHARP_LED_PIN = 25;   // Sharp Dust/particle sensor Led Pin
const uint8_t SHARP_VO_PIN = 34;    // Sharp Dust/particle analog out pin used for reading 
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

#define DHTPIN 15
const char* ssid = "mehdilinksys";
const char* password = "mehdi@1354@";
//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.1.100:8000/senddata/";
DHT dht(DHTPIN, DHT11);
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  dustSensor.begin();
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
      float h = dht.readHumidity();  
      float t = dht.readTemperature();  
      int gp2    = dustSensor.getRunningAverage();//ug/m3 GP2YDustSensor
      int mq9    = analogRead(33);
      delay(1000);
      int mq135  = analogRead(35);
      delay(1000);
      
      int mq2    = analogRead(32);
        Serial.print("mq9==>");
        Serial.println(mq9);
        Serial.print("mq2==>");
        Serial.println(mq2);
        Serial.print("mq135==>");
        Serial.println(mq135);
        Serial.print("Dust density: ");
        Serial.print(dustSensor.getDustDensity());
        Serial.print(" ug/m3; Running average: ");
        Serial.print(dustSensor.getRunningAverage());
        Serial.println(" ug/m3");
        if (isnan(h) || isnan(t)){
    
        Serial.println("Failed to read from DHT sensor!");  
        return;
    }
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
    //  http.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
      // Send HTTP POST request
      //http.POST("{\"temp\":\"30\",\"moist\":\"15\",\"time\":\"2021\"}");
     // int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
        String postStr = "{\"temp\":\""; 
        postStr += String(t);
        postStr +="\",\"moist\":\"";
        postStr += String(h);
        postStr += "\",\"time\":\"2021\"}";
    //   ! int httpResponseCode = http.POST("{\"temp\":\"30\",\"moist\":\"15\",\"time\":\"2021\"}");
      DynamicJsonDocument doc(2048);
      doc["temp"] = String(t);
      doc["moist"] = String(h);
      doc["mq9"] = String(mq9);
      doc["mq2"] = String(mq2);
      doc["mq135"] = String(mq135);
      doc["gp2"] = String(gp2);
      doc["time"] = "2020-02-04";
      String json;
      serializeJson(doc, json);
     //!   int httpResponseCode = http.POST(postStr);
       int httpResponseCode = http.POST(json);
      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}