#define USE_MIDI
#define USE_VELO

#ifdef USE_MIDI
#include <TeenyMidi.h>
MIDIMessage midimsg;
#endif

#ifdef USE_KEYBOARD
#include <TeenyKeyboard.h>
#endif


#include "TeenyTouchDusjagr.h"
#include "SampleFilter.h"

char key[] = {'C','O','C','O','M','A','K','E','7',' ','O','N','E','!',' '};

//TeenyTouchDusjagr test2;
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
#endif

uint8_t note_off[8] = {1,1,1,1,1,1,1,1};
uint16_t offset_adc[8] = {0,0,0,0,0,0,0,0};
SampleFilter filter_samp[8];
int filtered_value = 0;
uint8_t pin_queue = 0;
uint8_t multiplexer_mapping[8]  = {6,7,4,4,3,0,1,2}; //remap multiplexer pin
unsigned long previousMillis = 0;        // will store last time LED was updated
int keyCount = -1;
int keyTotal = 15;
int ledPin = PB0;

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
    TeenyKeyboard.update();
#endif


    TeenyTouchDusjagr.begin();
    TeenyTouchDusjagr.setAdcSpeed(4);
    TeenyTouchDusjagr.delay = 4;
    //TeenyTouchDusjagr.delay_cb = &delay;
    TeenyTouchDusjagr.usb_poll = &usb_poll;

    offset_adc[0] = TeenyTouchDusjagr.sense(ADC_SENSE_PIN,ADC_REF_PIN, 8 );

#ifdef USE_MIDI
    TeenyMidi.delay(100);
#endif
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); 
    TeenyMidi.delay(100);        
    digitalWrite(ledPin, LOW); 
    TeenyMidi.delay(100); 
    digitalWrite(ledPin, HIGH); 
    TeenyMidi.delay(100);         
    digitalWrite(ledPin, LOW); 
    TeenyMidi.delay(100);

}

void loop()
{

    if (millis()-previousMillis >= 5)     // 0% data loss
        {
            //TeenyMidi.sendCCHires(value, 1);
            filtered_value = SampleFilter_get(&filter_samp[0]);
            velocityValue[0] = filtered_value - prevValue[0];
            prevValue[0] = filtered_value;
            if (filtered_value <= 30) 
              {digitalWrite(ledPin, LOW);}
            if (filtered_value >= 100) 
              {digitalWrite(ledPin, HIGH);}
            if (filtered_value <= 0) 
              {filtered_value = 0;}
            if (filtered_value >= 1023) 
              {filtered_value = 1023;
              digitalWrite(ledPin, HIGH);          
              }
              
            //TeenyMidi.send(MIDI_NOTEON,0, filtered_value);
            //TeenyMidi.sendCCHires(filtered_value, (4*0)+1);
            TeenyMidi.sendCCHires(velocityValue[0]+500, (4*0)+1);
            //TeenyMidi.sendCCHires(value[0], (4*0)+1);

            if (filtered_value >= 10)
                { 
                    if (note_off[0] == 1)
                        {
#ifdef USE_MIDI
                           // TeenyMidi.send(MIDI_NOTEON,0, filtered_value );
#endif
#ifdef USE_KEYBOARD
                keyCount++;
                if (keyCount == keyTotal){keyCount = 0;}
                    digitalWrite(ledPin, HIGH);          

                            TeenyKeyboard.print(key[keyCount]);
#endif
                            note_off[0] = 0;
                        }
                }
            else
                {    digitalWrite(ledPin, LOW);          

                    if (note_off[0] == 0)
                        {
#ifdef USE_MIDI
                            //TeenyMidi.send(MIDI_NOTEOFF,0,127);
#endif
                            note_off[0] = 1;
                        }
                }

            previousMillis = millis();
        }


    value[0] = TeenyTouchDusjagr.sense(ADC_SENSE_PIN,ADC_REF_PIN, 7 ) - offset_adc[0];
    if (value[0] > 0) SampleFilter_put(&filter_samp[0], value[0]);
    
#ifdef USE_MIDI
    TeenyMidi.update();
#endif

#ifdef USE_MIDI
    TeenyMidi.delay(1);
#endif

#ifdef USE_KEYBOARD
    //TeenyKeyboard.update();
    TeenyKeyboard.delay(1);
#endif

//velocityValue[0] = value[0]-prevValue[0];
//prevValue[0] = value[0];
}

