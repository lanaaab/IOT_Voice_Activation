#ifndef _detect_wake_word_state_h_
#define _detect_wake_word_state_h_
#include <Preferences.h>
class I2SSampler;

class NeuralNetworkWakeWord;
class AudioProcessorWakeWord;

class CommandDetector
{
private:
    I2SSampler *m_sample_provider;
    NeuralNetworkWakeWord *m_nn_wake_word;
    AudioProcessorWakeWord *m_audio_processor_wake_word;
    unsigned long m_last_detection;

    bool isWakeWord;
    bool first_time;

public:
    CommandDetector(I2SSampler *sample_provider);
    ~CommandDetector();
    void run();
};

#endif
