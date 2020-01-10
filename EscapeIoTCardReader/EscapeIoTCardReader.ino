#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
// Standard SPI
#include <SPI.h>
#define _GLIBCXX_USE_C99 1

// WiFi module
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"

// Card reader module
#include <MFRC522.h>
#define serverIpAddress "192.168.2.110"
#define serverPort 5001

// WS2801 LED Control
const int lightDataPin = D8;
const int numLights = 6;
Adafruit_NeoPixel lights = Adafruit_NeoPixel(numLights, lightDataPin, NEO_GRB + NEO_KHZ800);

// WiFi credentials
const char* ssid = "IncredibleHouse24";
const char* password = "ravens, rooks, crows, jackdaws";

// Debug by sending text over serial?
const bool debugSerial = true;

// Card reader pins
const int RST_PIN = D2;
const int SS_PIN = D3;
const int IRQ_PIN = D4;
// On the HiLetgo ESP8266 WiFi Board, SPI pins
// are not labeled with SPI names. 
// These are the hardware SPI pins:
// MISO pin = D6
// MOSI pin = D7
// CLK pin = D5

// Card reader
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte cardNUID[4];

// For tracking time since last card poll
unsigned long lastCardPoll;

void setup() {
  // Initialize timekeeping
  lastCardPoll = millis();
	
  // Initialize lights
  lights.begin();
  lights.show();

  // Light strip diagnostic 
  for(int k = 0; k < numLights; k++){
    lights.setPixelColor(k, 128, 128, 128);
  }
  
  lights.show();
  
  for(int k = 0; k < numLights; k++){
    lights.setPixelColor(k, 128, 0, 0);
    lights.show();
    delay(30);    
  }
  
  for(int k = 0; k < numLights; k++){
    lights.setPixelColor(k, 0, 128, 0);
    lights.show();
    delay(30);
  }
  
  for(int k = 0; k < numLights; k++){
    lights.setPixelColor(k, 0, 0, 128);
    lights.show();
    delay(30);
  }

  for(int k = 0; k < numLights; k++){
    lights.setPixelColor(k, 0, 0, 0);
    lights.show();
    delay(50);
  }
  
  if(debugSerial){
    Serial.begin(9600);
  }
  SPI.begin();
  rfid.PCD_Init();

  // Initialize card key
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
      if(debugSerial){
        Serial.println("Connecting to WiFi..");
      }
  }
  if(debugSerial){
    Serial.println("Connected to the WiFi network");
  }

  Serial.println("Waiting for card");
}

// Wait until a card is read, then send the card data to the server
void loop() {	
	if((millis() - lastCardPoll) > 1000) {
		pollForCard();
	}  
}


void pollForCard() {
	
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) return;

  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial()) return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type.
  // If it isn't, we can't read it.
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  Serial.println("Read a card.");

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    cardNUID[i] = rfid.uid.uidByte[i];
  }
  
  Serial.println();

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  Serial.println("Sending card data");
  if (WiFi.status() == WL_CONNECTED) {
    sendCardData(cardNUID);
  } else {
    Serial.println("Error in WiFi connection");
  }
}

// Send card data to the server
// Print out a debug message on serial port
void sendCardData(byte NUID[4]) {
  Serial.println("Sending card data via WiFi");
  HTTPClient http;

  http.begin(serverIpAddress, serverPort, "/EspApi/PutCardData/");
  http.addHeader("Content-Type", "application/json");

  char buffer[256];

  // Data is sent in JSON format and  parsed by the server into object field data
  sprintf(buffer, "{sender: \"Card Reader 1\", cardData: [%d, %d, %d, %d]}", NUID[0], NUID[1], NUID[2], NUID[3]);
  Serial.print("Sending: ");
  Serial.println(buffer);

  int httpResponseCode = http.PUT(buffer);

  if (httpResponseCode > 0) {
    const String response = http.getString();
    int value = response.toInt();
    Serial.println(value, HEX);
    setAllLightsToSame(value);
    Serial.println(response);   
  } else {
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setAllLightsToSame(int color) {
  for(int k = 0; k < numLights; k++) {
    lights.setPixelColor(k, color);
  }
  
  lights.show();  
}
