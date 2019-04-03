/*
MIDI Pitchbend (full resolution)
By Amanda Ghassaei
July 2012
https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

*/
 

 
 int noteON = 144;//144 = 10010000 in binary, note on command
 //noteON data storage:
 int note = 60;//middle c
 int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //(higher velocity usually makes MIDI instruments louder)
 
 int pitchbend = 224;//224 = 11100000 in binary, pitchbend command
 //pitchbend data storage:
 int pitchbendVal = 8192;//value between 0 and 16383. zero pitchbend = 8192

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(31250);
}

void loop() {
  //first play note w/o pitchbend
  pitchbendVal = 8192;//reset pitchbend to "0" (zero pitchbend is pitchbendVal = 8192)
  MIDImessage(pitchbend, (pitchbendVal&127), (pitchbendVal>>7));//send pitchbend message
  MIDImessage(noteON, note, velocity);//turn note on
  delay(700);//sustain note
  MIDImessage(noteON, note, 0);//turn note off
  delay(500);//wait 500ms until triggering next note
  
  //then play with pitchbend up
  pitchbendVal = 8192;//reset pitchbend to "0" (zero pitchbend is pitchbendVal = 8192)
  MIDImessage(pitchbend, (pitchbendVal&127), (pitchbendVal>>7));//send pitchbend message
  MIDImessage(noteON, note, velocity);//turn note on
  for (pitchbendVal=8192;pitchbendVal<16384;pitchbendVal++){//increase pitchbend from 8192 to 16383
    MIDImessage(pitchbend, (pitchbendVal&127), (pitchbendVal>>7));//send pitchbend message
    delay(1);
  }
  MIDImessage(noteON, note, 0);//turn note off
  delay(500);//wait 500ms until triggering next note
  
  //then play with pitchbend down
  pitchbendVal = 8192;//reset pitchbend to "0" (zero pitchbend is pitchbendVal = 8192)
  MIDImessage(pitchbend, (pitchbendVal&127), (pitchbendVal>>7));//send pitchbend message
  MIDImessage(noteON, note, velocity);//turn note on
  for (pitchbendVal=8192;pitchbendVal>=0;pitchbendVal--){//decrease pitchbend 8192 to 0;
    MIDImessage(pitchbend, (pitchbendVal&127), (pitchbendVal>>7));//send pitchbend message
    delay(1);
  }
  MIDImessage(noteON, note, 0);//turn note off
  delay(500);//wait 500ms until triggering next note
}

//send MIDI message
void MIDImessage(int command, int data1, int data2) {
  Serial.write(command);//send command byte
  Serial.write(data1);//send data byte #1
  Serial.write(data2);//send data byte #2
}
