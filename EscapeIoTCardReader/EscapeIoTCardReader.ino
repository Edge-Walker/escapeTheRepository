/* ===================================

TO DO: 
Use IRQ pin instead of polling reader (saves battery, flops)

Add support for server to select a method and provide arguments

Add EEPROM read/write support

Add RFID card data read/write support
=====================================*/


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
#define serverIpAddress "10.7.32.5"
#define serverPort 5001

// WS2812B LED Control
#define  lightDataPin D1
#define  _numLights 6

//  ----------------------

Adafruit_NeoPixel _lights = Adafruit_NeoPixel(_numLights, lightDataPin, NEO_GRB + NEO_KHZ800);
byte _lightValues[_numLights * 3];
byte _lightTargets[_numLights * 3];
byte _lightBases[_numLights * 3];
bool _lightValuesAnim = false;
int _flashingMode = 0;
#define FLASH_SPEED 80
int _flashPositions[3] = { 0, FLASH_SPEED / 2, FLASH_SPEED };

// WiFi credentials
const char* _ssid = "monavezazizi";
const char* _password = "curlyrosebud666";

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
byte _cardEMPTY[4] = { 0, 0, 0, 0 };

// Used to track time
unsigned long _time;
unsigned long _lastCardPoll;
bool _cardPresent = false;
bool _halfPollRepeat = false;

// Server-assigned position, but retained if we reboot so we don't have to re-setup mid-escape
int serverId = 0;

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

  // Server-assigned EEPROM-stored id
  EEPROM.begin(16); // We only use one byte; 16 is generous thinking ahead!
  serverId = EEPROM.read(0);
  Serial.print("Server-assigned ID at startup: ");
  Serial.println(serverId);
}


// ========== MAIN LOOP ==========
// Wait until a card is read, then send the card data to the server
void loop() {
  _time = millis();
  if(_time - _lastCardPoll > 400) { // wait 400ms before reading after a successful change in state
    if (_cardPresent) {
      checkForNoCard();
    }
    // Don't bother looking for a new RFID if the old one hasn't been removed after a full-success
    if (_flashingMode == 0) {
      pollForCard();
    }
    _lastCardPoll = _time - 100; // Poll about every 300ms
  }
  
  delay(2);
  if (_lightValuesAnim) {
    lightFlicker(_numLights);
  }
}

void checkForNoCard() {
  byte buffer[4];
  byte bufferSize = 4;
  if (_rfid.PICC_WakeupA(buffer, &bufferSize) == MFRC522::STATUS_OK) {
    _rfid.PICC_HaltA();
    return;
  }

  _cardPresent = false;
  setAllLights(0, 0, 0, true);

  // Send WiFi data
  sendCardData(_cardEMPTY, true, true);
  _lastCardPoll = _time;
}

void pollForCard() {
  // Look for new cards
  if (!_rfid.PICC_IsNewCardPresent()) {
    // If card present, refresh (poll server) for victory condition, but half as often
    if (_cardPresent) {
      if (_halfPollRepeat) {
        sendCardData(_cardNUID, false, true);
      }
      _halfPollRepeat = !_halfPollRepeat;
    }
    return;
  }

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

  // Send WiFi data
  sendCardData(_cardNUID, false, false);
  _lastCardPoll = _time;
}



// Send card data to the server
// Print out a debug message on serial port
void sendCardData(byte NUID[4], bool ignoreResponse, bool ignoreError) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error in WiFi connection");
    // Flash lights so we know there's an error
    if (!ignoreError) {
      doPanic(_numLights);
    }
    return;
  }
  
  Serial.println("Sending card data via WiFi");
  HTTPClient http;

  http.begin(serverIpAddress, serverPort, "/EspApi/PutCardData/");
  http.addHeader("Content-Type", "application/json");

  char buffer[256];
  
  // Data is sent in JSON format and  parsed by the server into object field data
  sprintf(buffer, "{senderID: \"Sender ID\", cardData: [%d, %d, %d, %d], senderMacAddr:\"NOTSET\"}", NUID[0], NUID[1], NUID[2], NUID[3]);
  String message = String("{senderID: ") + 
    serverId +
    String(", cardData: [") +
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
    http.end();
    Serial.println("Server response:");
    Serial.println(httpResponseCode);
    Serial.println(response);
  
    if (!ignoreResponse) {
      setEachLight(response, _numLights);
    }
  } else {
    http.end();
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
    // Flash lights so we know there's an error
    if (!ignoreError) {
      doPanic(_numLights);
    }
  }
}

void flashLights(int duration) {
  setAllLights(0, 0, 0, false);
  delay(duration);
  setAllLights(255, 255, 255, false);
  delay(duration);
}

void setAllLights(byte red, byte green, byte blue, bool smooth) {
  for(int k = 0; k < _numLights; k++) {
    if (!smooth) {
      _lights.setPixelColor(k, red, green, blue);
      _lightValues[k*3] = red;
      _lightValues[k*3+1] = green;
      _lightValues[k*3+2] = blue;
    }
    _lightBases[k*3] = red;
    _lightBases[k*3+1] = green;
    _lightBases[k*3+2] = blue;
    _lightTargets[k*3] = red;
    _lightTargets[k*3+1] = green;
    _lightTargets[k*3+2] = blue;
  }
  
  _lights.show();
  _lightValuesAnim = smooth;
  _flashingMode = 0;
}

// Indicate an unintended state has been reached.
void doPanic(int numLights) {
  for(int k = 0; k < 10; k++) {
    flashLights(50);
    for (int i = 0; i < numLights; i ++) {
      setAllLights(255, 0, 0, false);
      delay(20);
    }
  }

  setAllLights(0, 0, 0, false);
}


