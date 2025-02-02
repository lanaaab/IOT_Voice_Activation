#include <Arduino.h>
#include <driver/i2s.h>
#include "I2SMicSampler.h"
#include "config.h"
#include "CommandDetector.h"
#include <esp_task_wdt.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <WiFiManager.h>
#include "CloudSpeechClient.h"
#include <ArduinoJson.h>

const char* ntpServer1 = "time.nist.gov";
const char* time_zone1 = "IST-2IDT,M3.5.0/3,M10.5.0/4";  // TimeZone rule for Israel


// const String serverName = "https://api.callmebot.com/whatsapp.php?";
// const String phoneNumber = "972542553023"; // Replace with your phone number
// const String apiKey = "2123884";          // Replace with your API key


HardwareSerial MP3(2);  
#define BLUE_PIN 25
#define GREEN_PIN 33
#define RED_PIN 32

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels2(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);




//////////////////////////////////////////////////////////////////////////////


bool reset_mp3() {
  Serial.println("MP3 RESET");
  MP3.flush();  //delete all data in serial2 buffer for MP3
  int len = 5;
  for (int i = 0; i < len; i++) {
    MP3.write(reset_CMD[i]);
  }
  delay(50);              //give the mp3 player time to reset and return message
  if (MP3.available()) {  //check if there is a response from the board (not reading the response itself)
    return true;
  } else {
    return false;
  }
}

void select_SD_card() {
  Serial.println("MP3_select_SD_card");
  int len = 4;
  for (int i = 0; i <= len; i++) {
    MP3.write(select_SD_CMD[i]);
  }
}


SoftwareSerial mySerial(14, 12);

char buffer_[100];
int buffer_len;

#define DELAYFORWHITESPACE 400
#define WIFIPIN 4
#define ButtonPin 13
// i2s config for reading from both channels of I2S
i2s_config_t i2sMemsConfigBothChannels = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s microphone pins
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

String convertToString(char *a, int size)
{
  int i;
  String s = "";
  for (i = 0; i < size; i++)
  {
    s = s + a[i];
  }
  return s;
}

// This task does all the heavy lifting for our application
void applicationTask(void *param)
{
  //Serial.println("Gotintoi2sApplicationTask");
  CommandDetector *commandDetector = static_cast<CommandDetector *>(param);

  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  while (true)
  {
    buffer_len = 0;

    if (mySerial.available() >= 2)
    {
       Serial.println("if");

      for (int i = 0; mySerial.available(); i++)
      {
        buffer_[i] = mySerial.read();
        buffer_len++;
      }

      // int whiteSpaces = 1;
      // for (int i = 0; i < buffer_len; i++)
      // {
      //   if (buffer_[i] == ' ')
      //   {
      //     whiteSpaces++;
      //   }
      // }

      Serial.println(buffer_);
      Serial.println(convertToString(buffer_, buffer_len));
      Serial2.println(convertToString(buffer_, buffer_len));

      delay((buffer_len / 2) * DELAYFORWHITESPACE);

      ESP.restart();
    }
   // Serial.println("Gotintoi2sREaderTaskAFterWhile");

    // wait for some audio samples to arrive
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue > 0)
    {
      commandDetector->run();
    }
  }
}
Preferences preferences1;
Preferences preferences2333;
Preferences preferences44;
Preferences preferencesLana;

void defaultLight(){
  preferencesLana.begin("my-appp", false);
  unsigned int current= preferencesLana.getUInt("currentColor", 0);
  if(current != 0){
    preferencesLana.end();
    return;
  }
  Serial.println("defaultLight");
  unsigned int default1 = preferencesLana.getUInt("default", 0);
  if(default1 == 1){
     pixels2.clear();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(255, 0, 0));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 0;
  unsigned int our_color3 = 0;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 1);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
  }
  else if(default1 == 2){
     pixels2.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(255, 69, 0));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 69;
  unsigned int our_color3 = 0;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 2);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
}
else if(default1 == 3){
       pixels2.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(255, 20, 147));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 20;
  unsigned int our_color3 = 147;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 3);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
}

