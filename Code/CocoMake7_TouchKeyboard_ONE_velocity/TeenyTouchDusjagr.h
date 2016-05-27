/*
  QTouchSense.h - Library for Capacitive touch sensors using only one ADC PIN and a Reference
  modified from https://github.com/jgeisler0303/QTouchADCArduino
  Released into the public domain.
*/

#ifndef QTouchADCTiny_h
#define QTouchADCTiny_h

#include <Arduino.h>

class TeenyTouchDusjagrClass
{
  public:
    TeenyTouchDusjagrClass();
    void begin();
    uint16_t sense(byte adcPin, byte refPin, uint8_t samples);
    uint16_t touch(uint8_t pin, uint8_t partner, uint8_t samples);
    uint16_t touch_probe(uint8_t pin, uint8_t partner, bool dir);
    uint16_t touchPin(uint8_t adcPin, uint8_t samples);
    void setAdcSpeed(uint8_t mode);
    uint8_t delay;
    void wait_for_five_ticks();
    void wait_for_tick();
    void (*delay_cb)(uint8_t* text);
    void (*usb_poll)(void);

};

extern TeenyTouchDusjagrClass TeenyTouchDusjagr;

#endif

