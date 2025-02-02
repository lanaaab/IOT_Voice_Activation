#include <Arduino.h>
#include "I2SSampler.h"
#include "AudioProcessorWakeWord.h"
#include "../lib/neural_network_wake_word/src/NeuralNetworkWakeWord.h"
#include "RingBuffer.h"
#include "CommandDetector.h"


#define BLUE_PIN 25
#define GREEN_PIN 33
#define RED_PIN 32
#define WINDOW_SIZE 320
#define STEP_SIZE 160
#define POOLING_SIZE 6
#define AUDIO_LENGTH 16000
#define DETECTION_THRESHOLD -3
#define WAIT_PERIOD 1500

CommandDetector::CommandDetector(I2SSampler *sample_provider)
{
    // save the sample provider for use later
    m_sample_provider = sample_provider;
    m_last_detection = 0;

    isWakeWord = true;
    first_time = true;
}

CommandDetector::~CommandDetector()
{
}

Preferences preferences;

void CommandDetector::run()
{
    //Serial.printf("startofrun");
    if (isWakeWord)
    {
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      digitalWrite(RED_PIN, LOW);
      Serial.println("startofruninIF");
        if (first_time)
        {
            /* Create for WakeWord */
            m_nn_wake_word = new NeuralNetworkWakeWord();
            // Serial.println("Created Neural Network Wake Word");
            //  create our audio processor
            m_audio_processor_wake_word = new AudioProcessorWakeWord(AUDIO_LENGTH, WINDOW_SIZE, STEP_SIZE, POOLING_SIZE);
            first_time = false;
        }

        // time how long this takes for stats
        long start = millis();

        // get access to the samples that have been read in
        RingBufferAccessor *reader = m_sample_provider->getRingBufferReader();

        // rewind by 1 second
        reader->rewind(16000);

        // get hold of the input buffer for the neural network so we can feed it data
        float *input_buffer = m_nn_wake_word->getInputBufferWakeWord();

        // process the samples to get the spectrogram
        m_audio_processor_wake_word->get_spectrogramWakeWord(reader, input_buffer);

        // finished with the sample reader
        delete reader;

        // get the prediction for the spectrogram
        float output = m_nn_wake_word->predictWakeWord();
        long end = millis();

        // use quite a high threshold to prevent false positives
        if (output > 0.90 && start - m_last_detection > WAIT_PERIOD)
        {
            m_last_detection = start;
            Serial.printf("P(%.2f): Detected the word Marvin\n", output);

            digitalWrite(GPIO_NUM_2, HIGH);
            delay(500);

            isWakeWord = false;
            first_time = true;
            //tone(18, 2000); // turn buzzer on
            delay(500);

            digitalWrite(GPIO_NUM_2, LOW);
            pinMode(18, INPUT);

            delete m_nn_wake_word;
            m_nn_wake_word = NULL;
            delete m_audio_processor_wake_word;
            m_audio_processor_wake_word = NULL;
            // Serial.println("destroyed Neural Network Wake Word");
            
        }
        //Serial.printf("endOfRun");
    }
    else{
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
      preferences.begin("my-app", false);
      unsigned int counter = preferences.getUInt("counter", 0);
      Serial.printf("Current counter value: %u\n", counter);
      counter = 1;
      Serial.printf("Current counter value: %u\n", counter);
      preferences.putUInt("counter", counter);
      preferences.end();
      ESP.restart();
    }
}