else if(default1 == 4 || default1 == 0){
       pixels2.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(255, 255, 255));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 255;
  unsigned int our_color3 = 255;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 4);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
}
else if(default1 == 5){
       pixels2.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 0;
  unsigned int our_color2 = 255;
  unsigned int our_color3 = 0;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 5);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
}
else if(default1 == 6){
        pixels2.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(255, 255, 0));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 255;
  unsigned int our_color2 = 255;
  unsigned int our_color3 = 0;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 6);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
}
else if(default1 == 7){
         pixels2.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels2.setPixelColor(i, pixels2.Color(0, 0, 255));
    pixels2.show();   // Send the updated pixel colors to the hardware.
  //delay(DELAYVAL); // Pause before next pass through loop
}
  unsigned int our_color1 = 0;
  unsigned int our_color2 = 0;
  unsigned int our_color3 = 255;
  preferencesLana.putUInt("counter1", our_color1);
  preferencesLana.putUInt("counter2", our_color2);
  preferencesLana.putUInt("counter3", our_color3);
  preferencesLana.putUInt("currentColor", 7);
  preferencesLana.putUInt("brightness", 100);
  preferencesLana.end();
}
}
void setup()
{  

  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(WIFIPIN, OUTPUT);
  pinMode(ButtonPin,INPUT_PULLUP);
   digitalWrite(GREEN_PIN, LOW);
   digitalWrite(BLUE_PIN, LOW);
   digitalWrite(RED_PIN, HIGH);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);//////////////////////////////
  preferences1.begin("my-app", false);
  unsigned int counter = preferences1.getUInt("counter", 0);
  mySerial.begin(9600);////////////////////////
  delay(1000);
  Serial.println("Starting up");

   if(counter == 0){
      // make sure we don't get killed for our long running tasks
      esp_task_wdt_init(10, false);
      I2SSampler *i2s_sampler = new I2SMicSampler(i2s_mic_pins, false);

     // create our application
     CommandDetector *commandDetector = new CommandDetector(i2s_sampler);

     // set up the i2s sample writer task
     TaskHandle_t applicationTaskHandle;
     xTaskCreatePinnedToCore(applicationTask, "Command Detect", 8192, commandDetector, 1, &applicationTaskHandle, 0);

     i2s_sampler->start(I2S_NUM_0, i2sMemsConfigBothChannels, applicationTaskHandle);
   }
   if(counter == 1){

     // Serial.printf("gotinIf1");
       counter=0;
       preferences1.putUInt("counter", counter);
       preferences1.end();

    WiFi.mode(WIFI_STA);
    WiFi.persistent(false);
    WiFiManager wm;
    //wm.resetSettings();
    bool res;
    wm.setConfigPortalTimeout(60);
    res = wm.autoConnect("AutoConnectAP"); // password protected ap
    
    /////should check why the is unlimited time waiting for wifi
    if(!res) {
        Serial.println("Failed to connect");
        //wm.resetSettings();
        // maybe play audio No wifi and turning on default light
        //after X amount of seconds switch to default light no wifi 
        //call default func to display regular light. no restart needed

        defaultLight();
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

       // Initiate the serial monitor.
      Serial.begin(9600);
      // Initiate the Serial MP3 Player Module.
      MP3.begin(9600, SERIAL_8N1, 17, 16);
      delay(100);  //delay for stability
      if (reset_mp3() == true) {
        Serial.println("reset MP3 success");
      } else {
        Serial.println("reset MP3 fail");
        delay(10000);
      }
      select_SD_card();  //ALWAYS select SD card at beginning
      delay(1200);       //indexing the files on your SD card will take at least 1 second. let the operation finish in the background before trying to play a file.


      Serial.begin(9600);//////////////////////////   
      CloudSpeechClient* cloudSpeechClient = new CloudSpeechClient(USE_APIKEY,MP3);
      Serial.println("\r\nRecord start!\r\n");
      set_volume(20,MP3);
      play_filename(1, 11, MP3);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
      cloudSpeechClient->Transcribe(MP3);
      //audio->Record(cloudSpeechClient);

      Serial.println("fineshed recording");
      delete cloudSpeechClient;
      //delete audio;
       preferences44.end();
       ESP.restart();

  }


 
}
Preferences preferences4;
Preferences preferences23334;
Preferences preferences233345;
Preferences preferences111;

void fadeInColor(int r, int g, int b, int steps) {
     pixels2.clear();
     for(int j=0;j<12;j++){
        for (int i = 0; i <= steps; i++) {
            int red   = (r * i) / steps;
            int green = (g * i) / steps;
            int blue  = (b * i) / steps;
            
            pixels2.setPixelColor(j, pixels2.Color(red, green, blue));
            pixels2.show();
            delay(20); // Adjust speed of transition
        }
     }
}
bool isOn = false;
unsigned long previousMillis = 0;
#define FLASH_TIME 200

