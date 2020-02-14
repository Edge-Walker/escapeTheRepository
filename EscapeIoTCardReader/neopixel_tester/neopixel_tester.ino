#include <Adafruit_NeoPixel.h>

// Settings
#define DATA_PIN       D1
#define NUMPIXELS      6
#define DELAY          1
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  pixels.begin(); // Initialize NeoPixel library.
}

// Go through each pixel in turn, cycling through a variety of colors each time
void loop() { 
    for(int i = 0; i < NUMPIXELS; i++) {
    // Max red
    for(int k = 0; k < 255; k++) {
      pixels.setPixelColor(i, pixels.Color(k, 0, 0));
      pixels.show();
      delay(DELAY);
    }

    // Move to yellow
    for(int k = 0; k < 255; k++) {
      pixels.setPixelColor(i, pixels.Color(255-k, k/2, 0));
      pixels.show();
      delay(DELAY);
    }

    // Max green
    for(int k = 128; k < 255; k++) {
      pixels.setPixelColor(i, pixels.Color(0, k, 0));
      pixels.show();
      delay(DELAY);
    }

    // Move to cyan
    for(int k = 0; k < 255; k++) {
      pixels.setPixelColor(i, pixels.Color(0, 255-k, k/2));
      pixels.show();
      delay(DELAY);
    }

    // Max blue
    for(int k = 128; k < 255; k++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, k));
      pixels.show();
      delay(DELAY);
    }
    
    // Move to magenta
    for(int k = 0; k < 255; k++) {
      pixels.setPixelColor(i, pixels.Color(k/2, 0, 255-k));
      pixels.show();
      delay(DELAY);
    }

    // Go to black
    for(int k = 0; k < 128; k++) {
      pixels.setPixelColor(i, pixels.Color(127-k, 0, 0));
      pixels.show();
      delay(DELAY);
    }    
  }
}
