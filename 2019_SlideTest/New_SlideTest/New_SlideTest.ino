#include <NewPing.h>

#include <midi_serialization.h>
#include <usbmidi.h>



const int buttonPin = 9;
const int ledPin = 13;
const int STrigger = 8;
const int SEcho = 7;

NewPing Slide(STrigger, SEcho, 30);

int SlideLength = 0;
int LastSlideLength = 0;

void sendCC(uint8_t channel, uint8_t control, uint8_t value) {
  USBMIDI.write(0xB0 | (channel & 0xf));
  USBMIDI.write(control & 0x7f);
  USBMIDI.write(value & 0x7f);
}

void sendNote(uint8_t channel, uint8_t note, uint8_t velocity) {
  USBMIDI.write((velocity != 0 ? 0x90 : 0x80) | (channel & 0xf));
  USBMIDI.write(note & 0x7f);
  USBMIDI.write(velocity &0x7f);
}

void pitchBendChange(byte channel, int value) {
  //byte lowValue = value & 0x7F;
  //byte highValue = value >> 7;
  //MIDIEvent event = {0x0E, 0xE0 | channel, lowValue, highValue};
  //MIDIUSB.write(event);
  //USBMIDI.write(event);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  digitalWrite(ledPin, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(buttonPin)==HIGH)
  {
    digitalWrite(ledPin, HIGH);
    SlideLength = Slide.ping_cm();
    //Serial.println(SlideLength);
    if(SlideLength != LastSlideLength)
    {
      //New slide position detected - cancel current note, send new one
      sendNote(0, LastSlideLength, 0);
      sendNote(0, SlideLength+30, 120);
    }
    //else note the same, so don't change anything
    LastSlideLength = SlideLength;
  }
  else
  {
  sendNote(0, SlideLength+30, 0);
  digitalWrite(ledPin, LOW);
  delay(100);
  USBMIDI.flush();
  }
}
