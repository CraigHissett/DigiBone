from SynthClass import *

#Setup the Fuildsynth player, then connect to the Arduino.
#Arduino will send comands  (based on its reads from the
#slide, volume trigger, octave buttons, range buttons
#and sound on/off toggle).
#Pi to process


Trombone = TromSynth()

Trombone.PlayExample()

Trombone.CloseSynth()


