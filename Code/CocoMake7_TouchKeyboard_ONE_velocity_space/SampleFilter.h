#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 125 Hz

fixed point precision: 10 bits

* 0 Hz - 5 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 22 Hz - 60 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define SAMPLEFILTER_TAP_NUM 10

typedef struct {
  int history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;

void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, int input);
int SampleFilter_get(SampleFilter* f);

#endif
