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


HardwareSerial MP3(2);  
#define BLUE_PIN 25
#define GREEN_PIN 33
#define RED_PIN 32
enum ButtonState
{
    PRESSED,
    UNPRESSED
};

enum ButtonIsPressedResult
{
    NOTHING,
    SHORT_PRESS,
    LONG_PRESS
};





Adafruit_NeoPixel pixels2(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//mp3
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

Preferences preferences234;

int isButtonPressed(int buttonPin) {   
    preferences234.begin("prevBut", false);
    unsigned int previousButtonState = preferences234.getUInt("prev", HIGH);
    unsigned int currentButtonState = digitalRead(buttonPin);

    if (currentButtonState != previousButtonState) {  
        preferences234.putUInt("prev", currentButtonState);  // Save only if state changes
        preferences234.end(); 

        if (currentButtonState == PRESSED) {
            return SHORT_PRESS;  // Only trigger on button press, not release
        }
    }

    preferences234.end();
    return NOTHING;
}


// This task does all the heavy lifting for our application
void applicationTask(void *param)
{
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

      Serial.println(buffer_);
      Serial.println(convertToString(buffer_, buffer_len));
      Serial2.println(convertToString(buffer_, buffer_len));

      delay((buffer_len / 2) * DELAYFORWHITESPACE);

      ESP.restart();
    }

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
  for(int i=0; i<NUMPIXELS; i++) {
    pixels2.setPixelColor(i, pixels2.Color(255, 0, 0));
    pixels2.show();
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
     for(int i=0; i<NUMPIXELS; i++) {
    pixels2.setPixelColor(i, pixels2.Color(255, 69, 0));
    pixels2.show();
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
     for(int i=0; i<NUMPIXELS; i++) { 
    pixels2.setPixelColor(i, pixels2.Color(255, 20, 147));
    pixels2.show();
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
     for(int i=0; i<NUMPIXELS; i++) {
    pixels2.setPixelColor(i, pixels2.Color(255, 255, 255));
    pixels2.show();
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
     for(int i=0; i<NUMPIXELS; i++) { 
    pixels2.setPixelColor(i, pixels2.Color(0, 255, 0));
    pixels2.show(); 
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
     for(int i=0; i<NUMPIXELS; i++) {
    pixels2.setPixelColor(i, pixels2.Color(255, 255, 0));
    pixels2.show();
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
     for(int i=0; i<NUMPIXELS; i++) {
    pixels2.setPixelColor(i, pixels2.Color(0, 0, 255));
    pixels2.show();
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
Preferences preferences14;

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
    res = wm.autoConnect("AutoConnectAP","arduino123"); // password protected ap
    
    if(!res) {
        Serial.println("Failed to connect");
        set_volume(20,MP3);
        delay(1000);
        play_filename(1,10,MP3);
        defaultLight();
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        //ESP.restart();
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
 
      delete cloudSpeechClient;
          preferences14.begin("help",false);
          unsigned int help=preferences14.getUInt("help",0);
          if(help == 1){
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
          preferences14.end();
  }
      //delete audio;
       preferences44.end();
       ESP.restart();

  }


 
}
Preferences preferences4;
Preferences preferences23334;
Preferences preferences233345;
Preferences preferences111;

void fadeInColor(int r, int g, int b, int steps,int default1) {
      preferencesLana.begin("my-appp",false);
      if(default1 == 1){
      r = 255;
      g = 0;
      b = 0;
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
      r = 255;
      g = 69;
      b = 0;
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
        
    r = 255;
    g = 20;
    b = 147;
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

  else if(default1 == 4 || default1 == 0){ //white
        
    r = 255;
    g = 255;
    b = 255;
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
        
    r = 0;
    g = 255;
    b = 0;
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
        
    r = 255;
    g = 255;
    b = 0;
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
          
      r = 0;
      g = 0;
      b = 255;
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
     pixels2.clear();
     for(int j=0;j<12;j++){
        for (int i = 0; i <= steps; i++) {
            int red   = (r * i) / steps;
            int green = (g * i) / steps;
            int blue  = (b * i) / steps;
            pixels2.setPixelColor(j, pixels2.Color(red, green, blue));
            pixels2.show();
            delay(20);
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
            
            unsigned int default1= preferencesLana.getUInt("default", 0);
            preferencesLana.end();
            
            fadeInColor(0, 0, 0, 50,default1);//stam nums
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
             return;
      }
          if(hournow > hour || (hournow == hour && minutenow > minute)){
          night = 0;
          preferences233345.putUInt("night", night);
          preferences233345.end();
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
              return;
      }
    }
  }
  boolean newState = digitalRead(ButtonPin);
  int res=isButtonPressed(ButtonPin);

  if(res == SHORT_PRESS){
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
             Serial.println("turning off");
    }
    else{
           defaultLight();
          // while (digitalRead(ButtonPin) == LOW){
            Serial.println("Turning on");

    }
  }
  preferences111.begin("help", false);
  unsigned int help = preferences111.getUInt("help", 0);

  if(help == 1){
    help=0;
    preferences111.putUInt("help", help);
    preferences111.end();
    Displaylight("set volume 30",MP3);
    for(int i=0;i<7;i++){
      delay(200);
      if(digitalRead(ButtonPin) == LOW) {
         MP3.begin(9600, SERIAL_8N1, 17, 16);
          preferencesLana.begin("my-appp",false);
          unsigned int volume1 = preferencesLana.getUInt("volume2",0);
          preferencesLana.putUInt("volume",0); 
          set_volume(30,MP3);
          unsigned int color1=preferencesLana.getUInt("counter1",0);
          unsigned int color2=preferencesLana.getUInt("counter2",0);
          unsigned int color3=preferencesLana.getUInt("counter3",0);
           pixels2.clear();
          for(int i=0; i<NUMPIXELS; i++) {
            pixels2.setPixelColor(i, pixels2.Color(color1, color2, color3));
            pixels2.show();
          }
          preferencesLana.end();

          delay(7000);
          break;
      }
        EmergencyLight();
        esp_task_wdt_init(30, false);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }     

          preferencesLana.begin("my-appp",false);
          unsigned int volume1 = preferencesLana.getUInt("volume2",0);
          preferencesLana.putUInt("volume",volume1); 
          set_volume(30,MP3);
          preferencesLana.end();
  }
  Serial.println("loop");
  esp_task_wdt_init(30, false);
  vTaskDelay(pdMS_TO_TICKS(1000));
}