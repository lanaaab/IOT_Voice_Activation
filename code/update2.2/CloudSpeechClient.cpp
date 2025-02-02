#include "CloudSpeechClient.h"
#include "network_param.h"
#include <base64.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <HTTPClient.h>


Adafruit_NeoPixel pixels1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define BLUE_PIN 25
#define GREEN_PIN 33
#define RED_PIN 32


#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
///////////////////////////////////Audioo

Preferences preferencesLana1;
void updateCurrentColor(int default1){
  preferencesLana1.begin("my-appp", false);
if(default1 == 1){
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 0;
  unsigned int our_color3 = 0;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 1);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
  }
  else if(default1 == 2){
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 69;
  unsigned int our_color3 = 0;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 2);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
}
else if(default1 == 3){
      
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 20;
  unsigned int our_color3 = 147;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 3);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
}

else if(default1 == 4 || default1 == 0){
      
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 255;
  unsigned int our_color3 = 255;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 4);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
}
else if(default1 == 5){
      
  unsigned int our_color1 = 0;
  unsigned int our_color2 = 255;
  unsigned int our_color3 = 0;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 5);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
}
else if(default1 == 6){
      
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 255;
  unsigned int our_color3 = 0;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 6);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
}
else if(default1 == 7){
        
  unsigned int our_color1 = 0;
  unsigned int our_color2 = 0;
  unsigned int our_color3 = 255;
  preferencesLana1.putUInt("counter1", our_color1);
  preferencesLana1.putUInt("counter2", our_color2);
  preferencesLana1.putUInt("counter3", our_color3);
  preferencesLana1.putUInt("currentColor", 7);
  preferencesLana1.putUInt("brightness", 100);
  preferencesLana1.end();
}
}

