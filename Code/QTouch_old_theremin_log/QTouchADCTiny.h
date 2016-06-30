/*
  QTouchSense.h - Library for Capacitive touch sensors using only one ADC PIN and a Reference
  modified from https://github.com/jgeisler0303/QTouchADCArduino
  Released into the public domain.
*/

#ifndef QTouchADCTiny_h
#define QTouchADCTiny_h

#include <Arduino.h>

class QTouchADCTinyClass
{
  public:
    void init();
    uint16_t sense(byte ADCReadPin, byte ADCRef, uint8_t samples);
    uint8_t touch(byte ADCReadPin);
};

extern QTouchADCTinyClass QTouchADCTiny;

#endif

