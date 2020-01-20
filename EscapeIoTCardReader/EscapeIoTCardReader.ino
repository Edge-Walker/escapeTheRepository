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
  }
    
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 128, 0, 0);
    delay(100);    
  }
  
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 128, 0);
    delay(100);
  }
  
  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 0, 128);
    delay(100);
  }

  for(int k = 0; k < numLights; k++){
    _lights.setPixelColor(k, 0, 0, 0);
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