void CloudSpeechClient::CreateWavHeader(byte* header, int waveDataSize){
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSizeMinus8 = waveDataSize + 44 - 8;
  header[4] = (byte)(fileSizeMinus8 & 0xFF);
  header[5] = (byte)((fileSizeMinus8 >> 8) & 0xFF);
  header[6] = (byte)((fileSizeMinus8 >> 16) & 0xFF);
  header[7] = (byte)((fileSizeMinus8 >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;  // linear PCM
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;  // linear PCM
  header[21] = 0x00;
  header[22] = 0x01;  // monoral
  header[23] = 0x00;
  header[24] = 0x80;  // sampling rate 16000
  header[25] = 0x3E;
  header[26] = 0x00;
  header[27] = 0x00;
  header[28] = 0x00;  // Byte/sec = 16000x2x1 = 32000
  header[29] = 0x7D;
  header[30] = 0x00;
  header[31] = 0x00;
  header[32] = 0x02;  // 16bit monoral
  header[33] = 0x00;
  header[34] = 0x10;  // 16bit
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(waveDataSize & 0xFF);
  header[41] = (byte)((waveDataSize >> 8) & 0xFF);
  header[42] = (byte)((waveDataSize >> 16) & 0xFF);
  header[43] = (byte)((waveDataSize >> 24) & 0xFF);
}


//////////////////////////////weather
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

String openWeatherMapApiKey = "c1534e2fed2464f7dd4941e28b5155a8";
String city = "Haifa";
String countryCode = "IL";
String jsonBuffer;


//////////////////////////////////

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



CloudSpeechClient::CloudSpeechClient(Authentication authentication,HardwareSerial& MP3) 
{
  this->authentication = authentication;
  wavData = new char*[wavDataSize/dividedWavDataSize];
  for (int i = 0; i < wavDataSize/dividedWavDataSize; ++i) wavData[i] = new char[dividedWavDataSize];
  i2s = new I2S(ICS43434);
  
  // WiFi.mode(WIFI_STA);//// added
  // WiFi.persistent(false);//// added
  // WiFi.begin(ssid, password);

  // Serial.println("Connecting to wifi");
  // while (WiFi.status() != WL_CONNECTED){ 
  //   Serial.println(".");
  //   delay(100);
  // }
  // Serial.print("Connected to wifi");
  
  client.setCACert(root_ca); // Set your root CA certificate here
  client.setInsecure();      // Accept any SSL certificate, including self-signed

  if (!client.connect(server, 443)){
     Serial.println("Connection failed!");
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,10,MP3);
      ESP.restart();

  }

 // while (WiFi.status() != WL_CONNECTED) delay(1000);
 // client.setCACert(root_ca);
  //if (!client.connect(server, 443)) Serial.println("Connection failed!");
}

String ans;

CloudSpeechClient::~CloudSpeechClient() {
  // client.stop();
  // WiFi.disconnect();

    client.stop();
    for (int i = 0; i < wavDataSize/dividedWavDataSize; ++i) delete[] wavData[i];
    delete[] wavData;
    delete i2s;
}
 
bool CloudSpeechClient::PrintHttpBody2()
{
  bool neverAgain = false;
  int curr =0;
  for(int segment = 0; segment<segmentValue;segment++){
     for (int j = 0; j < wavDataSize/dividedWavDataSize; ++j) {
      i2s->Read(i2sBuffer, i2sBufferSize);
      for (int i = 0; i < i2sBufferSize/8; ++i) {
        wavData[j][2*i] = i2sBuffer[8*i + 2];
        wavData[j][2*i + 1] = i2sBuffer[8*i + 3];
      }
    
      String enc = base64::encode((byte*)wavData[j], dividedWavDataSize);
      enc.replace("\n", "");// delete last "\n"
      client.print(enc);    
    
    // else if(digitalRead(button) == HIGH || neverAgain){
    //     if(segment ==0 && j< 15){
    //        Serial.print("recording is too short");
    //        return false;
    //     }
    //     if(!neverAgain){
    //     curr = j;
    //     memset(wavData[j], 0, dividedWavDataSize);
    //     neverAgain = true;
    //     digitalWrite(led_1,0);
    //     digitalWrite(led_3,0);
    //     digitalWrite(led_2,1);
    //     }
    //     String enc = base64::encode((byte*)wavData[curr], dividedWavDataSize);
    //     enc.replace("\n", "");// delete last "\n"
    //     client.print(enc);
    // }
 } 
}
return true;
}


void printLocalTime(HardwareSerial& MP3)
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.println(timeinfo.tm_hour);
  Serial.println(timeinfo.tm_min);
  delay(1000);
  set_volume(25, MP3);  // Set volume
  delay(1000);
  play_filename(2, timeinfo.tm_hour % 12, MP3);  // Play hour
  delay(1000);
  play_filename(3, timeinfo.tm_min, MP3);  // Play minutes
  delay(1000);
}


String URL = "http://api.openweathermap.org/data/2.5/weather?";

String ApiKeyW = "c1534e2fed2464f7dd4941e28b5155a8";



// Replace with your location Credentials

String lat = "32.77779191475653";

String lon = "35.01913642447971";

Preferences preferences122;
Preferences preferences3;
Preferences preferences123332223;
// Function to extract the hour from the string
int extractHour(String input) {
  int colonIndex = input.indexOf(':'); // Find the position of ':'
  int spaceIndex = input.lastIndexOf(' ', colonIndex != -1 ? colonIndex : input.length()); // Find the space before the hour
  
  if (spaceIndex != -1) {
    String hourPart = colonIndex != -1 
                      ? input.substring(spaceIndex + 1, colonIndex) // Extract hour when colon is present
                      : input.substring(spaceIndex + 1);            // Extract hour when colon is absent
    return hourPart.toInt(); // Convert to an integer
  }
  return -1; // Return -1 if hour extraction fails
}

// Function to extract the minutes from the string
int extractMinutes(String input) {
  int colonIndex = input.indexOf(':'); // Find the position of ':'
  
  if (colonIndex != -1) {
    int spaceIndex = input.indexOf(' ', colonIndex); // Find the space after the minutes
    String minutesPart = spaceIndex != -1 
                         ? input.substring(colonIndex + 1, spaceIndex) // Extract minutes if space follows
                         : input.substring(colonIndex + 1);            // Extract till end of string
    return minutesPart.toInt(); // Convert to an integer
  }
  
  return -1; // Return -1 if no minutes found
}

