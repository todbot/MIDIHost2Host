/* 
 *  MIDIHost2Host -- Connect two USB-MIDI host devices together
 *  2019 @todbot / Tod E. Kurt
 *  
 *  This sketch is meant to be installed on two Adafruit Trinket M0s.
 *  The connections between the two Trinket M0s are:
 *  - TrinketA Gnd --------------------- TrinketB Gnd
 *  - TrinketA pin 3  --- 1k resistor -- TrinketB pin 4
 *  - TrinketA pin 4  --- 1k resistor -- TrinketB pin 3
 *  
 *  When compiling:
 *  - Install libraries: Adafruit_TinyUSB & MIDI
 *  - Be sure to have updated all boards and libraries
 *  - Select "Tools" -> "USB Stack" -> "TinyUSB"
 *  
 *  .
 *  The following libraries are required:
 *  - Adafruit_TinyUSB library by Adafruit
 *    https://github.com/adafruit/Adafruit_TinyUSB_Arduino
 *  - MIDI Library by Forty Seven Effects
 *    https://github.com/FortySevenEffects/arduino_midi_library
 */

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create instance of Arduino MIDI library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, midiA);
// Create instance of Arduino MIDI library,
// and attach HardwareSerial Serial1 (TrinketM0 pins 3 & 4)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiB);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize MIDI, and listen to all MIDI channels
  // This will also call usb_midi's and Serial1's begin()
  midiA.begin(MIDI_CHANNEL_OMNI);
  midiB.begin(MIDI_CHANNEL_OMNI);

  midiA.turnThruOff();
  midiB.turnThruOff();

  Serial.begin(115200);

  // wait until device mounted
  while ( !USBDevice.mounted() ) delay(1);
}

void loop()
{

  // read any new MIDI messages
  if ( midiA.read() ) {
    midiB.send(midiA.getType(),
               midiA.getData1(),
               midiA.getData2(),
               midiA.getChannel());
//    Serial.println("midiA");
  }
  
  if ( midiB.read() ) {
    midiA.send(midiB.getType(),
               midiB.getData1(),
               midiB.getData2(),
               midiB.getChannel());
//    Serial.println("midiB");
  }

}
