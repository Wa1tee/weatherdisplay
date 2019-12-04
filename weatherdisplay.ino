

#include <Arduino.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

// #include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS D0  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_DC D8  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_RST -1 //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TS_CS D3   //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)

#define SSID "Ebin-AP" 				// Wifi SSID
#define PASSWD "Kissimirri" 		// Wifi passwd
#define IP "http://95.216.173.63:3001/api/getReadings/1"	// Server IP-address
#define NAME "weatherdisplay"			// Device name

ESP8266WiFiMulti WiFiMulti;

// // Initialize sensor
// Adafruit_BME280 bme;

// Initialize json document
// const int capacity = JSON_OBJECT_SIZE(4);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


void setup() {
  	Serial.begin(115200);

  	tft.begin();
  	tft.setTextColor(ILI9341_WHITE);
  	tft.setCursor(0, 0);
  	tft.fillScreen(ILI9341_BLACK);
  	yield();

  	WiFi.mode(WIFI_STA);
  	WiFiMulti.addAP(SSID, PASSWD);

}

void loop() {

	// DynamicJsonDocument doc(capacity);
  	// char output[128];
	// JsonObject& data = jb.createObject();

  	String payload = "";

  	Serial.println();
	
	if (WiFiMulti.run() == WL_CONNECTED)
	{
		HTTPClient http;

		// Serial.println("[HTTP] begin...");


		http.begin(IP);
		

		int httpcode = http.GET();

		if (httpcode == HTTP_CODE_OK)
		{
			// Serial.println("Toimii :D");
			tft.setCursor(0, 0);
			tft.setTextSize(3);
			payload = http.getString();
			
			
			// if(error){
			// 	tft.setTextSize(5);
			//     tft.println("JSON ERROR");
			// }

			
			// float temp = doc["temperature"];
			// tft.println(doc["temperature"].as<float>(), 6);
			// String temp = doc["temperature"];

			for(int i=0; i<payload.length() -1; i++){
			    if(payload[i]){
			        
			    }
			}

			tft.println(temp);
		}else{
			
			tft.setTextColor(ILI9341_RED);
			tft.setCursor(0, 20);
			tft.setTextSize(3);
			tft.println("Ei toimi :(");
			tft.setCursor(0, 100);
			tft.setTextSize(5);
			tft.println(httpcode);
		}

		http.end();
	}

	delay(10000);
}

void parseTemp(String s){
	
}