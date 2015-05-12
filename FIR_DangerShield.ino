/*
DSP Shield Mode FIR Load Example
Loads an FIR filter from the SD Card
*/

//order of library invocation matters here. All three are required to use the DSP Shield.
//This is inherent to the Arduino IDE.
#include <SPI.h>
#include <mailbox.h>
#include <DSPShield.h>

int count;
int filter;
int width_old;
int volume_old;
int center_old;

void setup() {
    DSPShield.init(); //initialize
    
    DSPShield.startLoopback(); //start shield audio loopback
  
    Serial.begin(9600);
    pinMode(A0, INPUT); //filter center frequency
    pinMode(A1, INPUT); //filter width
    pinMode(A2, INPUT); //system volume
    filter = -2;
    DSPShield.setOutputVolume(40);
}

void loop() {
  
    if (filter == -2) {
        Serial.println("Select a Filter");
        Serial.println("0: Low Pass Filter");
        Serial.println("1: High Pass Filter");
        Serial.println("2: Band Pass Filter");
        Serial.println("3: Band Stop Filter");
        Serial.println("then click [send] or press [return]");
        Serial.println();
        
        filter = -1;
    }
    
    while (Serial.available() > 0) {
       filter = Serial.parseInt();
       filter = constrain(filter, 0, 3);
       Serial.print("You have selected filter #");
       Serial.print(filter);
       switch (filter) {
         case 0:
           Serial.println(": low pass");
           break;
         case 1:
           Serial.println(": high pass");
           break;
         case 2:
           Serial.println(": band pass");
           break;
         case 3:
           Serial.println(": band stop");
           break;  
       }
       
    }
    
    switch (filter) {
      case 0:
        low_pass();
        break;
      case 1:
        high_pass();
        break;
      case 2:
        band_pass();
        break;
      case 3:
        band_stop();
        break;
    }

}



void band_pass() {
    int width = constrain(map(analogRead(A1),20,1000,100,10000),100,10000);
    int center = constrain(map(analogRead(A0),20,1000,10,16000),10,16000);
    int volume = constrain(map(analogRead(A2),20,1000,0,100),0,100);
    
    if (abs(width - width_old) > 20 || volume != volume_old || (center < 100 && center != center_old) || (center > 100 && abs(center_old - center) > 30)) {
        Serial.print("Band pass filter:\t");
        Serial.print("Center Frequency: ");
        Serial.print(center);
        Serial.print("\tWidth: ");
        Serial.print(2*width);
        Serial.print("\tVolume: ");
        Serial.print(volume);
        Serial.println();
    }

  
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
    
    
    center_old = center;
    volume_old = volume;
    width_old = width;
    
    DSPShield.setFIRFilter(CHAN_BOTH, BAND_PASS, 201, constrain(center-width, 10, 20000), constrain(center+width,20,20000));
    //try 511 taps for a sharper notch but slower load times! 
    DSPShield.setOutputVolume(volume);
}


void band_stop() {
    int width = constrain(map(analogRead(A1),20,1000,100,10000),100,10000);
    int center = constrain(map(analogRead(A0),20,1000,10,16000),10,16000);
    int volume = constrain(map(analogRead(A2),20,1000,0,100),0,100);
    
    if (abs(width - width_old) > 20 || volume != volume_old || (center < 100 && center != center_old) || (center > 100 && abs(center_old - center) > 30)) {
        Serial.print("Band stop filter:\t");
        Serial.print("Center Frequency: ");
        Serial.print(center);
        Serial.print("\tWidth: ");
        Serial.print(2*width);
        Serial.print("\tVolume: ");
        Serial.print(volume);
        Serial.println();    
    }
    center_old = center;
    volume_old = volume;
    width_old = width;
    
    DSPShield.setFIRFilter(CHAN_BOTH, BAND_STOP, 201, constrain(center-width, 10, 20000), constrain(center+width,20,20000));  
    DSPShield.setOutputVolume(volume);
}


void low_pass() {
    int center = constrain(map(analogRead(A0),20,1000,10,16000),10,16000);
    int volume = constrain(map(analogRead(A2),20,1000,0,100),0,100);
    
    if (volume != volume_old || (center < 100 && center != center_old) || (center > 100 && abs(center_old - center) > 30)) {
        Serial.print("Low pass filter:\t");
        Serial.print("Cutoff: ");
        Serial.print(center);
        Serial.print("\tVolume: ");
        Serial.print(volume);
        Serial.println();
    }
    center_old = center;
    volume_old = volume;

    DSPShield.setFIRFilter(CHAN_BOTH, LOW_PASS, 201, center);
    DSPShield.setOutputVolume(volume);
}


void high_pass() {
    int center = constrain(map(analogRead(A0),20,1000,10,16000),10,16000);
    int volume = constrain(map(analogRead(A2),20,1000,0,100),0,100);
    
    if (volume != volume_old || (center < 100 && center != center_old) || (center > 100 && abs(center_old - center) > 30)) {
        Serial.print("High pass filter:\t");
        Serial.print("Cutoff: ");
        Serial.print(center);
        Serial.print("\tVolume: ");
        Serial.print(volume);
        Serial.println();   
    }
    
    center_old = center;
    volume_old = volume;
    
    DSPShield.setFIRFilter(CHAN_BOTH, HIGH_PASS, 201, center);
    DSPShield.setOutputVolume(volume);
}
