/*
  Melody.cpp - Library for playing melody.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Melody.h"

Melody::Melody(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Melody::play_melody(int my_melody[], int sizeofmelody)
{

  // change this to make the song slower or faster
  int tempo = 140;
  
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  //int notes = sizeof(my_melody) / sizeof(my_melody[0]) / 2;
  int notes = sizeofmelody / sizeof(my_melody[0]) / 2;

  
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = my_melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(_pin, my_melody[thisNote], noteDuration * 0.9);
    //Serial.println("_pin:" + String(_pin) + ", thisNote:" + String(melody[thisNote]) + ", duration:" + String(noteDuration));

    // Wait for the specified duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(_pin);
  }

}


