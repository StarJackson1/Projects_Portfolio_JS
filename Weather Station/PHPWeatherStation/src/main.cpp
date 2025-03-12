#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFiUdp.h>
#include <mDNSResolver.h>

using namespace mDNSResolver;


//control variables
#define isDelay true
#define delayMins 5


char ssid[] = "A305";
char pass[] = "maui!!!!";
const char* serverName = "http://data.mzdavis.net/jackson-data-insert.php";
String apiKeyValue = "gocklord";
#define SEALEVELPRESSURE_HPA 860
//environment variables
float temperature;
float pressure;
float humidity;
float voc;

Adafruit_BME680 bme(&Wire);

void wifiConnect() {
  Serial.println("Connecting WiFi");
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected with IP Address: ");
  Serial.print(WiFi.localIP());
}
void sensorSetup() {
  if (!bme.begin()) {
    Serial.println("BME Sensor not initialized");
    while(1);
}
bme.setTemperatureOversampling(BME680_OS_8X);
bme.setHumidityOversampling(BME680_OS_2X);
bme.setPressureOversampling(BME680_OS_4X);
bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
bme.setGasHeater(320, 150); // 320*C for 150 ms
}
void performReading() {
  if (!bme.performReading()) {
    Serial.println("Sensor Reading Failed");
  }
  temperature = bme.temperature;
  pressure = bme.pressure/1000.00;
  humidity = bme.humidity;
  voc = bme.gas_resistance/1000.00;
}
void postData() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient https;
    https.begin(client, serverName);
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //prepare post string
    String httpRequestData = "api_key=" + apiKeyValue + "&tempVal=" + String(temperature) + "&pressVal=" + String(pressure) + "&humVal=" + String(humidity) + "&vocVal=" + String(voc) + "";
    Serial.println("");
    Serial.print("http request data: ");
    Serial.println(httpRequestData);
    int httpResponseCode = https.POST(httpRequestData);
    if (httpResponseCode > 0) {
      Serial.print("http response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("http error code: ");
      Serial.println(httpResponseCode);
    }

    https.end();
  } else {
    wifiConnect();
  }
}
void setup() {
  Serial.begin(115200);
  sensorSetup();
  wifiConnect();
}

void loop() {
  performReading();
  postData();
  Serial.println("Data Successfully Uplpaded");
  if (isDelay) {delay(delayMins * 60000);}
}


