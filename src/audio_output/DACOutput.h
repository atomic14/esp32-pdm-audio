#pragma once
#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

#include "I2SBase.h"

/**
 * Use I2S to output to the DAC
 **/
class DACOutput : public I2SBase
{
public:
    DACOutput() : I2SBase(I2S_NUM_0) {}
    void start(uint32_t sample_rate);
    virtual int16_t process_sample(int16_t sample)
    {
        // DAC needs unsigned 16 bit samples
        return sample + 32768;
    }
};
