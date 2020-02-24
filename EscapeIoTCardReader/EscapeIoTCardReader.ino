/* ===================================

TO DO: 
Use IRQ pin instead of polling reader (saves battery, flops)

Add support for server to select a method and provide arguments

Add EEPROM read/write support

Add RFID card data read/write support
=====================================*/


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Standard SPI
#include <SPI.h>

// WiFi module
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"

// Card reader module
#include <MFRC522.h>

// Make sure we use c99
#define _GLIBCXX_USE_C99 1

// CONFIGURATION ----------------------

// Server Settings
#define serverIpAddress "192.168.2.1"
#define serverPort 5001

// WS2812B LED Control
#define  lightDataPin D1
#define  _numLights 6

//  ----------------------

Adafruit_NeoPixel _lights = Adafruit_NeoPixel(_numLights, lightDataPin, NEO_GRB + NEO_KHZ800);

// WiFi credentials
const char* _ssid = "EscapeTheWiFi";
const char* _password = "HexSpecsTreks";

// Debug by sending text over serial?
const bool debugSerial = true;

// Card reader pins
const int _resetPin = D2;
const int _selectPin = D3;
const int _interruptPin = D4;
// On the HiLetgo ESP8266 WiFi Board, SPI pins
// are not labeled with SPI names. 
// These are the hardware SPI pins:
// CLK pin = D5
// MISO pin = D6
// MOSI pin = D7


// Card reader
MFRC522 _rfid(_selectPin, _resetPin);
MFRC522::MIFARE_Key _key;
byte _cardNUID[4];

// Used to track time
unsigned long _time;
unsigned long _lastCardPoll;
bool _cardPresent = false;

void setup() {

  // Initialize lights
  _lights.begin();
  _lights.show();

  lightstripDiag(_numLights);
  
  if(debugSerial){
    Serial.begin(9600);
  }
  SPI.begin();
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);


  WiFi.begin(_ssid, _password);


  // Try to connect, but after ten failures, assume we are
  // just charging and sleep.

  int retries = 10;
  while ((WiFi.status() != WL_CONNECTED) && retries > 0) {
    delay(1000);
      if(debugSerial){
        Serial.println("Connecting to WiFi..");
      }
      retries--;
  }

  if(retries == 0) {
    doPanic(_numLights);
    while(true) {
      delay(9999999);
    }
  }
  
  if(debugSerial){
    Serial.println("Connected to the WiFi network");
  }

  lightstripDiag(_numLights);

  Serial.println("Waiting for card");

  _rfid.PCD_Init();

  // Initialize card key
  for (byte i = 0; i < 6; i++) {
    _key.keyByte[i] = 0xFF;
  }
}


// ========== MAIN LOOP ==========
// Wait until a card is read, then send the card data to the server
void loop() {
  _time = millis();
  if(_time - _lastCardPoll > 1000) {
    if (_cardPresent) {
      checkForNoCard();
    }
    else {
      pollForCard();
    }
  }
  
  delay(10);  
}

void checkForNoCard() {
  byte buffer[4];
  byte bufferSize = 4;
  if (_rfid.PICC_WakeupA(buffer, &bufferSize) == MFRC522::STATUS_OK) {
    _rfid.PICC_HaltA();
    return;
  }

  _cardPresent = false;
  setAllLights(0x000000);
  // Telling the server isn't super important right now
}

void pollForCard() {
  // Look for new cards
  if (!_rfid.PICC_IsNewCardPresent()) return;

  // Verify if the NUID has been read
  if (!_rfid.PICC_ReadCardSerial()) return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = _rfid.PICC_GetType(_rfid.uid.sak);
  // Serial.println(_rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type.
  // If it isn't, we can't read it.
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  

  // Flash lights so we know something has been read.
  flashLights(100);
  _cardPresent = true;
  // Serial.println("Read a card.");

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    _cardNUID[i] = _rfid.uid.uidByte[i];
  }
  
  Serial.println();

  // Halt PICC
  _rfid.PICC_HaltA();

  // Stop encryption on PCD
  _rfid.PCD_StopCrypto1();

  Serial.println("Sending card data");
  if (WiFi.status() == WL_CONNECTED) {
    sendCardData(_cardNUID);
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
  sprintf(buffer, "{senderID: \"Sender ID\", cardData: [%d, %d, %d, %d], senderMacAddr:\"NOTSET\"}", NUID[0], NUID[1], NUID[2], NUID[3]);
  String message = String("{senderID: \"") + 
    ESP.getChipId() +
    String("\", cardData: [") +
    String(NUID[0], DEC) + ", " + 
    String(NUID[1], DEC) + ", " + 
    String(NUID[2], DEC) + ", " +  
    String(NUID[3], DEC) +
    String("], senderMacAddr:\"") + 
    WiFi.macAddress() + 
    String("\"}");
    
  Serial.println(message);
  //Serial.print("Sending: ");
 // Serial.println(buffer);

  int httpResponseCode = http.PUT(message);

  if (httpResponseCode > 0) {
    const String response = http.getString();
    Serial.println("Server response:");
    Serial.println(response);
  
    setEachLight(response, _numLights);
  } else {
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void flashLights(int duration) {
    setAllLights(0x000000);
    delay(duration);
    setAllLights(0xFFFFFF);
    delay(duration);
}

void setAllLights(int color) {
  for(int k = 0; k < _numLights; k++) {
    _lights.setPixelColor(k, color);
  }
  
  _lights.show();  
}

// Indicate an unintended state has been reached.
void doPanic(int numLights) {
  for(int k = 0; k < 10; k++) {
    flashLights(50);
    for (int i = 0; i < numLights; i ++) {
      setAllLights(0xFF0000);
      delay(20);
    }
  }

  setAllLights(0x000000);
}


// Accepts a string containing hex digits.
// Splits the string into sets of 6 digits each,
// and treats each as a color to assign to a different light.
void setEachLight(String str, int numLights) {
  // Make sure the string is long enough to address every light
  if(6 * (numLights) > str.length()) {
    doPanic(_numLights);
    Serial.print("String length too short.");
    Serial.println(str.length());
  }
  
  for(int i = 0; i < numLights; i++) {
    
    char buffer[8];
    int startIndex = i * 6;
    unsigned long color;
  
    String colorStr = str.substring(startIndex, startIndex + 6);
    
    // Serial.print("Color string is ");
    // Serial.println(colorStr);    
    
    colorStr.toCharArray(buffer,8);    
    color = strtoul(buffer, NULL, 16);

    // Serial.print("Parsed color integer: ");
    // Serial.println(color, HEX);
    
    _lights.setPixelColor(i, color);
  }

  _lights.show();
}

void lightstripDiag(int numLights) {
  
  // Light strip diagnostic 
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 128, 128, 128);
  	_lights.show();
	  delay(50);	
  }
    
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 128, 0, 0);
  	_lights.show();
    delay(50);    
  }
  
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 128, 0);
  	_lights.show();
    delay(50);
  }
  
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 0, 128);
  	_lights.show();
    delay(50);
  }

  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 0, 0);
	 _lights.show();
    delay(50);
  }
}