// Function to extract "am" or "pm" from a string
String extractAmPm(String input) {
  input.toLowerCase(); // Convert the string to lowercase for consistency
  if (input.indexOf("a.m.") != -1) {
    return "am";
  } else if (input.indexOf("p.m.") != -1) {
    return "pm";
  } else {
    return ""; // Return an empty string if neither is found
  }
}


// Function to extract number from a string
int extractNumber(String input) {
  
  String number = ""; // String to hold the number part
  int indexofcolon = input.indexOf(":");
  for (int i = 0; i < input.length(); i++) {
    if(i == indexofcolon){
    break;
    }
    if (isDigit(input[i])) { // Check if the character is a digit
      number += input[i]; // Append digit to the number string
    }
  }
  return number.toInt(); // Convert the extracted string to an integer
}
Preferences preferences1212;
Preferences preferences12121;
void Displaylight(String transcript,HardwareSerial& MP3){ 
  bool light = false;
  pixels1.begin(); 
  pixels1.clear();
   if (transcript.indexOf("set default") >= 0 ||transcript.indexOf("Set default") >= 0 ){
      light = true;
      String target = "default ";
int index = transcript.indexOf(target);

if (index != -1) {  
    int start = index + target.length();  // Position after "default "
    int end = transcript.indexOf(' ', start);  // Find next space after the color

    String color;
    if (end == -1) {  // No space found, color is the last word
        color = transcript.substring(start);
    } else {
        color = transcript.substring(start, end);
    }

    Serial.println(color); // Output: blue

      
      preferences122.begin("my-appp", false);
      if(color == "red" || color == "Red"){
        unsigned int default1 = 1;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else if(color == "green" || color == "Green"){
        unsigned int default1 = 5;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else if(color == "blue" || color == "Blue"){
        unsigned int default1 = 7;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else if(color == "white" || color == "White"){
        unsigned int default1 = 4;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else if(color == "yellow" || color == "Yellow"){
        unsigned int default1 = 6;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else if(color == "pink" || color == "Pink"){
        unsigned int default1 = 3;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else if(color == "orange" || color == "Orange"){
        unsigned int default1 = 2;
        preferences122.putUInt("default", default1);
        preferences122.end();
      }
      else{
        set_volume(30,MP3);
        delay(1000);
        play_filename(1,8,MP3);
      }
      set_volume(30,MP3);
      delay(1000);
      play_filename(1,18,MP3);
      return;
}}

  if(transcript.indexOf("weather") >= 0 || transcript.indexOf("Weather") >= 0){

    if(WiFi.status()== WL_CONNECTED){
      
    HTTPClient http;
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKeyW);
    // start connection and send HTTP Request

    int httpCode = http.GET();
    // httpCode will be negative on error

    if (httpCode > 0) {
      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);
          //Retrieve some information about the weather from the JSON format

      DynamicJsonDocument doc(2048);

      deserializeJson(doc, JSON_Data);

      JsonObject obj = doc.as<JsonObject>();
      //Display the Current Weather Info

      const char* description = obj["weather"][0]["description"].as<const char*>();
      const float temp = obj["main"]["temp"].as<float>();
      const float humidity = obj["main"]["humidity"].as<float>();
      Serial.println("Temp: " + String(temp));
      int temp1 = temp;
      set_volume(30,MP3);
      delay(1000);
      play_filename(4,103,MP3);
      delay(2000);
      play_filename(4,temp,MP3);
      delay(1000);
      play_filename(4,104,MP3);

      return;
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
      if (transcript.indexOf("timer cancel") >= 0 ||transcript.indexOf("Timer cancel") >= 0 ){
    light=true;
    preferences1212.begin("timer", false);
    unsigned int timer = preferences1212.getUInt("timer", 0);
    timer=0;
    preferences1212.putUInt("timer", timer);
    preferences1212.end();
    return;
  }

  if (transcript.indexOf("time") >= 0 || transcript.indexOf("Time") >= 0){
      light=true;
      // Configure NTP and time zone handling for Israel
      configTzTime(time_zone, ntpServer);
      printLocalTime(MP3);
  }

  if (transcript.indexOf("next") >= 0 || transcript.indexOf("Next") >= 0){
       String Colors[7] = {"red","green","blue","white","yellow","orange"};
      int i=random(0,6);
      //updateCurrentColor(i);
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
          light=true;
          int t=random(0,256);
          int j=random(0,256);
          int k=random(0,256);
          preferences122.begin("my-appp", false);
          unsigned int our_color1 = t;
          unsigned int our_color2 = j;
          unsigned int our_color3 = k;
          unsigned int currentColor = 17;
          preferences122.putUInt("counter1", our_color1);
          preferences122.putUInt("counter2", our_color2);
          preferences122.putUInt("counter3", our_color3);
          preferences122.putUInt("currentColor", currentColor);
          preferences122.putUInt("brightness", 100);
          preferences122.end();
           for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
              // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
              // Here we're using a moderately bright green color:
              pixels1.setPixelColor(i, pixels1.Color(t, j, k));
              pixels1.show();   // Send the updated pixel colors to the hardware.
              //delay(DELAYVAL); // Pause before next pass through loop
           }
           set_volume(30,MP3);
            delay(1000);
            play_filename(1,17,MP3);

  }   
  else if (transcript.indexOf("reading") >= 0 || transcript.indexOf("Reading") >= 0){//warm white with slight orange tint
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
   pixels1.setBrightness(102);
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 255;
    unsigned int our_color2 = 223;
    unsigned int our_color3 = 179;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", 12);
    preferences122.putUInt("brightness", 40);
    preferences122.end();
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  //to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255, 223, 179));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,12,MP3);
      
  }
  else if (transcript.indexOf("relaxing") >= 0 || transcript.indexOf("Relaxing") >= 0){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
   pixels1.setBrightness(78);//30% brightness soft purple
    preferences122.begin("my-appp", false);
    unsigned int our_color1 = 147;
    unsigned int our_color2 = 112;
    unsigned int our_color3 = 219;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", 13);
    preferences122.putUInt("brightness", 30);
    preferences122.end();
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  //to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(147, 112, 219));

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,13,MP3);
      
  }


  else if (transcript.indexOf("party") >= 0 || transcript.indexOf("Party") >= 0){
      set_volume(30,MP3);
      delay(1000);
      play_filename(1,14,MP3);
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 0;
    unsigned int our_color2 = 0;
    unsigned int our_color3 = 0;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", 14);
    preferences122.putUInt("brightness", 100);
    preferences122.end();
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  //to the count of pixels minus one.
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<2; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels1.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<pixels1.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels1.numPixels();
        uint32_t color = pixels1.gamma32(pixels1.ColorHSV(hue)); // hue -> RGB
        pixels1.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels1.show();                // Update strip with new contents
      delay(1000);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }

  }

  else if (transcript.indexOf("red") >= 0 || transcript.indexOf("Red") >= 0){
    light=true;

   pixels1.clear(); // Set all pixel colors to 'off'
    preferences122.begin("my-appp", false);
    unsigned int our_color1 = 150;
    unsigned int our_color2 = 0;
    unsigned int our_color3 = 0;
    unsigned int currentColor = 1;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255, 0, 0));
    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,1,MP3);
  }

  else if (transcript.indexOf("green") >= 0 ||transcript.indexOf("Green") >= 0 ){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 0;
    unsigned int our_color2 = 150;
    unsigned int our_color3 = 0;
    unsigned int currentColor = 5;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(0, 255, 0));

    // our_color0 = 0;
    // our_color1 = 150;
    // our_color2 = 0;

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
   pixels1.show();  
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,5,MP3);
  }

  else if (transcript.indexOf("blue") >= 0 ||transcript.indexOf("Blue") >= 0 ){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 0;
    unsigned int our_color2 = 0;
    unsigned int our_color3 = 255;
    unsigned int currentColor = 7;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(0, 0, 255));

    // our_color0 = 0;
    // our_color1 = 0;
    // our_color2 = 150;

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,7,MP3);
  }
  else if(transcript.indexOf("help") >=0 || transcript.indexOf("Help") >=0){
    preferences3.begin("help", false);
    unsigned int help = preferences3.getUInt("help", 0);
    help=1;
    preferences3.putUInt("help", help);
    preferences3.end();

         //preferences44.begin("help", false);
      //unsigned int help = preferences44.getUInt("help", 0);
      //if(help == 1){
          light=true;
          HTTPClient http;
          String message = "Help!";
          String serverPath = serverName + "phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + message;

          http.begin(serverPath.c_str());

          int httpResponseCode = http.GET();

          if (httpResponseCode > 0) {
            Serial.println("Message sent successfully!");
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
          } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
          }
          http.end();
        //  help = 0;
          //preferences44.putUInt("help", help);
          
     // }
         set_volume(30,MP3);
      delay(1000);
      play_filename(1,20,MP3);
       delay(1000);
     set_volume(30,MP3);
      delay(1000);
      play_filename(1,19,MP3);

  }

  else if (transcript.indexOf("white") >= 0 ||transcript.indexOf("White") >= 0 ){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 255;
    unsigned int our_color2 = 255;
    unsigned int our_color3 = 255;
    unsigned int currentColor = 4;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255, 255, 255));

    // our_color0 = 255;
    // our_color1 = 255;
    // our_color2 = 255;

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,4,MP3);
  }

  else if (transcript.indexOf("yellow") >= 0 ||transcript.indexOf("Yellow") >= 0 ){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 255;
    unsigned int our_color2 = 255;
    unsigned int our_color3 = 0;
    unsigned int currentColor = 6;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255, 255, 0));

    // our_color0 = 255;
    // our_color1 = 255;
    // our_color2 = 0;

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,6,MP3);
  }

  else if (transcript.indexOf("orange") >= 0 ||transcript.indexOf("Orange") >= 0 ){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 255;
    unsigned int our_color2 = 69;
    unsigned int our_color3 = 0;
    unsigned int currentColor = 2;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255,69,0));
    
    // our_color0 = 255;
    // our_color1 = 69;
    // our_color2 = 0;

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,2,MP3);
  }

  else if (transcript.indexOf("pink") >= 0 ||transcript.indexOf("Pink") >= 0 ){
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 255;
    unsigned int our_color2 = 20;
    unsigned int our_color3 = 147;
    unsigned int currentColor = 3;
    unsigned int brightness = 100;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
    preferences122.putUInt("currentColor", currentColor);
    preferences122.putUInt("brightness", brightness);
    preferences122.end();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels1.setPixelColor(i, pixels1.Color(255,20,147));

    // our_color0 = 255;
    // our_color1 = 20;
    // our_color2 = 147;

    pixels1.show();   // Send the updated pixel colors to the hardware.
    //delay(DELAYVAL); // Pause before next pass through loop
    
  }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,3,MP3);
  }

  else if (transcript.indexOf("current color") >= 0 ||transcript.indexOf("Current color") >= 0 ){
   light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
    preferences122.begin("my-appp", false);
    unsigned int currentColor = preferences122.getUInt("currentColor", 1);
    preferences122.end();
    if(currentColor == 0){
      set_volume(30,MP3);
      delay(1000);
      play_filename(1,9,MP3);
      return;
    }
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,currentColor,MP3);
  }

  else if (transcript.indexOf("current brightness") >= 0 ||transcript.indexOf("Current brightness") >= 0 ){
   light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
    preferences122.begin("my-appp", false);
    unsigned int currentBrightness = preferences122.getUInt("brightness", 100);
    unsigned int currentColor = preferences122.getUInt("currentColor", 1);
    preferences122.end();
      if(currentColor == 0){
      set_volume(30,MP3);
      delay(1000);
      play_filename(1,9,MP3);
      return;
      }
      set_volume(30,MP3);
      delay(1000);
      play_filename(4,101,MP3);
      delay(2000);
      play_filename(4,currentBrightness,MP3);
      delay(1000);
      play_filename(4,102,MP3);
  }

  if (transcript.indexOf("%") >= 0 ){
    pixels1.clear();
      int percent_pos = transcript.indexOf('%');
    int space_pos = transcript.lastIndexOf(' ', percent_pos);
  
    String number_str = transcript.substring(space_pos + 1, percent_pos);
    
    int number = number_str.toInt();
    int brightness = map(number, 0, 100, 0, 255);
    pixels1.setBrightness(brightness);
    preferences122.begin("my-appp", false);
    unsigned int our_color1 = preferences122.getUInt("counter1", 255);
    unsigned int our_color2 = preferences122.getUInt("counter2", 255);
    unsigned int our_color3 = preferences122.getUInt("counter3", 255);
    preferences122.putUInt("brightness", number);
    //preferences122.putUInt("counter", counter);
    preferences122.end();
    
    for (int i = 0; i < NUMPIXELS; i++) {
    pixels1.setPixelColor(i, pixels1.Color(our_color1, our_color2, our_color3));
    pixels1.show();
  
    }
    set_volume(30,MP3);
    delay(1000);
    play_filename(1,18,MP3);
      return;
  }

   if (transcript.indexOf("morning cancel") >= 0 ||transcript.indexOf("Morning cancel") >= 0 ){
    light=true;
    preferences1212.begin("morning", false);
    unsigned int morning = preferences1212.getUInt("morning", 0);
    morning=0;
    preferences1212.putUInt("morning", morning);
    preferences1212.end();
    return;
  }
   if (transcript.indexOf("night cancel") >= 0 ||transcript.indexOf("Night cancel") >= 0 ){
    light=true;
    preferences1212.begin("night", false);
    unsigned int night = preferences1212.getUInt("night", 0);
    night=0;
    preferences1212.putUInt("night", night);
    preferences1212.end();
    return;
  }

  if (transcript.indexOf("morning") >= 0 ||transcript.indexOf("Morning") >= 0 ){
    light=true;
    configTzTime(time_zone, ntpServer);
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,15,MP3);
      delay(2000);
      play_filename(1,8,MP3);
      delay(1000);
      return;
      //some sort of feedback
    }
    Serial.println("morning");
    int extractedHour = extractNumber(transcript);
    int extractedMinutes = extractMinutes(transcript);
    String amPm = extractAmPm(transcript);
    int hour = extractedHour;
    int minute = extractedMinutes;
    if(extractedHour >= 13 || extractedHour <= 0){
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,8,MP3);
      return;
    }
    if(hour == -1){
      Serial.println("Failed to extract hour");
      //some sort of feedback maybe try again......
      return;
    }
    if(minute == -1){
      Serial.println("Failed to extract minute");
      //some sort of feedback maybe try again......
      minute = 0;
    }
    if(amPm == "pm"){
      if(hour == 12){
        hour = 0;
      }
      hour = (hour + 12) % 24;
    }
    
    Serial.println(hour);
    Serial.println(minute);
    preferences1212.begin("morning", false);
    preferences1212.putUInt("hour", hour);
    preferences1212.putUInt("minute", minute);
    unsigned int morning = preferences1212.getUInt("morning", 0);
    morning=1;
    preferences1212.putUInt("morning", morning);
    preferences1212.end();
  }
    if (transcript.indexOf("night") >= 0 ||transcript.indexOf("Night") >= 0 ){
    light=true;
    configTzTime(time_zone, ntpServer);
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      //some sort of feedback
    }
    Serial.println("night");
    int extractedHour = extractNumber(transcript);
    int extractedMinutes = extractMinutes(transcript);
    String amPm = extractAmPm(transcript);
    int hour = extractedHour;
    int minute = extractedMinutes;
    if(extractedHour >= 13 || extractedHour <= 0){
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,8,MP3);
      delay(1000);
      return;
    }
    if(hour == -1){
      Serial.println("Failed to extract hour");
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,15,MP3);
      delay(2000);
      play_filename(1,8,MP3);
      delay(1000);
      //some sort of feedback maybe try again......
      return;
    }
    if(minute == -1){
      Serial.println("Failed to extract minute");
      //some sort of feedback maybe try again......
      minute = 0;
    }
    if(amPm == "pm"){
      if(hour == 12){
        hour = 0;
      }
      hour = (hour + 12) % 24;
    }
    
    Serial.println(hour);
    Serial.println(minute);
    preferences12121.begin("night", false);
    preferences12121.putUInt("hour", hour);
    preferences12121.putUInt("minute", minute);
    unsigned int night = preferences12121.getUInt("night", 0);
    night=1;
    preferences12121.putUInt("night", night);
    preferences12121.end();
    set_volume(30,MP3);
    delay(1000);
    play_filename(1,16,MP3);
  }

  if (transcript.indexOf("turn off") >= 0 ||transcript.indexOf("Turn off") >= 0 ){
    //turn on the current coloor... if its on it is not supposed to matter, if off turn on the default color...
    light=true;
    preferences122.begin("my-appp", false);
    int extractedNumber = extractNumber(transcript);
    if(extractedNumber == 0){
         if (transcript.indexOf("turn off in") >= 0 ||transcript.indexOf("Turn off in") >= 0 ){
          set_volume(20,MP3);
          delay(1000);
          play_filename(1,8,MP3);
          return;
        }
        preferences123332223.begin("timer", false);
        unsigned int timer = preferences123332223.getUInt("timer", 0);
        timer=0;
        preferences123332223.putUInt("timer", timer);
        preferences123332223.end();
      pixels1.clear(); // Set all pixel colors to 'off'
      unsigned int our_color1 = 0;
      unsigned int our_color2 = 0;
      unsigned int our_color3 = 0;
      preferences122.putUInt("counter1", our_color1);
      preferences122.putUInt("counter2", our_color2);
      preferences122.putUInt("counter3", our_color3);
      preferences122.putUInt("currentColor", 0);
      preferences122.end();
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
      return;
      //some sort of feedback maybe try again......
    }
    
    unsigned int currentColor = preferences122.getUInt("currentColor", 1);
    if(currentColor == 0){
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,9,MP3);
      preferences122.end();
      return;
    }

    configTzTime(time_zone, ntpServer);
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
      //some sort of feedback
    }
    unsigned int hour = timeinfo.tm_hour;
    unsigned int minute = timeinfo.tm_min;
    unsigned int second = timeinfo.tm_sec;
    
    int h=extractedNumber/60;
    hour+=h;
    int m=extractedNumber%60;
    minute+=m;
    hour+=minute/60;
    minute=minute%60;
    Serial.println(hour);
    Serial.println(minute);
    Preferences preferences3122;
    preferences3122.begin("timer", false);
    preferences3122.putUInt("hour", hour);
    preferences3122.putUInt("minute", minute);
    preferences3122.putUInt("second", second);
    unsigned int timer = preferences3122.getUInt("timer", 0);
    timer=1;
    preferences3122.putUInt("timer", timer);
    preferences3122.end();
  }


  if(!light){
      set_volume(20,MP3);
      delay(1000);
      play_filename(1,8,MP3);
  }
}

