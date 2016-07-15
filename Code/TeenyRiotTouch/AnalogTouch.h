/*
  ADCTouch.h - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
/*
Copyright (c) 2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include guard
#pragma once

// Software version
#define ANALOGTOUCH_VERSION 100

// Include Arduino functions to read pins
#include "Arduino.h"

#define MUX_REF_VCC 0b00000000 // value to set the ADC reference to Vcc
#define MUX_ADMUX_GND 0b00001101 // value to set the ADC reference to Vcc


// Check if all needed reigsters are available to through a better error if not
#if !defined(MUX0) || !defined(MUX1) || !defined(MUX2) || !defined(MUX3)
|| !defined(ADMUX) || !defined(ADCSRA) || !defined(ADSC) || !defined(ADIF)
#error "MCU not compatible with this library."
#endif

static inline uint16_t analogTouchRead(uint8_t analogPin, uint16_t samples = 1)
{
    // Measure more than once to get a more precise result
    uint32_t value = 0;
    for (uint16_t i = 0; i < samples; i ++)
    {
        // Enable pullup
        pinMode(PB2, INPUT_PULLUP);

        // MCUs such as 328
        ADMUX = MUX_REF_VCC | MUX_ADMUX_GND;

        // Start the conversion
        ADCSRA |= (1 << ADSC);

        // Wait for conversion to finish
        while (!(ADCSRA & (1 << ADIF)));
        // Reset the flag
        ADCSRA |= (1 << ADIF);

        // Disable pullup
        pinMode(PB2, INPUT);

        // Measure and add to average
        value += analogRead(analogPin);
    }

    // Return average
    return value / samples;
}