void EmergencyLight(){
  unsigned long currentMillis = millis();
    pixels2.clear();
    if (currentMillis - previousMillis >= FLASH_TIME) {
        previousMillis = currentMillis; // Update timer
        
        if (isOn) {
            // Turn off all LEDs
            for (int i = 0; i < NUMPIXELS; i++) {
                pixels2.setPixelColor(i, pixels2.Color(0, 0, 0)); // Off
            }
        } else {
            // Turn on all LEDs red
            for (int i = 0; i < NUMPIXELS; i++) {
                pixels2.setPixelColor(i, pixels2.Color(255, 0, 0)); // Red
            }
        }

        pixels2.show();
        isOn = !isOn; // Toggle state
    }
}
void loop()
{
  preferences2333.begin("timer", false); 
  unsigned int timer = preferences2333.getUInt("timer", 0);
  if(timer == 1){
    Serial.println("timerison");
    unsigned int hour = preferences2333.getUInt("hour", 0);
    unsigned int minute = preferences2333.getUInt("minute", 0);
    unsigned int second = preferences2333.getUInt("second", 0);
    configTzTime(time_zone1, ntpServer1);
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      //some sort of feedback
    }
    else{
      unsigned int hour1 = timeinfo.tm_hour;
      unsigned int minute1 = timeinfo.tm_min;
      unsigned int second1 = timeinfo.tm_sec;

      Serial.println(hour1);
      Serial.println(minute1);
      Serial.println(second1);
      Serial.println(hour);
      Serial.println(minute);
      Serial.println(second);
      Serial.println("Lights off");
      if(hour1 == hour && minute1 == minute && ((second1 - second) < 10)){

          Serial.println("turning off");
          Serial.println("Lights off");
          timer = 0;
          preferences2333.putUInt("timer", timer);
          preferences2333.end();
          pixels2.clear();
          for(int i=0; i<NUMPIXELS; i++) { 
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
            pixels2.show();  
        }
             preferencesLana.begin("my-appp", false);
             unsigned int our_color1 = 0;
             unsigned int our_color2 = 0;
             unsigned int our_color3 = 0;
             preferencesLana.putUInt("counter1", our_color1);
             preferencesLana.putUInt("counter2", our_color2);
             preferencesLana.putUInt("counter3", our_color3);
             preferencesLana.putUInt("currentColor", 0);
             preferencesLana.end();
      }
          if(hour1 > hour || (hour1 == hour && minute1 > minute) || (hour1 == hour && minute1 == minute && second1 > second)){
          Serial.println("turning off");
          Serial.println("Lights off");
          timer = 0;
          preferences2333.putUInt("timer", timer);
          preferences2333.end();
          pixels2.clear();
          for(int i=0; i<NUMPIXELS; i++) { 
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
            pixels2.show();  
        }
             preferencesLana.begin("my-appp", false);
             unsigned int our_color1 = 0;
             unsigned int our_color2 = 0;
             unsigned int our_color3 = 0;
             preferencesLana.putUInt("counter1", our_color1);
             preferencesLana.putUInt("counter2", our_color2);
             preferencesLana.putUInt("counter3", our_color3);
             preferencesLana.putUInt("currentColor", 0);
             preferencesLana.putUInt("brightness", 100);
             preferencesLana.end();
      }
    }
  }
  preferences23334.begin("morning", false); 
  unsigned int morning = preferences23334.getUInt("morning", 0);
  Serial.println(morning);
  if(morning == 1){
    Serial.println("Morningison");
    unsigned int hour = preferences23334.getUInt("hour", 0);
    unsigned int minute = preferences23334.getUInt("minute", 0);
    Serial.println(hour);
    Serial.println(minute);
    configTzTime(time_zone1, ntpServer1);
    Serial.println("Morningison");
    struct tm timeinfo1;
    if(!getLocalTime(&timeinfo1)){
       Serial.println("Morningison");
      Serial.println("Failed to obtain time");
      //some sort of feedback
    }
    else{
      unsigned int hournow = timeinfo1.tm_hour;
      unsigned int minutenow = timeinfo1.tm_min;
      Serial.println(hournow);
      Serial.println(minutenow);
      Serial.println(hour);
      Serial.println(minute);
      Serial.println("Morning on");
      if(hournow == hour && minutenow == minute){
          morning = 0;
          preferences23334.putUInt("morning", morning);
          preferences23334.end();
           preferencesLana.begin("my-appp", false);
           unsigned int current= preferencesLana.getUInt("currentColor", 0);
           if( current != 0){
              preferencesLana.end();
              return;
           }

        //   pixels2.clear();
        //   for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        //     // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        //     // Here we're using a moderately bright green color:
        //     pixels2.setPixelColor(i, pixels2.Color(255, 255, 255));
        //     pixels2.show();   // Send the updated pixel colors to the hardware.
        //   //delay(DELAYVAL); // Pause before next pass through loop
        // }
             
             unsigned int our_color1 = 255;
             unsigned int our_color2 = 255;
             unsigned int our_color3 = 255;
             preferencesLana.putUInt("counter1", our_color1);
             preferencesLana.putUInt("counter2", our_color2);
             preferencesLana.putUInt("counter3", our_color3);
             preferencesLana.putUInt("currentColor", 4);
             preferencesLana.putUInt("brightness", 100);
             preferencesLana.end();
            fadeInColor(255, 255, 255, 50);
      }
    }
  }
  preferences233345.begin("night", false); 
  unsigned int night = preferences233345.getUInt("night", 0);
  Serial.println(night);
  if(night == 1){
    Serial.println("nightison");
    unsigned int hour = preferences233345.getUInt("hour", 0);
    unsigned int minute = preferences233345.getUInt("minute", 0);
    Serial.println(hour);
    Serial.println(minute);
    configTzTime(time_zone1, ntpServer1);
    Serial.println("nightison");
    struct tm timeinfo1;
    if(!getLocalTime(&timeinfo1)){
       Serial.println("nightison");
      Serial.println("Failed to obtain time");
      //some sort of feedback
    }
    else{
      unsigned int hournow = timeinfo1.tm_hour;
      unsigned int minutenow = timeinfo1.tm_min;
      Serial.println(hournow);
      Serial.println(minutenow);
      Serial.println(hour);
      Serial.println(minute);
      Serial.println("Night on");
      if(hournow == hour && minutenow == minute){
          night = 0;
          preferences233345.putUInt("night", night);
          preferences233345.end();
            preferencesLana.begin("my-appp", false);
           unsigned int current= preferencesLana.getUInt("currentColor", 0);
           if( current == 0){
              preferencesLana.end();
              return;
           }
          pixels2.clear();
          for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
            // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
            // Here we're using a moderately bright green color:
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
            pixels2.show();   // Send the updated pixel colors to the hardware.
          //delay(DELAYVAL); // Pause before next pass through loop
        }
             preferencesLana.begin("my-appp", false);
             unsigned int our_color1 = 0;
             unsigned int our_color2 = 0;
             unsigned int our_color3 = 0;
             preferencesLana.putUInt("counter1", our_color1);
             preferencesLana.putUInt("counter2", our_color2);
             preferencesLana.putUInt("counter3", our_color3);
             preferencesLana.putUInt("currentColor", 0);
             preferencesLana.end();
             set_volume(20,MP3);
              delay(1000);
              play_filename(1, 9, MP3);/////check why sound is not playing

      }
          if(hournow > hour || (hournow == hour && minutenow > minute)){
          Serial.println("turning off");
          Serial.println("Lights off");
          night = 0;
          preferences2333.putUInt("night", night);
          preferences2333.end();
          pixels2.clear();
          for(int i=0; i<NUMPIXELS; i++) { 
            pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
            pixels2.show();  
        }
             preferencesLana.begin("my-appp", false);
             unsigned int our_color1 = 0;
             unsigned int our_color2 = 0;
             unsigned int our_color3 = 0;
             preferencesLana.putUInt("counter1", our_color1);
             preferencesLana.putUInt("counter2", our_color2);
             preferencesLana.putUInt("counter3", our_color3);
             preferencesLana.putUInt("currentColor", 0);
             preferencesLana.putUInt("brightness", 100);
             preferencesLana.end();
              set_volume(20,MP3);
              delay(1000);
              play_filename(1, 9, MP3);
      }
    }
  }
  boolean newState = digitalRead(ButtonPin);
  if(newState == LOW ){
    delay(20);
    preferencesLana.begin("my-appp", false);
    unsigned int current= preferencesLana.getUInt("currentColor", 0);
    if(current != 0){
        pixels2.clear();
        for(int i=0; i<NUMPIXELS; i++) { 
          pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
          pixels2.show();  
        }
             unsigned int our_color1 = 0;
             unsigned int our_color2 = 0;
             unsigned int our_color3 = 0;
             preferencesLana.putUInt("counter1", our_color1);
             preferencesLana.putUInt("counter2", our_color2);
             preferencesLana.putUInt("counter3", our_color3);
             preferencesLana.putUInt("currentColor", 0);
             preferencesLana.putUInt("brightness", 100);
             preferencesLana.end();
             set_volume(20,MP3);
             delay(1000);
             play_filename(1, 9, MP3);
             while (digitalRead(ButtonPin) == LOW);  // Wait for button release
            delay(50);  // Extra debounce after release
    }
    else{
           defaultLight();
           while (digitalRead(ButtonPin) == LOW){
            Serial.println("LANa ANd DnSelle");
           }  // Wait for button release
            delay(50);  // Extra debounce after release
    }
  }
  preferences111.begin("help", false);
  unsigned int help = preferences111.getUInt("help", 0);
  
  if(help == 1){
    help=0;
    preferences111.putUInt("help", help);
    preferences111.end();
 
    for(int i=0;i<7;i++){
      delay(200);
      if(digitalRead(ButtonPin) == LOW) {
        reset_mp3();
        break;
      }
      EmergencyLight();
        esp_task_wdt_init(30, false);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
  
  }
  Serial.println("loop");
  esp_task_wdt_init(30, false);
  vTaskDelay(pdMS_TO_TICKS(1000));
}