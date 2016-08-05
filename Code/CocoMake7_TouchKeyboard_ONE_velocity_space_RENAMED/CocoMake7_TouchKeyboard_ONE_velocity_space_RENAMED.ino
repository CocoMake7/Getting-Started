#define USE_KEYBOARD
#define USE_VELO
#ifdef USE_MIDI
#include <CocoMidi.h>
MIDIMessage midimsg;
#endif

#ifdef USE_KEYBOARD
#include <CocoKeyboard.h>
#endif

#include <CocoTouch.h>
#include "CocoTouchFilterSetting.h"
    
//char key[] = {'C','O','C','O','M','A','K','E','7',' ','O','N','E','!',' '};
char key[] = {' '};
int keyTotal = 1;

// ATMEL ATTINY85
//
//                   +-\/-+
//             PB5  1|    |8  VCC
//ref     ADC3/PB3  2|    |7  PB2/ADC1
//capsens ADC2/PB4  3|    |6  PB1*
//             GND  4|    |5  PB0*
//                   +----+
//
// * indicates PWM port
//

int value[8] = {0,0,0,0,0,0,0,0};

#ifdef USE_VELO
int prevValue[8] = {0,0,0,0,0,0,0,0};
int velocityValue[8] = {0,0,0,0,0,0,0,0};
int prevVelocity[8] =  {0,0,0,0,0,0,0,0};
#endif

uint8_t note_off[8] = {1,1,1,1,1,1,1,1};
uint16_t offset_adc[8] = {0,0,0,0,0,0,0,0};
CocoTouchFilterSetting CocoFilter[8];
int filtered_value = 0;
uint8_t pin_queue = 0;
uint8_t multiplexer_mapping[8]  = {6,7,4,4,3,0,1,2}; //remap multiplexer pin
unsigned long previousMillis = 0;        // will store last time LED was updated
int keyCount = -1;
int ledPin = PB0;
int velocityThreshold = 80;

#define PIN_SELECT 0
#define NUM_CHANNEL 3

#define ADC_REF_PIN PB2
#define ADC_SENSE_PIN PB4


void usb_poll()
{
    usbPoll();
}

void setup()
{

#ifdef USE_MIDI
    TeenyMidi.init();
#endif

#ifdef USE_KEYBOARD
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
#endif


    CocoTouch.begin();
    CocoTouch.setAdcSpeed(4);
    CocoTouch.delay = 4;
    //TeenyTouchDusjagr.delay_cb = &delay;
    CocoTouch.usb_poll = &usb_poll;

    offset_adc[0] = CocoTouch.sense(ADC_SENSE_PIN,ADC_REF_PIN, 8 );

#ifdef USE_MIDI
    CocoMidi.delay(100);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); 
    CocoMidi.delay(100);        
    digitalWrite(ledPin, LOW); 
    CocoMidi.delay(100); 
    digitalWrite(ledPin, HIGH); 
    CocoMidi.delay(100);         
    digitalWrite(ledPin, LOW); 
    CocoMidi.delay(100);
#endif




}
          
void loop()
{

    if (millis()-previousMillis >= 5)     // 0% data loss
        {
            filtered_value = CocoTouchFilter_get(&CocoFilter[0]);
            velocityValue[0] = filtered_value - prevValue[0]+500;
            prevValue[0] = filtered_value;
            
            if (velocityValue[0]  >= 500 + velocityThreshold && prevVelocity[0] <= 500 + velocityThreshold)
                {digitalWrite(ledPin, HIGH);
                    if (note_off[0] == 1)
                        {
#ifdef USE_MIDI
                           // TeenyMidi.send(MIDI_NOTEON,0, filtered_value );
#endif

#ifdef USE_KEYBOARD
                keyCount++;
                if (keyCount == keyTotal) 
                  {keyCount = 0;}
                CocoKeyboard.print(key[keyCount]);
#endif
                        note_off[0] = 0;
                        }
                }
            if (velocityValue[0]  <= 500 - velocityThreshold)
              {digitalWrite(ledPin, HIGH);}
            else
              {digitalWrite(ledPin, LOW);
                 if (note_off[0] == 0) 
                   {note_off[0] = 1;}
               }
            previousMillis = millis();
        }


    value[0] = CocoTouch.sense(ADC_SENSE_PIN,ADC_REF_PIN, 7 ) - offset_adc[0];
    if (value[0] > 0) CocoTouchFilter_put(&CocoFilter[0], value[0]);
    
#ifdef USE_MIDI
    TeenyMidi.update();
#endif

#ifdef USE_MIDI
    TeenyMidi.delay(1);
#endif

#ifdef USE_KEYBOARD
    CocoKeyboard.delay(1);
#endif

prevVelocity[0] = velocityValue[0];

}

