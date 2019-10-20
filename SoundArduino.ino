//Inclusion of libraries that build up subfunctions or modules
#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"
#include <Wire.h>
#include <EasyTransferI2C.h>

//Definitions, global variables, and creation of objects
#define SD_ChipSelectPin 10
TMRpcm tmrpcm;
EasyTransferI2C ET;

//Setup for Microcontroller-microcontroller communication
struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char message[6];
};
RECEIVE_DATA_STRUCTURE mydata;

// Setup of microcontroller
void setup(){
tmrpcm.speakerPin = 9;
pinMode(9, OUTPUT);
Serial.begin(9600);
Wire.begin(8);
ET.begin(details(mydata), &Wire);
Wire.onReceive(receiveEvent);

//Failure handling
if (!SD.begin(SD_ChipSelectPin)) {
  Serial.println("SD fail");
  return;
}

}

// Major loop that controls the microcontroller
void loop(){ 
  if(ET.receiveData()){
    Serial.println(mydata.message);
    tmrpcm.play(mydata.message);
  }

}

// Waste function that handles the interupt.
void receiveEvent(int howMany) {
}
