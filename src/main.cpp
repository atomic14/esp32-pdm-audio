#include <Arduino.h>
#include "SPIFFS.h"

#include "audio_output/PCMOutput.h"
#include "audio_output/DACOutput.h"
#include "audio_output/PDMTimerOutput.h"
#include "audio_output/PDMOutput.h"

#include "wavfile/WAVFileReader.h"

AudioOutput *audioOutput = NULL;
WAVFileReader *wavFileReader = NULL;

// assume the test wav file is 16KHz...
#define SAMPLE_RATE 16000

void setup()
{
  Serial.begin(115200);
  // for(int i = 0; i<10; i++) {
  //   Serial.printf(".");
  //   delay(1000);
  // }
#ifdef USE_DAC_AUDIO
  audioOutput = new DACOutput(I2S_NUM_0);
  audioOutput->start(SAMPLE_RATE);
#endif
#ifdef PDM_GPIO_NUM
  i2s_pin_config_t i2s_pdm_pins = {
    // no bck for PDM
    .bck_io_num = I2S_PIN_NO_CHANGE,
    // use a dummy pin for the LR clock - 45 or 46 is a good options
    // as these are normally not connected
    .ws_io_num = GPIO_NUM_45,
    // where should we send the PDM data
    .data_out_num = PDM_GPIO_NUM,
    // no data to read
    .data_in_num = I2S_PIN_NO_CHANGE};
  audioOutput = new PDMOutput(i2s_pdm_pins);
  audioOutput->start(SAMPLE_RATE);
#endif
#ifdef I2S_SPEAKER_SERIAL_CLOCK
  // i2s speaker pins
  i2s_pin_config_t i2s_speaker_pins = {
      .bck_io_num = I2S_SPEAKER_SERIAL_CLOCK,
      .ws_io_num = I2S_SPEAKER_LEFT_RIGHT_CLOCK,
      .data_out_num = I2S_SPEAKER_SERIAL_DATA,
      .data_in_num = I2S_PIN_NO_CHANGE};

  audioOutput = new PCMOutput(I2S_NUM_0, i2s_speaker_pins);
  audioOutput->start(SAMPLE_RATE);
#endif

  // open up the WAV file from spiffs
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  }
  FILE *fp = fopen("/spiffs/sample.wav", "r");
  if (fp == NULL)
  {
    Serial.println("Failed to open file for reading");
  }
  wavFileReader = new WAVFileReader(fp);
}

void loop()
{
  while(true) {
    int16_t samples[1000];
    int count = wavFileReader->read(samples, 1000);
    if (count > 0)
    {
      audioOutput->write(samples, count);
    }
    else
    {
      wavFileReader->rewind();
    }
  }
}
