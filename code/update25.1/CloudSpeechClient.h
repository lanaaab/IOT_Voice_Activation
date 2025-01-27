#ifndef _CLOUDSPEECHCLIENT_H
#define _CLOUDSPEECHCLIENT_H
#include <WiFiClientSecure.h>
#include "Audio.h"
#include <WiFi.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>
#include <Adafruit_NeoPixel.h>
//#include <Arduino_JSON.h>
#define PIN        12 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size

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
  void PrintHttpBody2(Audio* audio);
  Authentication authentication;

public:
  CloudSpeechClient(Authentication authentication,HardwareSerial& MP3);
  ~CloudSpeechClient();
  void Transcribe(Audio* audio,HardwareSerial& MP3);
};

String httpGETRequest(const char* serverName);

#endif // _CLOUDSPEECHCLIENT_H
