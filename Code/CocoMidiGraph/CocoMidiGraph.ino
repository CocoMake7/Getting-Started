//for CocoMake v0.18+

#include <CocoMidi.h>
#include <CocoTouch.h>
#include <CocoTouchFilterSettingDefault.h>


//filter settings
CocoTouchFilterSetting CocoFilter;

int value = 0;
int prevValue = 0;
int velocityValue = 0;
int prevVelocity =  0;

uint16_t offset_adc = 0;

unsigned long previousMillis = 0; // will store last time LED was updated

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

void setup() {
  CocoMidi.init();


CocoTouch.begin();
CocoTouch.setAdcSpeed(4);
CocoTouch.delay = 4;
//TeenyTouchDusjagr.delay_cb = &delay;
CocoTouch.usb_poll = &usb_poll;
offset_adc = CocoTouch.sense(ADC_SENSE_PIN, ADC_REF_PIN, 8 );

}

void loop() {

  if (millis() - previousMillis >= 5)   // 0% data loss
  {
    filtered_value = CocoTouchFilter_get(&CocoFilter);
    velocityValue = filtered_value - prevValue + 500;
    prevValue = filtered_value;

    CocoMidi.sendCCHires(filtered_value,1);


    previousMillis = millis();
  }

  value = CocoTouch.sense(ADC_SENSE_PIN, ADC_REF_PIN, 7 ) - offset_adc;
  if (value > 0) CocoTouchFilter_put(&CocoFilter, value);
  prevVelocity = velocityValue;
  CocoMidi.update();

}