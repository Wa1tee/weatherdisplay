/*
 * Weather data display to be used with the IoT web app and sensor node by Digit Codeclub (Digitin koodikerho) https://github.com/DigitKoodit/iot-workshop
 * Board used: 	Wemos D1 Mini Pro v2.0.0
 * Shield: 		Wemos TFT 2.4" touch screen shield for D1 mini
 *
 * By Antti Auranen
 */

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


#define SSID "" 										// Wifi SSID
#define PASSWD "" 									// Wifi passwd

// #define SSID "Ebin-AP" 							// Wifi SSID
// #define PASSWD "Kissimirri" 						// Wifi passwd
#define IP "http://127.0.0.1:3001/api/getReadings/1"	// Server IP-address
#define NAME "WeatherDisplay"						// Device name



ESP8266WiFiMulti WiFiMulti;

// // Initialize sensor
// Adafruit_BME280 bme;

// Initialize json document
// const int capacity = JSON_OBJECT_SIZE(4);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);



// String parseTemp(String s){
// 	String text = "";

// 	int pos = s.find("\"temperature\":");
// 	tft.println("Position is %s", pos);
// 	return "";
// }

void setup() {
  	Serial.begin(115200);
	pinMode(A0, INPUT);

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
	float volt;
	unsigned int raw = 0;
  	String payload = "";

  	Serial.println();
	
	raw = analogRead(A0);
  	volt=raw/1023.0;
  	volt=volt*4.2;

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
			// Serial.println(payload);
			// String temp = parseTemp(payload);
			tft.fillScreen(ILI9341_BLACK);
			tft.setTextColor(ILI9341_WHITE);
			// tft.println("Temperature: ");
			// tft.setCursor(0, 10);
			// tft.println(temp);

			int pos1 = payload.indexOf("\"temperature\":");
			String temp = "";
			pos1 += 14;
			for(int i=pos1; i<pos1 + 20; i++){
			    if(payload[i] == ','){
			        break;
			    }else{
			    	temp += payload[i];
			    }
			}
			tft.print("Deg C: ");
			tft.println(temp);

			int pos2 = payload.indexOf("\"pressure\":");
			String press = "";
			pos2 += 11;
			for(int i=pos2; i<pos2 + 20; i++){
			    if(payload[i] == ','){
			        break;
			    }else{
			    	press += payload[i];
			    }
			}
			// tft.setCursor(0, 10);
			tft.print("Psi: ");
			tft.println(press);

			int pos3 = payload.indexOf("\"humidity\":");
			String humi = "";
			pos3 += 11;
			for(int i=pos3; i<pos3 + 20; i++){
			    if(payload[i] == ','){
			        break;
			    }else{
			    	humi += payload[i];
			    }
			}
			// tft.setCursor(0, 20);
			tft.print("%: ");
			tft.println(humi);

			tft.setTextColor(ILI9341_GREEN);
			tft.print("Battery: ");
			tft.println(volt);

			// float temp = parseTemp(payload);
			// tft.println(doc["temperature"].as<float>(), 6);
			// String temp = doc["temperature"];

			

			// tft.println(temp);
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
