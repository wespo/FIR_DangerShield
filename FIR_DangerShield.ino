/*
DSP Shield Mode FIR Load Example
Loads an FIR filter from the SD Card
*/

//order of library invocation matters here. All three are required to use the DSP Shield.
//This is inherent to the Arduino IDE.
#include <SPI.h>
#include <mailbox.h>
#include <DSPShield.h>

void setup() {
  DSPShield.init(); //initialize
  
  DSPShield.startLoopback(); //start shield audio loopback
  DSPShield.displayPrint("Danger Shield Demo");

  //Serial.begin(9600);
  pinMode(A0, INPUT); //filter center frequency
  pinMode(A1, INPUT); //filter width
  pinMode(A2, INPUT); //system volume
}

void loop() {
    int width = constrain(map(analogRead(A1),20,1000,100,10000),100,10000);
    int center = constrain(map(analogRead(A0),20,1000,20,20000),20,20000);
    int volume = constrain(map(analogRead(A2),20,1000,0,100),0,100);
    Serial.print("Center Frequency: ");
    Serial.print(center);
    Serial.print("\tWidth: ");
    Serial.print(2*width);
    Serial.print("\tVolume: ");
    Serial.print(volume);
    Serial.println();
  
    /*load a band stop filter from the SD Card. 
    
    For a band / notch filter:
    DSPShield.setFIRFilter(channel to use, filter type, filter order, filter cutoff1, filter cutoff2);
    For a low / high filter:
    DSPShield.setFIRFilter(channel to use, filter type, filter order, filter cutoff);
    
    channel options:
    channels are CHAN_LEFT, CHAN_RIGHT, CHAN_BOTH
    
    type options:
    LOW_PASS, HIGH_PASS, BAND_PASS, BAND_STOP
    
    order options:
    tap settings are 41, 101, 201, 511
    
    cutoff options:
    For filters with two cutoffs, use two parameters. For single paramter filters (LOW_PASS and HIGH_PASS) use one cutoff parameter
    Increments are 1Hz steps from 10Hz to 20000Hz
    
    */
    DSPShield.setFIRFilter(CHAN_BOTH, BAND_PASS, 201, constrain(center-width, 20, 20000), constrain(center+width,20,20000));
    //try 511 taps for a sharper notch but slower load times!
    DSPShield.setOutputVolume(volume);
}

