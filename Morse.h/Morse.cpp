/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Morse.h"

Morse::Morse(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Morse::dot()
{
  //digitalWrite(_pin, HIGH);
  //delay(250);
  //digitalWrite(_pin, LOW);
  //delay(250);  
  tone(_pin, 300, 500);
  delay(250);
  noTone(_pin);
  delay(250);
}

void Morse::dash()
{
  //digitalWrite(_pin, HIGH);
  //delay(1000);
  //digitalWrite(_pin, LOW);
  //delay(250);
  tone(_pin, 198, 1000);
  delay(500);
  noTone(_pin);
  delay(250);
}
