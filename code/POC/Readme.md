## Introduction
The first thing we're going to need is some kind of "wake word detection system". This will continuously listen to audio, waiting for a trigger phrase or word.

When it hears this word it will wake up the rest of the system and start recording audio to capture whatever instructions the user has.

Once the audio has been captured it will send it off to a server to be recognised.

The server processes the audio and works out what the user is asking for.

![An Alexa System](https://blog.cmgresearch.com/assets/marvin/alexa.png)

In some systems the server may process the user request, calling out to other services to execute the user's wishes. In the system we are going to build we'll just be using the server to work out what the user's intention was and then our ESP32 will execute the command.

We'll need to build three components:

- Wake word detection
- Audio capture and Intent Recognition
- Intent Execution

We'll wire these together to build our complete system.

---
## Wake Word Detection

Let's start off with the Wake word detection. We need to create something that will tell use when a "wake" word is heard by the system. This will need to run on our embedded devices - an ideal option for this is to use TensorFlow and TensorFlow Lite.

### Training Data

Our first port of call is to find some data to train a model against. We can use the [Speech Commands Dataset](https://www.tensorflow.org/datasets/catalog/speech_commands). This dataset contains over 100,000 audio files consisting of a set of 20 core commands words such as "Up", "Down", "Yes", "No" and a set of extra words. Each of the samples is 1 second long.

One of these words in particular looks like a good candidate for a wake word - I've chosen to use the word "Marvin" for my wake word as a tribute to the android from The Hitch Hikers Guide to the Galaxy.

Here's a couple of samples of the word "Marvin":

[Marvin1](https://blog.cmgresearch.com/assets/marvin/marvin1.wav)
|[Marvin1](https://blog.cmgresearch.com/assets/marvin/marvin2.wav)

And here's a few of the other random words from the dataset:

[Forward](https://blog.cmgresearch.com/assets/marvin/forward.wav)
|[Left](https://blog.cmgresearch.com/assets/marvin/left.wav)
|[Right](https://blog.cmgresearch.com/assets/marvin/right.wav)

To augment the dataset you can also record ambient background noise, I recorded several hours of household noises and TV shows to provide a large amount of random data.

### Features

With our training data in place we need to think about what features we are going to train our neural network against. It's unlikely that feeding a raw audio waveform into our neural network will give us good results.

![Audio Waveform](https://blog.cmgresearch.com/assets/marvin/waveform.jpg)

A popular approach for word recognition is to translate the problem into one of image recognition.

We need to turn our audio samples into something that looks like an image - to do this we can take a spectrogram.

To get a spectrogram of an audio sample we break the sample into small sections and then perform a discrete Fourier transform on each section. This will give us the frequencies that are present in that slice of audio.

Putting these frequency slices together gives us the spectrogram of the sample.

![Spectrogram](https://blog.cmgresearch.com/assets/marvin/spectrogram.webp)

In the `model` folder you'll find several Jupyter notebooks. Follow the setup instructions in the `README.md` to configure your local environment.

The notebook `Generate Training Data.ipynb` contains the code required to extract our features from our audio data.

The following function can be used to generate a spectrogram from an audio sample:

```python
def get_spectrogram(audio):
    # normalise the audio
    audio = audio - np.mean(audio)
    audio = audio / np.max(np.abs(audio))
    # create the spectrogram
    spectrogram = audio_ops.audio_spectrogram(audio,
                                              window_size=320,
                                              stride=160,
                                              magnitude_squared=True).numpy()
    # reduce the number of frequency bins in our spectrogram to a more sensible level
    spectrogram = tf.nn.pool(
        input=tf.expand_dims(spectrogram, -1),
        window_shape=[1, 6],
        strides=[1, 6],
        pooling_type='AVG',
        padding='SAME')
    spectrogram = tf.squeeze(spectrogram, axis=0)
    spectrogram = np.log10(spectrogram + 1e-6)
    return spectrogram
```

This function first normalises the audio sample to remove any variance in volume in our samples. It then computes the spectrogram - there is quite a lot of data in the spectrogram so we reduce this by applying average pooling.

We finally take the log of the spectrogram so that we don't feed extreme values into our neural network which might make it harder to train.

Before generating the spectrogram we add some random noise and variance to our sample. We randomly shift the audio sample the 1-second segment - this makes sure that our neural network generalises around the audio position.

```python
# randomly reposition the audio in the sample
voice_start, voice_end = get_voice_position(audio, NOISE_FLOOR)
end_gap=len(audio) - voice_end
random_offset = np.random.uniform(0, voice_start+end_gap)
audio = np.roll(audio,-random_offset+end_gap)
```

We also add in a random sample of background noise. This helps our neural network work out the unique features of our target word and ignore background noise.

```python
# get the background noise files
background_files = get_files('_background_noise_')
background_file = np.random.choice(background_files)
background_tensor = tfio.audio.AudioIOTensor(background_file)
background_start = np.random.randint(0, len(background_tensor) - 16000)
# normalise the background noise
background = tf.cast(background_tensor[background_start:background_start+16000], tf.float32)
background = background - np.mean(background)
background = background / np.max(np.abs(background))
# mix the audio with the scaled background
audio = audio + background_volume * background
```

To make sure we have a balanced dataset we add more samples of the word "Marvin" to our dataset. This also helps our neural network generalise as there will be multiple samples of the word with different background noises and in different positions in the 1-second sample.

```python
# process all the words and all the files
for word in tqdm(words, desc="Processing words"):
    if '_' not in word:
        # add more examples of marvin to balance our training set
        repeat = 70 if word == 'marvin' else 1
        process_word(word, repeat=repeat)
```

We then add in samples from our background noise, we run through each background noise file and chop it into 1-second samples, compute the spectrogram, and add these to our negative examples.

With all of this data we end up with a reasonably sized training, validation and testing dataset.

![Marvin Spectrograms](https://blog.cmgresearch.com/assets/marvin/marvin.jpg)

Here's some examples spectrograms of the "Marvin", and here's some examples of the word "yes".

![Yes Spectrograms](https://blog.cmgresearch.com/assets/marvin/yes.jpg)

That's our training data prepared, let's have a look at how we train our model up.

### Model Training

In the `model` folder you'll find another Jupyter notebook `Train Model.ipynb`. This takes the training, test and validation data that we generated in the previous step.

For our system we only really care about detecting the word Go so we'll modify our Y labels so that it is a 1 for Go and 0 for everything else.

```python
Y_train = [1 if y == words.index('go') else 0 for y in Y_train_cats]
Y_validate = [1 if y == words.index('go') else 0 for y in Y_validate_cats]
Y_test = [1 if y == words.index('go') else 0 for y in Y_test_cats]
```

We feed this raw data into TensorFlow datasets - we set up our training data repeat forever, randomly shuffle, and to come out in batches.

```python
# create the datasets for training
batch_size = 30

train_dataset = Dataset.from_tensor_slices(
    (X_train, Y_train)
).repeat(
    count=-1
).shuffle(
    len(X_train)
).batch(
    batch_size
)

validation_dataset = Dataset.from_tensor_slices((X_validate, Y_validate)).batch(X_validate.shape[0])

test_dataset = Dataset.from_tensor_slices((X_test, Y_test)).batch(len(X_test))
```

I've played around with a few different model architectures and ended up with this as a trade-off between time to train, accuracy and model size.

We have a convolution layer, followed by a max-pooling layer, following by another convolution layer and max-pooling layer. The result of this is fed into a densely connected layer and finally to our output neuron.

```python
model = Sequential([
    Conv2D(4, 3,
           padding='same',
           activation='relu',
           kernel_regularizer=regularizers.l2(0.001),
           name='conv_layer1',
           input_shape=(IMG_WIDTH, IMG_HEIGHT, 1)),
    MaxPooling2D(name='max_pooling1', pool_size=(2,2)),
    Conv2D(4, 3,
           padding='same',
           activation='relu',
           kernel_regularizer=regularizers.l2(0.001),
           name='conv_layer2'),
    MaxPooling2D(name='max_pooling2', pool_size=(2,2)),
    Flatten(),
    Dropout(0.2),
    Dense(
        40,
        activation='relu',
        kernel_regularizer=regularizers.l2(0.001),
        name='hidden_layer1'
    ),
    Dense(
        1,
        activation='sigmoid',
        kernel_regularizer=regularizers.l2(0.001),
        name='output'
    )
])
model.summary()
```

When I train this model against the data I get the following accuracy:

| Dataset            | Accuracy |
| ------------------ | -------- |
| Training Dataset   | 0.9683   |
| Validation Dataset | 0.9567   |
| Test Dataset       | 0.9562   |

These are pretty good results for such a simple model.

If we look at the confusion matrix using the high threshold (0.9) for the true class we see that we have very few examples of background noise being classified as a "Marvin" and quite a few "Marvin"s being classified as background noise.

|        | Predicted Noise | Predicted Marvin |
| ------ | --------------- | ---------------- |
| Noise  | 13980           | 63               |
|   Go   | 1616            | 11054            |

This is ideal for our use case as we don't want the device waking up randomly.

### Converting the model to TensorFlow Lite

With our model trained we now need to convert it for use in TensorFlow Lite. This conversion process takes our full model and turns it into a much more compact version that can be run efficiently on our micro-controller.

In the `model` folder there is another workbook `Convert Trained Model To TFLite.ipynb`.

This notebook passes our trained model through the `TFLiteConverter` along with examples of input data. Providing the sample input data lets the converter quantise our model accurately.

Once the model has been converted we can run a command-line tool to generate C code that we can compile into our project.

```
xxd -i converted_model.tflite > model_data.cc
```

---

## Wiring it all up

The `tfmicro` library contains the code from TensorFlow Lite and includes everything needed to run a TensorFlow Lite mode.

The `neural_network` library contains a wrapper around the TensorFlow Lite code making it easier to interface with the rest of our project.

To get audio data into the system we use the `audio_input` library. We can support both I2S microphones directly and analogue microphones using the analogue to digital converter. Samples from the microphone are read into a circular buffer with room for just over 1 seconds worth of audio.

Our audio output library `audio_output` supports playing WAV files from SPIFFS via an I2S amplifier.

To actually process the audio we need to recreate the same process that we used for our training data. This is the job of the `audio_processor` library.

The first thing we need to do is work out the mean and max values of the sample so that we can normalise the audio.

```c++
int startIndex = reader->getIndex();
// get the mean value of the samples
float mean = 0;
for (int i = 0; i < m_audio_length; i++)
{
    mean += reader->getCurrentSample();
    reader->moveToNextSample();
}
mean /= m_audio_length;
// get the absolute max value of the samples taking into account the mean value
reader->setIndex(startIndex);
float max = 0;
for (int i = 0; i < m_audio_length; i++)
{
    max = std::max(max, fabsf(((float)reader->getCurrentSample()) - mean));
    reader->moveToNextSample();
}
```

We then step through the 1 second of audio extracting a window of samples on each step and computing the spectrogram at each step.

The input samples are normalised and copied into our FFT input buffer. The input to the FFT is a power of two so there is a blank area that we need to zero out.

```c++
// extract windows of samples moving forward by step size each time and compute the spectrum of the window
for (int window_start = startIndex; window_start < startIndex + 16000 - m_window_size; window_start += m_step_size)
{
    // move the reader to the start of the window
    reader->setIndex(window_start);
    // read samples from the reader into the fft input normalising them by subtracting the mean and dividing by the absolute max
    for (int i = 0; i < m_window_size; i++)
    {
        m_fft_input[i] = ((float)reader->getCurrentSample() - mean) / max;
        reader->moveToNextSample();
    }
    // zero out whatever else remains in the top part of the input.
    for (int i = m_window_size; i < m_fft_size; i++)
    {
        m_fft_input[i] = 0;
    }
    // compute the spectrum for the window of samples and write it to the output
    get_spectrogram_segment(output_spectrogram);
    // move to the next row of the output spectrogram
    output_spectrogram += m_pooled_energy_size;
}
```

Before performing the FFT we apply a Hamming window and then once we have done the FFT we extract the energy in each frequency bin.

We follow that by the same average pooling process as in training. And then finally we take the log.

```c++
// apply the hamming window to the samples
m_hamming_window->applyWindow(m_fft_input);
// do the fft
kiss_fftr(
    m_cfg,
    m_fft_input,
    reinterpret_cast<kiss_fft_cpx *>(m_fft_output));
// pull out the magnitude squared values
for (int i = 0; i < m_energy_size; i++)
{
    const float real = m_fft_output[i].r;
    const float imag = m_fft_output[i].i;
    const float mag_squared = (real * real) + (imag * imag);
    m_energy[i] = mag_squared;
}
// reduce the size of the output by pooling with average and same padding
float *output_src = m_energy;
float *output_dst = output;
for (int i = 0; i < m_energy_size; i += m_pooling_size)
{
    float average = 0;
    for (int j = 0; j < m_pooling_size; j++)
    {
        if (i + j < m_energy_size)
        {
            average += *output_src;
            output_src++;
        }
    }
    *output_dst = average / m_pooling_size;
    output_dst++;
}
// now take the log to give us reasonable values to feed into the network
for (int i = 0; i < m_pooled_energy_size; i++)
{
    output[i] = log10f(output[i] + EPSILON);
}
```

This gives us the set of features that our neural network is expecting to see.

Our application consists of a very simple state machine - we can be in one of two states - we can either be waiting for the wake word, or we can recognising a command.

When we are waiting for the wake word we process the audio as it streams past grabbing a 1-second window of samples and feeding it through the audio processor and neural network.

When the neural network detects the wake word we switch into the command recognition state.

