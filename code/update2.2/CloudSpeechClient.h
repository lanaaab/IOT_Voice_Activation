#ifndef _CLOUDSPEECHCLIENT_H
#define _CLOUDSPEECHCLIENT_H
#include <WiFiClientSecure.h>
//#include "Audio.h"
#include <WiFi.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>
#include <Adafruit_NeoPixel.h>
#include "I2S.h"
//#include <Arduino_JSON.h>
#define PIN        12 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size

class Audio;

// Define the required MP3 Player Commands: based on command codes listed in manual - https://static1.squarespace.com/static/584d41b3f5e2310b396cd953/t/5c7c2f29104c7b336a2f8380/1551642412037/Serial+MP3+Player+A+v1.0+Manual.pdf
static byte start_byte = 0x7E;  // Start byte
static byte end_byte = 0xEF;    // End byte

static byte set_volume_CMD = 0x31;
static byte play_index_CMD = 0x41;
static byte play_filename_CMD = 0x42;
static uint8_t select_SD_CMD[] = { 0x7e, 0x03, 0X35, 0x01, 0xef };
static uint8_t reset_CMD[] = { 0x7e, 0x03, 0X35, 0x05, 0xef };
enum Authentication {
  USE_ACCESSTOKEN,
  USE_APIKEY
};
void set_volume(byte volume,HardwareSerial& MP3);
void play_filename(int8_t directory, int8_t file,HardwareSerial& MP3);




class CloudSpeechClient {
  WiFiClientSecure client;
  bool PrintHttpBody2();
  Authentication authentication;
  I2S* i2s;
  static const int headerSize = 44;
  static const int i2sBufferSize = 12000;
  char i2sBuffer[i2sBufferSize];
  void CreateWavHeader(byte* header, int waveDataSize);
public:
  CloudSpeechClient(Authentication authentication,HardwareSerial& MP3);
  ~CloudSpeechClient();
  void Transcribe(HardwareSerial& MP3);
  void preTranscribe();
  static const int segmentValue = 2; // how many iterations
  static const int wavDataSize = 60000;                   // It must be multiple of dividedWavDataSize. Recording time is about 1.9 second.
  static const int dividedWavDataSize = i2sBufferSize/4;
  char** wavData;                                         // It's divided. Because large continuous memory area can't be allocated in esp32.
  byte paddedHeader[headerSize + 4] = {0};  
};

String httpGETRequest(const char* serverName);

const String serverName = "https://api.callmebot.com/whatsapp.php?";
const String phoneNumber = "972542553023"; // Replace with your phone number
const String apiKey = "2123884";          // Replace with your API key



#endif // _CLOUDSPEECHCLIENT_H
