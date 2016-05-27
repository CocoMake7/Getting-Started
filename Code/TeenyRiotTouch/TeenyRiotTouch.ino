#define USE_KEYBOARD
#define USE_VELO

#ifdef USE_MIDI
#include <TeenyMidi.h>
MIDIMessage midimsg;
#endif

#ifdef USE_KEYBOARD
#include <TeenyKeyboard.h>
#endif


#include "TeenyTouchDusjagr.h"
#include "AnalogTouch.h"
#include "SampleFilter.h"

char key[] = {'I','Y','O','K','K','K','K','K','O','O','O','O','O'};

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


}


void loop()
{

    if (millis()-previousMillis >= 5)     // 0% data loss
        {
            //TeenyMidi.sendCCHires(value, 1);
            filtered_value = SampleFilter_get(&filter_samp[0]);

            //velocityValue[0] = filtered_value - prevValue[0];
            //prevValue[0] = filtered_value;

            //TeenyMidi.sendCCHires(filtered_value, (4*0)+1);
            //TeenyMidi.sendCCHires(value[0], (4*0)+1);

            if (filtered_value >= 100)
                {
                    if (note_off[0] == 1)
                        {
#ifdef USE_MIDI
                            TeenyMidi.send(MIDI_NOTEON,i, 127 );
#endif
#ifdef USE_KEYBOARD
                            TeenyKeyboard.print(key[0]);
#endif
                            note_off[0] = 0;
                        }
                }
            else
                {
                    if (note_off[0] == 0)
                        {
#ifdef USE_MIDI
                            //TeenyMidi.send(MIDI_NOTEOFF,i,127);
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
