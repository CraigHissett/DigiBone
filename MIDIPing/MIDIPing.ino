/*
 * Based on MIDIUSB_test.ino
 */ 

// =========================================================================== [PINS]

// Digital I/O
const int LED_PIN = 13;        // internal led 
const int TRIGGER_PIN = 8;    // ultrasonic sensor
const int ECHO_PIN = 9;        // ultrasonic sensor
const int GATE_PIN = 4;        // sound sensor (start/stop)
const int BUTTON_PIN = 5;      // Button for testing purposes - in use until DIP 6 air pressure sensor in play
// Analog Inputs
const int ENV_PIN = A5;        // sound sensor (envelope)
const int RANGE_PIN = A0;      // softpot

// =========================================================================== [CONSTANTS]
const int NUMBER_OF_MODES = 3;

const float MAX_SLIDE_DISTANCE = 72;
const int NUMBER_OF_POSITIONS = 7;

const int GATE_OFF = 0;
const int GATE_START = 1;
const int GATE_ON = 2;
const int GATE_END = 3;

// DISPLAY MODES:
const int BONE_MODE_MIDI = 1;
const int BONE_MODE_EXT_SYNTH = 2;
const int BONE_MODE_INT_SYNTH = 3;

// =========================================================================== [GLOBALS]
int prevReading = LOW;
long lastBounceTime = 0;
long debounceLimit = 40;
int currButtonState = LOW;
int prevButtonState = HIGH;
int mode = 1;

int lastGateStatus = LOW;
int currGateStatus = LOW;
bool noteisOn = false;
int pitch = 0x3C; // middle C
float amp = 0;

#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GATE_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(RANGE_PIN, HIGH); // pullup resitor for softpot

  delay(500);
  
  //controlChange(0, 10, 57); // Set the value of controller 10 on channel 0 to 57
}

void loop() {
  
  float cm = 0;
  int pos = 1;
  int harm = 2;
  int amp = 0;

   // ====================================================================== [Tracking Notes]
   switch(mode)
   {
       case BONE_MODE_MIDI:
       {
             switch(gateStatus())
            {
                case GATE_START:
                {
                    cm = trackSlide();
                    pos = slidePosition(cm); // 0 - 6
                    harm = harmonic();
                    pitch =  pitchNumber(pos, harm);
                    //sendMIDI(0x90, pitch, 0x40);
                    //noteOn(0, pitch, 64);
                    noteOn(0x90, pitch, 0x40);
                    MidiUSB.flush();
                    break;
                }
                
                case GATE_ON:
                {
                    // track envelope...
                    //amp = analogRead(ENV_PIN);
                    break;
                }
                
                case GATE_END:
                {
                    //sendMIDI(0x80, pitch, 0x00);
                    noteOn(0x80, pitch, 0x00);
                    MidiUSB.flush();
                    break;
                }
            }

            break;
       }
       
       case BONE_MODE_EXT_SYNTH:
       {
           // TBD 
           break;
       }
       
       case BONE_MODE_INT_SYNTH:
       {
           // TBD
            break;
       }
   }
  
   
} // END LOOP

 /* Serial.println("Sending note on");
  noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(1000);
  //Serial.println("Sending note off");
  noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(1500); */

  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65



  float trackSlide(void)
{
   // this should be set to the length of the slide
  const float SENSOR_MAX = 72.0;
  
  // minimum reliable distance (cm) measurement
  // from the ultrasonic sensor (HC-SR04)  
  const float SENSOR_MIN = 6.0;           

  long pulse = 0;
  float cm = 0;
  
  // Ping
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Pong
  pulse = pulseIn(ECHO_PIN, HIGH);
  // Calculate Distance
  cm = pulse / 29 / 2;
   // offset sensor's blind zone...
  cm -= SENSOR_MIN;
  
  // limit output to stay within the size of the slide
  if(cm < 0)  cm = 0;
  if(cm > SENSOR_MAX)  cm = SENSOR_MAX;
  
  return cm;
}

// ====================================================================== changeMode()
void changeMode(void)
{
 /*  
  *   
  // change to next mode...
      mode++;
      
  // if we reached the last mode...
  if(mode > NUMBER_OF_MODES)
       mode = 1;    // circle back to the first.
   
   // show changes...
   //updateDisplay(mode);
   */
   mode=1;
}

// ====================================================================== sendMIDI()
/*void sendMIDI(byte statusByte, byte data1, byte data2)
{
  Serial.write(statusByte);
  Serial.write(data1);
  Serial.write(data2);
}
*/
// ====================================================================== slidePosition()

