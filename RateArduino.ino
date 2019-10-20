//TODO: Update pin definitions
//300 LOC

//Inclusion of libraries that build up subfunctions or modules
#include <FastLED.h>
#include <Wire.h>
#include <EasyTransferI2C.h>


//Definitions for the LED Library
#define LED_PIN     4
#define NUM_LEDS    23
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

//Definitions and global variables
const int switch_pin = 7; 
      //change this !!!

const int languageSwitch = 5;
char language = 'e';

int state = 0;
long time1 = 0;
long time2 = 0;
int flip = 0;
int sound = 8;

int waitCounter = 0;
int tempArr[100];
int gc = 0;

bool notPlayed = true;

//Setup for Microcontroller-microcontroller communication
EasyTransferI2C ET;
EasyTransferI2C ET2;

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float readOut;
};

struct SEND_DATA_STRUCTURE{
  char message[6];
};

RECEIVE_DATA_STRUCTURE mydata;
SEND_DATA_STRUCTURE mydata2;



// Setup of microcontroller
void setup() {
  //LED Setup
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  for(int led = 0; led< 23; led++){
    leds[led] = CRGB::Black;
  }
  FastLED.show();
  //Language Setup
  if(digitalRead(languageSwitch) == LOW){
    language = 's';
  }
  //MC-MC communications
  Wire.begin(9);        
  Serial.begin(9600); 
  ET.begin(details(mydata), &Wire);
  ET2.begin(details(mydata2), &Wire);
  Wire.onReceive(receive);
  //Sets up Pins
  pinMode(switch_pin, INPUT_PULLUP);
  pinMode(languageSwitch, INPUT_PULLUP);
  //pinMode(continue_button, INPUT_PULLUP);
  //Plays intro
  updateSound(-30);
        //delay time to play intro instructions
  delay(10000);

  Serial.println("Done Waiting");
}

// Function that allows for Rate LED to be updated.
void updateLED(){
  Serial.println("UPDATING");
  long looptime = time2-time1;
  Serial.println(looptime);
  float temp = 1/(float)looptime;
  int numLeds = (temp*8800);
  Serial.println("NUM LEDS = ");
  Serial.print(numLeds);
  for(int led = 0; led< 23; led++){
    leds[led] = CRGB::Black;
  }
  if(numLeds > 23){
    numLeds = 23;
  }
  for(int led = 0; led < numLeds; led++){
    leds[led] = CRGB::Red;
    if(led >=  8){
      leds[led] = CRGB::Orange;
    }
    if(led >= 12){
      leds[led] = CRGB::Green;
    }
    if(led >= 19){
      leds[led] = CRGB::Orange;
    }
    if(led >= 24){
      leds[led] = CRGB::Red;
    }
  }
 FastLED.show(); 
}

// Simple function that finds the largest number in an Array
int largest(int arr[], int n) { 
    int i; 
     
    // Initialize maximum element 
    int max = arr[0]; 
  
    // Traverse array elements from second and 
    // compare every element with current max   
    for (i = 1; i < n; i++) 
        if (arr[i] > max) 
            max = arr[i]; 
  
    return max; 
} 

// Function that allows transmission of various sound commands
void updateSound(int maxval){
  //Serial.println("??????");
  //Serial.println(maxval);
  //Serial.println("??????");
  if(maxval == -30){
    strcpy(mydata2.message, "s.wav");
    ET2.sendData(8);
    return;
  }
  if(maxval == -77){
    strcpy(mydata2.message, "b.wav");
    ET2.sendData(8);
    return;
  }
  if(maxval < 14){
     strcpy(mydata2.message, "l.wav");
     ET2.sendData(8);
  }
  if(maxval > 20){
    strcpy(mydata2.message, "h.wav");
    ET2.sendData(8);
  }
  

}


// Function that handles rescue breaths
void breathCount(){
  Serial.println("here");
   for(int led = 0; led< 8; led++){
    leds[led] = CRGB::Red;
  }
  for(int led = 7; led< 23; led++){
    leds[led] = CRGB::White;
  }
  for(int led = 23; led >0; led-=1){
    leds[led] = CRGB::Black;
    delay(45);
  }
  
}

// Major loop that controls the microcontroller
void loop() { 
 while(gc < 30){
 Serial.println(gc);
 if(ET.receiveData()){
   float val = mydata.readOut;
   switch(state){
     case 0:
      if(val >10){
        state = 1;
      }
      break;
     case 1:
      if(waitCounter < 100){
        //Serial.println("WC: ");
        //Serial.print(waitCounter);
        tempArr[waitCounter] = (int)val;
        waitCounter += 1;
      }
      if(waitCounter>100){
        memset(tempArr, 0, sizeof(tempArr));
      }
      //Serial.println("CASE 1");
      if(val < 10){
        state = 0;
        int n = sizeof(tempArr)/sizeof(tempArr[0]); 
        int maxval = largest(tempArr, n);
        int loop = 0;
        //Serial.println(" ");
        //for(loop = 0; loop < 100; loop++){
          //Serial.print(tempArr[loop]);
          //Serial.print(" ");
        //}
        
        updateSound(maxval);
        //gc += 1;
        memset(tempArr, 0, sizeof(tempArr));
        waitCounter = 0;
      if(flip == 0){
        time1 = millis();
        gc+=1;
        //Serial.println(time1);
        flip = 1;
      }else{
        time2 = millis();
        //Serial.println(time2);
        flip = 0;
        updateLED();
        gc+=1;
      }
      }
      break;
   }
  }
  delay(30);
  
 }

  //change this pin!!!
  const int pouchLed = 12;


 while(gc >= 30){
  if(notPlayed){
    updateSound(-77);
    notPlayed = false; }
    //digitalWrite to pouchLed pin
    analogWrite(pouchLed, 255);
    delay(100);
    analogWrite(pouchLed, 0);
    delay(100);
    analogWrite(pouchLed, 255);
    delay(100);
    analogWrite(pouchLed, 0);
    delay(100);
    analogWrite(pouchLed, 255);
    delay(100);
    analogWrite(pouchLed, 0);
    delay(100);
    analogWrite(pouchLed, 255);
    delay(100);
    analogWrite(pouchLed, 0);
    delay(100);
    analogWrite(pouchLed, 255);
    delay(100);
    analogWrite(pouchLed, 0);
    delay(100);
    //delay for the breath instructions
    delay(2000);
   //Serial.println("here-01");
   breathCount();
   gc+=1;
   if(gc == 32){
    gc = 0;
    notPlayed = true;
   }
 }
}


// Waste function that handles the interupt.
void receive(int numBytes) {}
