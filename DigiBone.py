from SynthClass import *
import serial, time

#Setup the Fuildsynth player, then connect to the Arduino.
#Arduino will send comands  (based on its reads from the
#slide, volume trigger, octave buttons, range buttons
#and sound on/off toggle).
#Pi to process

Trombone = TromSynth()

def serial_data(port, baudrate):
    ser = serial.Serial(port, baudrate)

    while True:
        yield ser.readline()

    ser.close()

for line in serial_data('/dev/ttyACM0',9600):
    if line == 'C':
        Trombone.NoteTest(60)
        break
    elif line == 'D':
        Trombone.NoteTest(62)
        break
    elif line == 'E':
        Trombone.NoteTest(64)
        break
    elif line == 'F':
        Trombone.NoteTest(65)
        break
    elif line == 'G':
        Trombone.NoteTest(67)
        break
    elif line == 'A':
        Trombone.NoteTest(69)
        break
    elif line == 'B':
        Trombone.NoteTest(71)
        break
    elif line == 'PlayExample':
        Trombone.PlayExample()
    else:
        print 'Not Recognised'


Trombone.CloseSynth()
