#pragma once

#include "I2SBase.h"

/**
 * Use I2S PCM output
 **/
class PCMOutput : public I2SBase
{
private:
    i2s_pin_config_t m_i2s_pins;
public:
    PCMOutput(i2s_port_t i2s_port, i2s_pin_config_t &i2s_pins);
    void start(uint32_t sample_rate);
};
