#include <CocoKeyboard.h>
#include <CocoTouch.h>
#include "CocoTouchFilterSetting.h"

//filter settings
CocoTouchFilterSetting CocoFilter;

int value = 0;
int prevValue = 0;
int velocityValue = 0;
int prevVelocity =  0;

uint8_t note_off = 0;
uint16_t offset_adc = 0;

char key[] = {' '};
int keyTotal = 1;

unsigned long previousMillis = 0; // will store last time LED was updated

int keyCount = -1;
int ledPin = PB0;
int velocityThreshold = 80;

int filtered_value = 0;
uint8_t pin_queue = 0;

//cocoTouch pin
#define ADC_REF_PIN PB2
#define ADC_SENSE_PIN PB4

void usb_poll()
{
  usbPoll();
}

void setup()
{

  CocoKeyboard.update();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  CocoKeyboard.delay(100);
  digitalWrite(ledPin, LOW);
  CocoKeyboard.delay(100);
  digitalWrite(ledPin, HIGH);
  CocoKeyboard.delay(100);
  digitalWrite(ledPin, LOW);
  CocoKeyboard.delay(100);

  CocoTouch.begin();
  CocoTouch.setAdcSpeed(4);
  CocoTouch.delay = 4;
  //TeenyTouchDusjagr.delay_cb = &delay;
  CocoTouch.usb_poll = &usb_poll;
  offset_adc = CocoTouch.sense(ADC_SENSE_PIN, ADC_REF_PIN, 8 );

}

void loop()
{

  if (millis() - previousMillis >= 5)   // 0% data loss
  {
    filtered_value = CocoTouchFilter_get(&CocoFilter);
    velocityValue = filtered_value - prevValue + 500;
    prevValue = filtered_value;

    if (velocityValue  >= 500 + velocityThreshold && prevVelocity <= 500 + velocityThreshold)
    { digitalWrite(ledPin, HIGH);
      if (note_off == 1)
      {

        keyCount++;
        if (keyCount == keyTotal)
        {
          keyCount = 0;
        }
        CocoKeyboard.print(key[keyCount]);
        note_off = 0;
      }
    }
    if (velocityValue  <= 500 - velocityThreshold)
    {
      digitalWrite(ledPin, HIGH);
    }
    else
    { digitalWrite(ledPin, LOW);
      if (note_off == 0)
      {
        note_off = 1;
      }
    }
    previousMillis = millis();
  }

  value = CocoTouch.sense(ADC_SENSE_PIN, ADC_REF_PIN, 7 ) - offset_adc;
  if (value > 0) CocoTouchFilter_put(&CocoFilter, value);

  CocoKeyboard.delay(1);

  prevVelocity = velocityValue;

}

