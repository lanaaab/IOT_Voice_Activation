#include <Arduino.h>
#include "I2SSampler.h"
#include <driver/i2s.h>
#include <algorithm>
#include "RingBuffer.h"
#include <Preferences.h>
void I2SSampler::addSample(int16_t sample)
{
    // store the sample
    m_write_ring_buffer_accessor->setCurrentSample(sample);
    if (m_write_ring_buffer_accessor->moveToNextSample())
    {
        // trigger the processor task as we've filled a buffer
        xTaskNotify(m_processor_task_handle, 1, eSetBits);
    }
}

void i2sReaderTask(void *param)
{
    I2SSampler *sampler = (I2SSampler *)param;
    while (true)
    {
        //Serial.println("whilereader");
        // wait for some data to arrive on the queue
        i2s_event_t evt;
        if (xQueueReceive(sampler->m_i2s_queue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_RX_DONE)
            {
                size_t bytesRead = 0;
                do
                {
                    // read data from the I2S peripheral
                    uint8_t i2sData[1024];
                    // read from i2s
                    i2s_read(sampler->getI2SPort(), i2sData, 1024, &bytesRead, 10);
                    // process the raw data
                    sampler->processI2SData(i2sData, bytesRead);
                } while (bytesRead > 0);
            }
        }
        
    }
}

I2SSampler::I2SSampler()
{
    m_audio_buffers = (AudioBuffer **)malloc(sizeof(AudioBuffer **) * AUDIO_BUFFER_COUNT);

    // allocate the audio buffers
    for (int i = 0; i < AUDIO_BUFFER_COUNT; i++)
    {
        m_audio_buffers[i] = new AudioBuffer();
    }
    m_write_ring_buffer_accessor = new RingBufferAccessor(m_audio_buffers, AUDIO_BUFFER_COUNT);
    BITS_PER_SAMPLE = I2S_BITS_PER_SAMPLE_32BIT;
}

Preferences preferences11;
void I2SSampler::start(i2s_port_t i2s_port, i2s_config_t &i2s_config, TaskHandle_t processor_task_handle)
{
    //preferences11.begin("my-app", false);
    //unsigned int counter2 = preferences11.getUInt("counter22", 0);
    //Serial.println("Starting2 i2s");
      m_i2s_port = i2s_port;
      //Serial.println("Starting2 i2s");
    m_processor_task_handle = processor_task_handle;
    //Serial.println("Starting2 i2s");
    // if(counter2 == 1){
    //     Serial.println("Starting2 i2s");
    //      //install and start i2s driver
    //     return;
    //     Serial.println("Starting2 i2s");
    // }
    //Serial.println("Starting2 i2s");
    i2s_driver_install(m_i2s_port, &i2s_config, 4, &m_i2s_queue);
    // counter2=1;
    // preferences11.putUInt("counter22", counter2);
    // preferences11.end();
    //i2s_driver_uninstall(m_i2s_port);
    // set up the I2S configuration from the subclass
    configureI2S();
    // start a task to read samples
    xTaskCreatePinnedToCore(i2sReaderTask, "i2s Reader Task", 4096, this, 1, &m_reader_task_handle, 0);
}

RingBufferAccessor *I2SSampler::getRingBufferReader()
{
    RingBufferAccessor *reader = new RingBufferAccessor(m_audio_buffers, AUDIO_BUFFER_COUNT);
    // place the reader at the same position as the writer - clients can move it around as required
    reader->setIndex(m_write_ring_buffer_accessor->getIndex());
    return reader;
}


// int I2SSampler::Read(char* data, int numData) {
//   size_t bytes_read;
//   Serial.println("\r\before messi35!\r\n");
//   i2s_read(I2S_NUM_0, (void*)data, numData, &bytes_read, portMAX_DELAY);
//   Serial.println("\r\before messi4!\r\n");
//   return bytes_read;
// }

// int I2SSampler::GetBitPerSample() {
//   return (int)BITS_PER_SAMPLE;
// }

