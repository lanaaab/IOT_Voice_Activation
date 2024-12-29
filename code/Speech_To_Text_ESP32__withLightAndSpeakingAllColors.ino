#include "Audio.h"
#include "CloudSpeechClient.h"
#include <ArduinoJson.h>
/////////////////////////////////////////////////////////////////
// Define the Serial MP3 Player Module.
HardwareSerial MP3(2);  // Use UART2 for MP3 player communication
// GPIO17 ------------------------ MP3 TX
// GPIO16 ------------------------ MP3 RX





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


void setup() {

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


   Serial.begin(115200);
  Serial.begin(115200);
  delay(500);
  Serial.println("\r\nRecord start!\r\n");
  Audio* audio = new Audio(ICS43434);
  //Audio* audio = new Audio(M5STACKFIRE);
  audio->Record();
  Serial.println("Recording Completed. Now Processing...");
  CloudSpeechClient* cloudSpeechClient = new CloudSpeechClient(USE_APIKEY);
  cloudSpeechClient->Transcribe(audio,MP3);

  delete cloudSpeechClient;
  delete audio;

}

void loop() {
  
}



