#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

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
#define serverIpAddress "192.168.2.110"
#define serverPort 5001

// WS2812B LED Control
#define  lightDataPin D8
#define  numLights 6

//  ----------------------

Adafruit_NeoPixel _lights = Adafruit_NeoPixel(numLights, lightDataPin, NEO_GRB + NEO_KHZ800);

// WiFi credentials
const char* _ssid = "IncredibleHouse24";
const char* _password = "ravens, rooks, crows, jackdaws";

// Debug by sending text over serial?
const bool debugSerial = true;

// Card reader pins
const int _resetPin = D2;
const int _selectPin = D3;
const int _interruptPin = D4;
// On the HiLetgo ESP8266 WiFi Board, SPI pins
// are not labeled with SPI names. 
// These are the hardware SPI pins:
// MISO pin = D6
// MOSI pin = D7
// CLK pin = D5

// Card reader
MFRC522 _rfid(_selectPin, _resetPin);
MFRC522::MIFARE_Key _key;
byte _cardNUID[4];

// Tracks the current unadjusted base color for each LED
// Used to create effects without throwing away current color
uint8_t _red[numLights];
uint8_t _green[numLights];
uint8_t _blue[numLights];

// Tracks the current frame for breathing LED effect
// so that each light can be on a different frame
unsigned long _sineFrame[numLights];

// Used to track time
unsigned long _time;
unsigned long _lastCardPoll;

// ================================================================
static const uint8_t _SineTable[256] = {
  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  
  6,  7,  9, 10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29,
 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67,
 70, 73, 76, 79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,
118,121,124,128,131,134,137,140,143,146,149,152,155,158,162,165,
167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,
211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,
241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,
255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,
249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,
224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,
185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,
137,134,131,128,124,121,118,115,112,109,106,103,100, 97, 93, 90,
88, 85, 82,  79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47,
44, 42, 40,  37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15,
 14, 12, 11, 10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1  
   };
// ================================================================

void setup() {
  // Initialize EEPROM
  EEPROM.begin(512);

  // Initialize lights
  _lights.begin();
  _lights.show();

  lightstripDiag();
  
  if(debugSerial){
    Serial.begin(9600);
  }
  SPI.begin();
  _rfid.PCD_Init();

  // Initialize card key
  for (byte i = 0; i < 6; i++) {
    _key.keyByte[i] = 0xFF;
  }

  WiFi.begin(_ssid, _password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
      if(debugSerial){
        Serial.println("Connecting to WiFi..");
      }
  }
  if(debugSerial){
    Serial.println("Connected to the WiFi network");
  }

  lightstripDiag();

  Serial.println("Waiting for card");
}


// ========== MAIN LOOP ==========
// Wait until a card is read, then send the card data to the server
void loop() {
  _time = millis();
  if(_time - _lastCardPoll > 1000) {
    pollForCard();
  }
  
  delay(10);  
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

  Serial.println("Read a card.");

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
  sprintf(buffer, "{sender: \"Cinnamon\", cardData: [%d, %d, %d, %d]}", NUID[0], NUID[1], NUID[2], NUID[3]);
  Serial.print("Sending: ");
  Serial.println(buffer);

  int httpResponseCode = http.PUT(buffer);

  if (httpResponseCode > 0) {
    const String response = http.getString();
    
    Serial.println(response);

    // Flash lights so we know something has been read.
    flashLights(15);
    
    setEachLight(response);
    
    Serial.print("PUT response code: ");
    Serial.println(response);   
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
  for(int k = 0; k < numLights; k++) {
    _lights.setPixelColor(k, color);
  }
  
  _lights.show();  
}

// Indicate an unintended state has been reached.
void doPanic() {
  for(int k = 0; k < 10; k++) {
    flashLights(50);
    for (int i = 0; i < numLights; i ++) {
      setAllLights(0xFF0000);
      delay(200);
    }
  }
}

void setEachLight(String str) {
  // Make sure the string is long enough to address every light
  if(6 * (numLights) > str.length()) {
    doPanic();
    Serial.print("String length too short.");
    Serial.println(str.length());
  }
  
  for(int i = 0; i < numLights; i++) {
    
    char buffer[8];
    int startIndex = i * 6;
    unsigned long color;
  
    String colorStr = str.substring(startIndex, startIndex + 6);
    
    Serial.print("Color string is ");
    Serial.println(colorStr);    
    
    colorStr.toCharArray(buffer,8);    
    color = strtoul(buffer, NULL, 16);

    Serial.print("Parsed color integer: ");
    Serial.println(color, HEX);
    
    _lights.setPixelColor(i, color);
  }

  _lights.show();
}

void lightstripDiag() {
  
  // Light strip diagnostic 
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 128, 128, 128);
	_lights.show();
	delay(100);	
  }
    
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 128, 0, 0);
	_lights.show();
    delay(100);    
  }
  
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 128, 0);
	_lights.show();
    delay(100);
  }
  
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 0, 128);
	_lights.show();
    delay(100);
  }

  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 0, 0);
	_lights.show();
    delay(100);
  }
}

void saveColorData(int lightNum, unsigned long color) {
  if(lightNum > numLights) {
    return;
  }
  EEPROM.write(lightNum * 32, color); 

  // TODO
}

uint16_t getColorData(int lightNum) {
  EEPROM.read(lightNum * 32);

  // TODO
}