// Accepts a string containing hex digits.
// Splits the string into sets of 6 digits each,
// and treats each as a color to assign to a different light.
// If present, another digit represents mode to flash lights indicating some sort of success.
// If present, a final set of 2 digits is used to assign serverId.
void setEachLight(String str, int numLights) {
  // Make sure the string is long enough to address every light
  if(6 * (numLights) > str.length()) {
    doPanic(_numLights);
    Serial.print("String length too short.");
    Serial.println(str.length());
  }
  
  char buffer[8];
  for(int i = 0; i < numLights * 3; i++) {
    
    int startIndex = i * 2;
    unsigned long color;
  
    String colorStr = str.substring(startIndex, startIndex + 2);
    
    colorStr.toCharArray(buffer,8);    
    color = strtoul(buffer, NULL, 16);

    if (color != _lightBases[i]) {
      _lightBases[i] = color;
      _lightTargets[i] = color;
    }
  }
  _lightValuesAnim = true;
  lightFlicker(numLights);

  int flashModeIndex = numLights * 6;
  if (str.length() > flashModeIndex) {
    String modeStr = str.substring(flashModeIndex, flashModeIndex + 1);
    modeStr.toCharArray(buffer,8);    
    unsigned long newMode = strtoul(buffer, NULL, 16);
    _flashingMode = newMode;
  }
  
  int serverIdIndex = numLights * 6 + 1;
  if (str.length() > serverIdIndex + 1) {
    String idStr = str.substring(serverIdIndex, serverIdIndex + 2);
    idStr.toCharArray(buffer,8);    
    unsigned long newId = strtoul(buffer, NULL, 16);
    if (newId != serverId) {
      Serial.print("Setting server ID to: ");
      Serial.println(newId);
      serverId = newId;
      EEPROM.write(0, serverId);
      if (EEPROM.commit()) {
        Serial.println("EEPROM successfully committed");
      } else {
        Serial.println("ERROR! EEPROM commit failed");
      }
    }
  }
}

void lightFlicker(int numLights) {
  for (int i = 0; i < numLights * 3; i += 3) {
    bool done = true;
    for (int j = i; j < i + 3; ++j) {
      if (_lightValues[j] < _lightTargets[j]) {
        ++_lightValues[j];
        done = false;
      }
      else if (_lightValues[j] > _lightTargets[j]) {
        --_lightValues[j];
        done = false;
      }
    }
    if (done) {
      // range of target- 25% to 100%
      int range = random(25, 100);
      for (int j = i; j < i + 3; ++j) {
        _lightTargets[j] = _lightBases[j] * range / 100;
      }
    }
  }

  int _flashPercent = 0;
  if (_flashingMode > 0) {
    // Flashing mode 1: Rotate white light 50% 100% 50% around the circle
    for (int x = 0; x < 3; ++x) {
      ++_flashPositions[x];
      if (_flashPositions[x] >= numLights * FLASH_SPEED) {
        _flashPositions[x] = 0;
      }
    }
    // Flashing mode 2: All lights pulse 0 to 100 and back to 0
    if (_flashingMode == 2) {
      // Just convert the first position to a range of -100 thru 100 and absolute value it
      _flashPercent = _flashPositions[0] * 200 / (numLights * FLASH_SPEED);
      _flashPercent = _flashPercent - 100;
      if (_flashPercent < 0) _flashPercent = -_flashPercent;
    }
  }
  for(int i = 0; i < numLights; i++) {
    // Currently: mode 1 flashes to white, mode 2 to a dark red
    int flashTowardsA = _flashingMode == 1 ? 255 : 128;
    int flashTowardsB = _flashingMode == 1 ? 255 : 0;
    
    if (_flashingMode == 1 && i == (_flashPositions[1] / FLASH_SPEED)) {
      _lights.setPixelColor(i, flashTowardsA, flashTowardsB, flashTowardsB);
    }
    else if (_flashingMode == 1 && (i == (_flashPositions[0] / FLASH_SPEED) || i == (_flashPositions[2] / FLASH_SPEED))) {
      _lights.setPixelColor(i, 
        (_lightValues[i * 3] + flashTowardsA) / 2,
        (_lightValues[i * 3 + 1] + flashTowardsB) / 2,
        (_lightValues[i * 3 + 2] + flashTowardsB) / 2
        );
    }
    else if (_flashingMode == 2) {
      _lights.setPixelColor(i, 
        _lightValues[i * 3] + (flashTowardsA - _lightValues[i * 3]) * _flashPercent / 100,
        _lightValues[i * 3 + 1] + (flashTowardsB - _lightValues[i * 3 + 1]) * _flashPercent / 100,
        _lightValues[i * 3 + 2] + (flashTowardsB - _lightValues[i * 3 + 2]) * _flashPercent / 100
        );
    }
    else {
      _lights.setPixelColor(i, _lightValues[i * 3], _lightValues[i * 3 + 1], _lightValues[i * 3 + 2]);
    }
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
    _lightValues[k*3] = 0;
    _lightValues[k*3+1] = 0;
    _lightValues[k*3+2] = 0;
    _lightBases[k*3] = 0;
    _lightBases[k*3+1] = 0;
    _lightBases[k*3+2] = 0;
    _lightTargets[k*3] = 0;
    _lightTargets[k*3+1] = 0;
    _lightTargets[k*3+2] = 0;
    _lights.setPixelColor(k, 0, 0, 0);
	  _lights.show();
    delay(50);
  }
  
  _lightValuesAnim = false;
  _flashingMode = 0;
}
