
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <SoftwareSerial.h>

// number of pixels in the LED string
#define NUMPIXELS (15)

// number of seconds thet the timer will run
#define TIMEOUT (15)
#define PIN 4

SoftwareSerial mySerial(3, 2); // pin 2 = TX, pin 3 = RX (unused)
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int cycle = 50;

int d0Pin = 5;
int d1Pin = 6;
int d2Pin = 7;
int d3Pin = 8;
int d4Pin = 9;
int d5Pin = 10;
int d6Pin = 11;
int d7Pin = 12;

void setup() {
  initDisplay();
  strip.begin();
  lightsOff();
  //Left turn pin
  pinMode(d0Pin, INPUT_PULLUP);
  pinMode(d1Pin, INPUT_PULLUP);
  pinMode(d2Pin, INPUT_PULLUP);
  pinMode(d3Pin, INPUT_PULLUP);
  pinMode(d4Pin, INPUT_PULLUP);
  pinMode(d5Pin, INPUT_PULLUP);
  pinMode(d6Pin, INPUT_PULLUP);
  pinMode(d7Pin, INPUT_PULLUP);

  //Serial Trouble Shooting
  Serial.begin(9600);
}

void loop() {
  int d0Binary = digitalRead(d0Pin);
  int d1Binary = digitalRead(d1Pin);
  int d2Binary = digitalRead(d2Pin);
  int d3Binary = digitalRead(d3Pin);
  int d4Binary = digitalRead(d4Pin);
  int d5Binary = digitalRead(d5Pin);
  int d6Binary = digitalRead(d6Pin);
  int d7Binary = digitalRead(d7Pin);
  int binaryNumber = 128 * d7Binary + 64 * d6Binary + 32 * d5Binary + 16 * d4Binary + 8 * d3Binary + 4 * d2Binary + 2 * d1Binary + d0Binary;
  //------------------------FIRST TWO MINUTES------------------------
  int startPixels = NUMPIXELS - 1;
  float startPixelsVal = 0;
  setLEDsToColor( NUMPIXELS - 1, 0, 255, 0);
  for (int countdownSeconds = TIMEOUT + 120; countdownSeconds > 15; countdownSeconds--) {
    timeDisplay(countdownSeconds);
    if (startPixelsVal == 8) {
      startPixelsVal = 0;
      startPixels--;
    }
    setLEDsToColor( startPixels, 0, 127, 127);
    Serial.print("startPixelsVal= ");
    Serial.println(startPixelsVal);
    Serial.println(startPixels);
    delay(1000);
    startPixelsVal++;
  }


  //------------------------LAST FIFTEEN SECONDS------------------------

  int litPixels = NUMPIXELS - 1;
  setLEDsToColor( NUMPIXELS - 1, 0, 255, 0);
  for (int countdownSeconds = TIMEOUT; countdownSeconds > -1; countdownSeconds--) {
    timeDisplay(countdownSeconds);
    setLEDsToColor( litPixels, 255, 0, 0);
    litPixels--;
    delay(1000);
  }
  while (1) {}
  }
  

void left_trn() {
  int left_len = 25;
  for (uint16_t i = 0; i < left_len ; i++) {
    strip.setPixelColor((24 - i), 0, 255, 0);
    strip.setPixelColor((49 - i), 0, 255, 0);
    strip.show();
    delay(cycle / 3);
  }
  for (uint16_t i = 0; i < left_len ; i++) {
    strip.setPixelColor((24 - i), 0, 0, 0);
    strip.setPixelColor((49 - i), 0, 0, 0);
    //strip.setPixelColor((29-i),0,0,0);
    strip.show();
    delay(cycle / 3);
  }
}

void right_trn() {

  int left_len = 25;

  Serial.println("rightTurn");

  for (uint16_t i = 0; i < (left_len) ; i++) {
    //strip.setPixelColor((14-i),0,0,0);
    strip.setPixelColor((i + 25), 0, 255, 0);
    strip.setPixelColor((i + 0), 0, 255, 0);
    strip.show();
    delay(cycle / 3);
  }
  for (uint16_t i = 0; i < (left_len) ; i++) {
    strip.setPixelColor((i + 25), 0, 0, 0);
    strip.setPixelColor((i + 0), 0, 0, 0);
    strip.show();
    delay(cycle / 3);
  }

}


//Bad Ccode: uses "magic numbers"


void idle() {
  rainbow(20);
  Serial.println("IDLE");
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels() ; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);

  }
}
//---------------LED routine----------------------
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    Serial.println("rainbow");
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Bad Ccode: uses "magic numbers"
void party() {
  int party_len = 50;
  for (int i = 0; i < 175 ; i++) {
    if (i >= 0 and i < 50) {
      strip.setPixelColor(i, 0, 0, 255);
    }
    if (i >= 25 and i < 75) {
      strip.setPixelColor((i - 25), 255, 0, 255);
    }
    if (i >= 75 and i < 125) {
      strip.setPixelColor((i - 75), 255, 0, 0);
    }
    if (i >= 125 and i < 175) {
      strip.setPixelColor((i - 125), 255, 60, 0);
    }
    strip.show();
  }
}

void lightsOff() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}
void hazard() {
  int hazard_len = 250;

  for (uint16_t i = 0; i < 50 ; i++) {
    strip.setPixelColor(i, 255, 0, 0);
    //strip.setPixelColor((15+i),0,0,0);
    //strip.setPixelColor((29-i),0,255,0);
  }

  strip.show();
  delay(hazard_len);

  for (uint16_t i = 0; i < 50 ; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    //strip.setPixelColor((29-i),0,0,0);
  }

  strip.show();
  delay(hazard_len);

}
void brakes() {

  int left_len = 25;
  int right_len  = 25;


  for (uint16_t i = 0; i < left_len ; i++) {
    strip.setPixelColor(i, 0, 255, 0);
    strip.setPixelColor((49 - i), 0, 255, 0);
    strip.show();
    delay(cycle / 1.7);
  }
}

void setLEDsToColor( int number, int red, int green, int blue) {
  for (int i = 0; i <= number; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  for (int i = number + 1; i <= NUMPIXELS - 1; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

//--------------------------DISPLAY--------------------------------------------------------------
// display current time on the display.
void timeDisplay(int cndSeconds) {

  mySerial.write(0xFE);
  mySerial.write(0X01);
  mySerial.write(0xFE); // move cursor to beginning of first line
  mySerial.write(0x80);

  if (cndSeconds < 10) {
    mySerial.print(" ");
  }
  if (cndSeconds < 100) {
    mySerial.print(" ");
  }

  mySerial.print(cndSeconds);
}

void  initDisplay() {
  mySerial.begin(9600); // set up serial port for 9600 baud
  delay(500); // Wait for display to boot up.

  mySerial.write(254); // move cursor to beginning of first line
  mySerial.write(128);

  mySerial.write(0xFE);
  mySerial.write(0X01);

  mySerial.write(254); // move cursor to beginning of first line
  mySerial.write(128);
}

