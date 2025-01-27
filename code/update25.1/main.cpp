#include <Arduino.h>
#include <driver/i2s.h>
#include "I2SMicSampler.h"
#include "config.h"
#include "CommandDetector.h"
#include <esp_task_wdt.h>
#include <SoftwareSerial.h>
#include <Wire.h>
//#include <WiFiManager.h>
#include "Audio.h"
#include "CloudSpeechClient.h"
#include <ArduinoJson.h>
const char* ntpServer1 = "time.nist.gov";
const char* time_zone1 = "IST-2IDT,M3.5.0/3,M10.5.0/4";  // TimeZone rule for Israel

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
void setup()
{  
  
  Serial.begin(115200);
  // /////////////////////////////////wifi manager

  // WiFiManager wifiManager;
  // wifiManager.resetSettings();
  
  // // Create a WiFiManager object
  // WiFiManager wifiManager;

  // // Set a custom Access Point (AP) name and password
  // const char* apName = "ESP32-Config";
  // const char* apPassword = "IOT";

  // // Automatically connect, or start the configuration portal with the AP credentials
  // if (!wifiManager.autoConnect(apName, apPassword)) {
  //   Serial.println("Failed to connect and hit timeout. Restarting...");
  //   ESP.restart(); // Restart the ESP32 if it fails to connect
  // }

  // // Connected to WiFi
  // Serial.println("Connected to Wi-Fi!");
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());
  // /////////////////////////////////

  //pinMode(18, INPUT); // buzzer
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(WIFIPIN, OUTPUT);
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
      delay(500);
      Serial.println("\r\nRecord start!\r\n");
      set_volume(20,MP3);
      play_filename(1, 11, MP3);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(RED_PIN, LOW);

      Audio* audio = new Audio(ICS43434);
      //Audio* audio = new Audio(M5STACKFIRE);
      audio->Record();
      Serial.println("Recording Completed. Now Processing...");
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
      CloudSpeechClient* cloudSpeechClient = new CloudSpeechClient(USE_APIKEY,MP3);
      cloudSpeechClient->Transcribe(audio,MP3);

      delete cloudSpeechClient;
      delete audio;
      ESP.restart();

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
      return;
      //some sort of feedback
    }
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
      unsigned int timer = 0;
      preferences2333.putUInt("timer", timer);
      preferences2333.end();
      pixels2.clear();
       for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        // Here we're using a moderately bright green color:
        pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
        pixels2.show();   // Send the updated pixel colors to the hardware.
       //delay(DELAYVAL); // Pause before next pass through loop
     }
    }
  }
  Serial.println("loop");
  esp_task_wdt_init(30, false);
  vTaskDelay(pdMS_TO_TICKS(1000));
}