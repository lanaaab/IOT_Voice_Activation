//main.cpp
//RGB LED pins
#define BLUE_PIN 25
#define GREEN_PIN 33
#define RED_PIN 32

//I2s.cpp
//microphone pins
#define PIN_I2S_BCLK 26 //SCK
#define PIN_I2S_LRC 22 // SW
#define PIN_I2S_DIN 21  //SD

//config.h 
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_26
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_22
#define I2S_MIC_SERIAL_DATA GPIO_NUM_21

// mp3
#define RXp2 16
#define TXp2 17

//CloudSpeechClient.h
// neopixel ring
#define PIN       12 
#define NUMPIXELS 12 