void CloudSpeechClient::preTranscribe(){
  String HttpBody1 = "{\"config\":{\"encoding\":\"LINEAR16\",\"sampleRateHertz\":16000,\"languageCode\":\"en-US\"},\"audio\":{\"content\":\"";
  String HttpBody3 = "\"}}\r\n\r\n";
  int httpBody2Length = ((wavDataSize*segmentValue) + sizeof(paddedHeader)) * 4 / 3; // 4/3 is from base64 encoding
  String ContentLength = String(HttpBody1.length() + httpBody2Length + HttpBody3.length());
  String HttpHeader;

  HttpHeader = String("POST /v1/speech:recognize?key=") + ApiKey
               + String(" HTTP/1.1\r\nHost: speech.googleapis.com\r\nContent-Type: application/json\r\nContent-Length: ") + ContentLength + String("\r\n\r\n");

  client.print(HttpHeader);
  client.print(HttpBody1);
  CreateWavHeader(paddedHeader,wavDataSize*segmentValue);
  String enc = base64::encode(paddedHeader, 48);
  enc.replace("\n", "");
  client.print(enc);
}
void CloudSpeechClient::Transcribe(HardwareSerial& MP3) {
  CloudSpeechClient::preTranscribe();
  PrintHttpBody2();
  String HttpBody3 = "\"}}\r\n\r\n";
  client.print(HttpBody3);
  String My_Answer="";
  while (!client.available());
  while (client.available())
  {
    char temp = client.read();
    My_Answer = My_Answer + temp;
   // Serial.write(client.read());
  }
  Serial.println("Recording Completed. Now Processing...");
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(RED_PIN, HIGH);
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
if( ans.indexOf("\"0s") >= 0 ){
  ESP.restart();
}


///Display color thats requested

Displaylight(transcript,MP3);
}


