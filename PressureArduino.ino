//Inclusion of libraries that build up subfunctions or modules
#include "HX711.h"
#include <Wire.h>
#include <FastLED.h>
#include <EasyTransferI2C.h>

//Definitions for the LED Library
#define LED_PIN     6
#define NUM_LEDS    23
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

//Definitions and global variables
const int DT = 5;
const int SLCK = 4;
//-22500 To be accurate
float calibration_factor = -7050;

//Creation of objects used
HX711 scale;
EasyTransferI2C ET;

//Setup up for MC-MC communications
struct SEND_DATA_STRUCTURE{
  float readOut;
};
SEND_DATA_STRUCTURE mydata;

// Setup of microcontroller
void setup() {
  delay(3000);
  Serial.begin(9600);
  Serial.println("Starting LEDs");
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  Serial.println("Get everyting off the scale");
  
  scale.begin(DT, SLCK);
  scale.set_scale();
  scale.tare();

  long zero = scale.read_average();
  scale.set_scale(calibration_factor);

  Wire.begin(10);
  ET.begin(details(mydata), &Wire);

  Serial.println("MADE IT");
}

// Major loop that controls the microcontroller
void loop() {
  float scale_readOut = scale.get_units();
  Serial.println(scale_readOut);
  mydata.readOut = scale_readOut;
  Serial.println(mydata.readOut);
  ET.sendData(9);
  int numLeds = scale_readOut;
  if(numLeds > 23){
    numLeds = 23;
  }
  for(int led = 0; led< 23; led++){
    leds[led] = CRGB::Black;
  }
  for(int led = 0; led < numLeds; led++){
    leds[led] = CRGB::Red;
    if(led >  12){
      leds[led] = CRGB::Orange;
    }
    if(led >= 17){
      leds[led] = CRGB::Green;
    }
    if(led > 20){
      leds[led] = CRGB::Red;
    }
  }
 FastLED.show(); 
  delay(20);

}
