#include <driver/i2s.h>
#include <math.h>

#define I2S_NUM           I2S_NUM_0 // Use I2S Port 0
#define I2S_BCLK_PIN      27        // Bit Clock
#define I2S_LRCLK_PIN     26        // Left-Right Clock (Word Select)
#define I2S_DIN_PIN       25        // Data Input
#define BUTTON_PIN        15        // GPIO for the button
#define SAMPLE_RATE       44100     // 44.1 kHz Sample Rate
#define SINE_FREQUENCY    440       // 440 Hz (A4 Note)

void setup() {
  Serial.begin(115200);

  // I2S Configuration
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK_PIN,
    .ws_io_num = I2S_LRCLK_PIN,
    .data_out_num = I2S_DIN_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  // Initialize I2S
  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM);

  // Button configuration
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor
  Serial.println("Setup complete!");
}

void loop() {
  const int amplitude = 10000; // Amplitude of sine wave
  const int samples_per_cycle = SAMPLE_RATE / SINE_FREQUENCY;
  static int sample_index = 0;
  int16_t samples[256];

 
    // Generate sine wave samples
    for (int i = 0; i < 256; i++) {
      samples[i] = (int16_t)(amplitude * sin(2.0 * M_PI * (sample_index++) / samples_per_cycle));
      if (sample_index >= samples_per_cycle) {
        sample_index = 0;
      }
    }

    // Send the samples to I2S
    size_t bytes_written;
    i2s_write(I2S_NUM, samples, sizeof(samples), &bytes_written, portMAX_DELAY);
  
  
}