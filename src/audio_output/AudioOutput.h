#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

/**
 * Base Class for both the DAC and I2S output
 **/
class AudioOutput
{
public:
  AudioOutput() {};
  virtual void start(uint32_t sample_rate) = 0;
  virtual void stop() = 0;
  // override this in derived classes to turn the sample into
  // something the output device expects - for the default case
  // this is simply a pass through
  virtual int16_t process_sample(int16_t sample) { return sample; }
  // write samples to the output
  virtual void write(int16_t *samples, int count) = 0;
};