// This function divides the slide in regions in order to define which position
// is currently in use. Each position is associated with an upper and an lower
// limit. THis should be used for determining pitch when running in MIDI mode.
// Each note is then a discrete elementm which can be reffered to by a MIDI
// note number.
int slidePosition(float distance)
{
    int i;
    float positions[NUMBER_OF_POSITIONS] = {5,11,19,30,42,52,63};
    
    if(distance > MAX_SLIDE_DISTANCE)
        return 0;
    
    for(i = 0; i < NUMBER_OF_POSITIONS; i++)
        if(distance <= positions[i])
            return i; // trombone positions here range from 0 to 6
    
    return 0;
}

// ====================================================================== harmonic()
// This function works with a value from the soft pot, used as an alternative
// to the pitch detection algorithm. The player must then slide  his/her finger
// along the pot to define which harmonic should be chosen.
int harmonic(void)
{
  int harm = 2;
  // Range values from softpot:
  // range array size
  const int MAX_RANGES = 9;      
  // the index to this array tells which harmonic
  int ranges[MAX_RANGES] = {113,226,339,452,565,678,791,904,1005};
  // The last range was hand tuned to the sensor output. When nothing is touching 
  // the pot,  it returns an oscilating value around 1012. So I shortened this last 
  // range and assigned anything higher to the first range, in order to have a more 
  // sensible default value when the pot is idle. 
  int range = analogRead(RANGE_PIN);
  
  if(range > 1005)
  {
      harm = 2; // default range
  }
  else
  {
      for(int i = 0; i < MAX_RANGES; i++)
    {
      if(range <= ranges[i])
      {
        harm = (i+2);
        break;
      }
    }
  }
  
  return harm;
}

// ====================================================================== pitchNumber()
int pitchNumber(int pos, int harm)
{ 
    int basePitch = 34; // Pedal Bb
    // interval from pedal base pitch to each harmonic (in semitones)
    // The intervals start with the second harmonic on position 0,
    // so, given the harmonic number, we need to subtract 2 from the 
    // harmonic to get the correct transposition
    int verticalInterval[9] = {12,19,24,28,31,34,36,38,40};
    return basePitch + (verticalInterval[harm-2]) - pos;
}

// ====================================================================== frequency()
// This function should be used to determine the final 
// frequency when playing in non-MIDI mode.
float frequency(float cm, float harm)
{
  const float FREQ_MIN = 58.27;        // trombone fundamental on 7th position
  const float FREQ_MAX = 41.2;       // trombone fundamental in 1st position
  const int SENSOR_MAX = 70;          // this should be set to the length of the slide
  const int SENSOR_MIN = 3;           // minimum reliable distance (cm) measurement from the ultrasonic sensor (HC-SR04)
  float freq = 0;
  
  // calculate fundamental frequency...
  //freq = cm * (FREQ_MIN - FREQ_MAX) / (SENSOR_MAX - SENSOR_MIN) + FREQ_MIN;
  freq = map(cm,FREQ_MIN,FREQ_MAX,SENSOR_MAX,FREQ_MIN);
  
  // before calculating final frequency, convert the harmonic from
  // 0-6 to 1-7 format
  freq *= (harm+1); 
  
  return freq;
}

// ====================================================================== gateStatus()
int gateStatus(void)
{
  int gate = GATE_OFF;
  
  currGateStatus = digitalRead(GATE_PIN);
  
  // Note Started
  if(currGateStatus == HIGH && lastGateStatus == LOW)
  {
      noteisOn = true;
      lastGateStatus = currGateStatus;
      return GATE_START;
  }
  
  // Note Ended
  if(currGateStatus == LOW && lastGateStatus == HIGH)
  {
       noteisOn = false;
       lastGateStatus = currGateStatus;
      return GATE_END;
  }
  
  // Note is currently activated
  if(noteisOn)
  {
    lastGateStatus = currGateStatus;  
    return  GATE_ON;
  }
  
  lastGateStatus = currGateStatus;
  return gate;
}


// DEBUGGING CODE LEFT OVERS...
void testMidiFromButton()
{
  if(!noteOn)
      {  
        // pick a random note in middle octave
        pitch = rand()%12 + 60;
        noteOn(0x90, pitch, 0x40);
        noteisOn = true;
      }
      else
      {
        noteOff(0x80, pitch, 0x00);  
        noteisOn = false;
      }
}
