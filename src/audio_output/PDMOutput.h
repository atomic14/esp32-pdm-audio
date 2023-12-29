#pragma once
#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

#include "I2SBase.h"

/**
 * Use I2S to output PDM samples
 **/
class PDMOutput : public I2SBase
{
private:
    i2s_pin_config_t m_i2s_pins;
public:
    PDMOutput(i2s_pin_config_t &i2s_pins): I2SBase(I2S_NUM_0), m_i2s_pins(i2s_pins) {};
    void start(uint32_t sample_rate);
    int16_t process_sample(int16_t sample) { 
        float normalised = (float)sample / 32768.0f;
        // give it some welly
        normalised *= 10.0f;
        if (normalised > 1.0f) normalised = 1.0f;
        if (normalised < -1.0f) normalised = -1.0f;
        return normalised * 32767.0f;
    }
};
