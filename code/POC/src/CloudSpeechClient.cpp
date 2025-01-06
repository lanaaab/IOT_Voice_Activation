#include "CloudSpeechClient.h"
#include "network_param.h"
#include <base64.h>
#include <ArduinoJson.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        12 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels



CloudSpeechClient::CloudSpeechClient(Authentication authentication) 
{
  this->authentication = authentication;
  WiFi.mode(WIFI_STA);//// added
  WiFi.begin(ssid, password);

  Serial.println("Connecting to wifi");
  while (WiFi.status() != WL_CONNECTED){ 
    Serial.println(".");
    delay(100);
  }
  Serial.print("Connected to wifi");
  
  client.setCACert(root_ca); // Set your root CA certificate here
  client.setInsecure();      // Accept any SSL certificate, including self-signed

  if (!client.connect(server, 443)) Serial.println("Connection failed!");

 // while (WiFi.status() != WL_CONNECTED) delay(1000);
 // client.setCACert(root_ca);
  //if (!client.connect(server, 443)) Serial.println("Connection failed!");
}

String ans;

CloudSpeechClient::~CloudSpeechClient() {
  client.stop();
  WiFi.disconnect();
}
 
void CloudSpeechClient::PrintHttpBody2(Audio* audio)
{
  String enc = base64::encode(audio->paddedHeader, sizeof(audio->paddedHeader));
  enc.replace("\n", "");  // delete last "\n"
  client.print(enc);      // HttpBody2
  char** wavData = audio->wavData;
  for (int j = 0; j < audio->wavDataSize / audio->dividedWavDataSize; ++j) {
    enc = base64::encode((byte*)wavData[j], audio->dividedWavDataSize);
    enc.replace("\n", "");// delete last "\n"
    client.print(enc);    // HttpBody2
  }
}


///////////////////////////////////////////////////////////////MP3 realted
void set_volume(byte volume,HardwareSerial& MP3) {
  Serial.print("set volume= ");
  Serial.print(volume);
  Serial.println(" of 30");
  MP3.write(start_byte);
  byte msg_len = 0x03;
  MP3.write(msg_len);
  MP3.write(set_volume_CMD);
  MP3.write(volume);
  MP3.write(end_byte);
  delay(20);  //required for stability
}



void play_filename(int8_t directory, int8_t file,HardwareSerial& MP3) {
  Serial.print("playing directory ");
  Serial.print(directory);
  Serial.print(" file number ");
  Serial.println(file);
  MP3.write(start_byte);
  byte msg_len = 0x04;
  MP3.write(msg_len);
  MP3.write(play_filename_CMD);
  byte dir_ind = directory;
  MP3.write(dir_ind);
  byte file_ind = file;
  MP3.write(file_ind);
  MP3.write(end_byte);
  delay(20);  //required for stability
}
/////////////////////////////////////////////////////////////////////

