#include "CloudSpeechClient.h"
#include "network_param.h"
#include <base64.h>
#include <ArduinoJson.h>
#include <Preferences.h>
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels1(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Which pin on the Arduino is connected to the NeoPixels?



#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels



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
  WiFi.mode(WIFI_STA);//// added
  WiFi.persistent(false);//// added
  WiFi.begin(ssid, password);

  Serial.println("Connecting to wifi");
  while (WiFi.status() != WL_CONNECTED){ 
    Serial.println(".");
    delay(100);
  }
  Serial.print("Connected to wifi");
  
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
// Function to extract number from a string
int extractNumber(String input) {
  
  String number = ""; // String to hold the number part
  for (int i = 0; i < input.length(); i++) {
    if (isDigit(input[i])) { // Check if the character is a digit
      number += input[i]; // Append digit to the number string
    }
  }
  return number.toInt(); // Convert the extracted string to an integer
}

void Displaylight(String transcript,HardwareSerial& MP3){ 
  bool light = false;
  pixels1.begin(); 
  pixels1.clear();


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


  if (transcript.indexOf("time") >= 0 || transcript.indexOf("Time") >= 0){
      light=true;
      // Configure NTP and time zone handling for Israel
      configTzTime(time_zone, ntpServer);
      printLocalTime(MP3);
  }

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
          light=true;
          int t=random(0,256);
          int j=random(0,256);
          int k=random(0,256);
          preferences122.begin("my-appp", false);
          unsigned int our_color1 = t;
          unsigned int our_color2 = j;
          unsigned int our_color3 = k;
          unsigned int currentColor = 0;
          preferences122.putUInt("counter1", our_color1);
          preferences122.putUInt("counter2", our_color2);
          preferences122.putUInt("counter3", our_color3);
          preferences122.putUInt("currentColor", currentColor);
          preferences122.end();
           for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
              // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
              // Here we're using a moderately bright green color:
              pixels1.setPixelColor(i, pixels1.Color(t, j, k));
              pixels1.show();   // Send the updated pixel colors to the hardware.
              //delay(DELAYVAL); // Pause before next pass through loop
           }

  }   
  // else if (transcript.indexOf("turn off") >= 0 || transcript.indexOf("Turn Off") >= 0){
  //   light=true;
  //  pixels1.clear(); // Set all pixel colors to 'off'
  //     preferences122.begin("my-appp", false);
  //   unsigned int our_color1 = 0;
  //   unsigned int our_color2 = 0;
  //   unsigned int our_color3 = 0;
  //   preferences122.putUInt("counter1", our_color1);
  //   preferences122.putUInt("counter2", our_color2);
  //   preferences122.putUInt("counter3", our_color3);
  //   preferences122.end();
  // // The first NeoPixel in a strand is #0, second is 1, all the way up
  // //to the count of pixels minus one.
  // for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  //   // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  //   // Here we're using a moderately bright green color:
  //   pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
  //   pixels1.show();   // Send the updated pixel colors to the hardware.
  //   //delay(DELAYVAL); // Pause before next pass through loop
  // }
  //   set_volume(30,MP3);
  //     delay(1000);
  //     play_filename(1,9,MP3);
    
  // }


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
    light=true;
   pixels1.clear(); // Set all pixel colors to 'off'
      preferences122.begin("my-appp", false);
    unsigned int our_color1 = 0;
    unsigned int our_color2 = 0;
    unsigned int our_color3 = 0;
    preferences122.putUInt("counter1", our_color1);
    preferences122.putUInt("counter2", our_color2);
    preferences122.putUInt("counter3", our_color3);
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
    set_volume(30,MP3);
      delay(1000);
      play_filename(1,14,MP3);
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
    unsigned int our_color3 = 150;
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
    preferences122.end();

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
  
    }  return;
  }

  if (transcript.indexOf("turn off") >= 0 ||transcript.indexOf("Turn off") >= 0 ){
    //turn on the current coloor... if its on it is not supposed to matter, if off turn on the default color...
    light=true;
    int extractedNumber = extractNumber(transcript);
    if(extractedNumber == 0){
         pixels1.clear(); // Set all pixel colors to 'off'
      preferences122.begin("my-appp", false);
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
    preferences122.begin("timer", false);
    preferences122.putUInt("hour", hour);
    preferences122.putUInt("minute", minute);
    preferences122.putUInt("second", second);
    unsigned int timer = preferences122.getUInt("timer", 0);
    timer=1;
    preferences122.putUInt("timer", timer);
    preferences122.end();
  }


  if(!light){
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
if(transcript == "edTime" || transcript.indexOf("[") >= 0 ){
  ESP.restart();
}


///Display color thats requested

Displaylight(transcript,MP3);
}