void Displaylight(String transcript,HardwareSerial& MP3){ 
  pixels1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels1.clear();
  // String strippedString="";
  // for(int i = 0; i < transcript.length(); i++) {
  // if (transcript[i] != ' ') {
  //   strippedString += transcript[i];
  // }
  // }
  // transcript = strippedString;
  if (transcript.indexOf("next") >= 0 || transcript.indexOf("Next") >= 0){
       String Colors[7] = {"red","green","blue","white","yellow","orange"};
      int i=random(0,6);
      transcript = Colors[i];  
   }
  if (transcript.indexOf("list") >= 0 || transcript.indexOf("List") >= 0){
      for(int i=1;i<=7;i++){
        set_volume(30,MP3);
        delay(1500);
        play_filename(1,i,MP3);
        
      }
    return;
  }

  if (transcript.indexOf("Random") >= 0 || transcript.indexOf("random") >= 0){
          int t=random(0,256);
          int j=random(0,256);
          int k=random(0,256);
           for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
              // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
              // Here we're using a moderately bright green color:
              pixels1.setPixelColor(i, pixels1.Color(t, j, k));

              pixels1.show();   // Send the updated pixel colors to the hardware.
              //delay(DELAYVAL); // Pause before next pass through loop
           }
    }   
  else if (transcript.indexOf("turn off") >= 0 || transcript.indexOf("Turn Off") >= 0){

   pixels1.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  //to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,9,MP3);
      
  }
  else if (transcript.indexOf("red") >= 0 || transcript.indexOf("Red") >= 0){

   pixels1.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(150, 0, 0));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,1,MP3);
  }
  else if (transcript.indexOf("green") >= 0 ||transcript.indexOf("Green") >= 0 ){
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(0, 150, 0));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,5,MP3);
  }
  else if (transcript.indexOf("blue") >= 0 ||transcript.indexOf("Blue") >= 0 ){
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(0, 0, 150));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,7,MP3);
  }

    else if (transcript.indexOf("white") >= 0 ||transcript.indexOf("White") >= 0 ){
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255, 255, 255));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,4,MP3);
  }
    else if (transcript.indexOf("yellow") >= 0 ||transcript.indexOf("Yellow") >= 0 ){
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255, 255, 0));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,6,MP3);
  }
    else if (transcript.indexOf("orange") >= 0 ||transcript.indexOf("Orange") >= 0 ){
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255,69,0));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,2,MP3);
  }
    else if (transcript.indexOf("pink") >= 0 ||transcript.indexOf("Pink") >= 0 ){
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255,20,147));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,3,MP3);
  }
  else{
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,8,MP3);
  }
}



void CloudSpeechClient::Transcribe(Audio* audio,HardwareSerial& MP3) {
  String HttpBody1 = "{\"config\":{\"encoding\":\"LINEAR16\",\"sampleRateHertz\":16000,\"languageCode\":\"en-US\"},\"audio\":{\"content\":\"";
  String HttpBody3 = "\"}}\r\n\r\n";
  int httpBody2Length = (audio->wavDataSize + sizeof(audio->paddedHeader)) * 4 / 3; // 4/3 is from base64 encoding
  String ContentLength = String(HttpBody1.length() + httpBody2Length + HttpBody3.length());
  String HttpHeader;
  // if (authentication == USE_APIKEY)
  HttpHeader = String("POST /v1/speech:recognize?key=") + ApiKey
               + String(" HTTP/1.1\r\nHost: speech.googleapis.com\r\nContent-Type: application/json\r\nContent-Length: ") + ContentLength + String("\r\n\r\n");
  //  else if (authentication == USE_ACCESSTOKEN)
  //    HttpHeader = String("POST /v1beta1/speech:syncrecognize HTTP/1.1\r\nHost: speech.googleapis.com\r\nContent-Type: application/json\r\nAuthorization: Bearer ")
  //   + AccessToken + String("\r\nContent-Length: ") + ContentLength + String("\r\n\r\n");
  client.print(HttpHeader);
  client.print(HttpBody1);
  PrintHttpBody2(audio);
  client.print(HttpBody3);
  String My_Answer="";
  while (!client.available());
   
  while (client.available())
  {
    char temp = client.read();
    My_Answer = My_Answer + temp;
   // Serial.write(client.read());
  }

  Serial.print("My Answer - ");Serial.println(My_Answer);
  int postion = My_Answer.indexOf('{');
  Serial.println(postion);
  ans = My_Answer.substring(postion);
  Serial.print("Json daata--");
  Serial.print(ans);
  

  // Find the start and end positions of the transcript
int startPos = ans.indexOf("\"transcript\":") + 15;  // 14 is the length of the string "\"transcript\":" + 1
int endPos = ans.indexOf("\"", startPos);  // find the closing quote after the transcript

// Extract the transcript substring
String transcript = ans.substring(startPos, endPos);

// Print the transcript
Serial.println("Transcript: " + transcript);


///Display color thats requested

Displaylight(transcript,MP3);
}


