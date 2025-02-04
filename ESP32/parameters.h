//main.cpp
//RGB LED pins
#define BLUE_PIN 25
#define GREEN_PIN 33
#define RED_PIN 32

//I2s.cpp
//microphone pins
#define PIN_I2S_BCLK 26 //SCK
#define PIN_I2S_LRC 22 // SW
#define PIN_I2S_DIN 21  //SD

//config.h 
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_26
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_22
#define I2S_MIC_SERIAL_DATA GPIO_NUM_21

// mp3
#define RXp2 16
#define TXp2 17

//cloudSpeechClient.h
// neopixel ring
#define PIN       12 
#define NUMPIXELS 12 


//network_param.h

//Time:
// NTP Server
const char* ntpServer = "time.nist.gov";
// Time Zone rule for Israel (IST to IDT daylight saving)
const char* time_zone = "IST-2IDT,M3.5.0/3,M10.5.0/4";


//Google's STT API

// To get the certificate for your region run:
// openssl s_client -showcerts -connect speech.googleapis.com:443
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.
const char* root_ca= 
"-----BEGIN CERTIFICATE-----\n"
"MIIF4TCCBMmgAwIBAgIQGrjkJmHkovwQsoPEsDFvzzANBgkqhkiG9w0BAQsFADA7\n"
"MQswCQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMQww\n"
"CgYDVQQDEwNXUjIwHhcNMjQxMjAyMDgzNjU4WhcNMjUwMjI0MDgzNjU3WjAiMSAw\n"
"HgYDVQQDExd1cGxvYWQudmlkZW8uZ29vZ2xlLmNvbTBZMBMGByqGSM49AgEGCCqG\n"
"SM49AwEHA0IABFoJCZwjZYuE2VKFXONgPTbGY0EuupY8pK+jjg5OGEzYO09OMiRg\n"
"40FtPfizcNUS55UhW0R6pA7BkdeS77RnzhmjggPDMIIDvzAOBgNVHQ8BAf8EBAMC\n"
"B4AwEwYDVR0lBAwwCgYIKwYBBQUHAwEwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQU\n"
"YEXz9P+8/8NNzsHnBgUBfaOWCGUwHwYDVR0jBBgwFoAU3hse7XkV1D43JMMhu+w0\n"
"OW1CsjAwWAYIKwYBBQUHAQEETDBKMCEGCCsGAQUFBzABhhVodHRwOi8vby5wa2ku\n"
"Z29vZy93cjIwJQYIKwYBBQUHMAKGGWh0dHA6Ly9pLnBraS5nb29nL3dyMi5jcnQw\n"
"ggGYBgNVHREEggGPMIIBi4IXdXBsb2FkLnZpZGVvLmdvb2dsZS5jb22CFCouY2xp\n"
"ZW50cy5nb29nbGUuY29tghEqLmRvY3MuZ29vZ2xlLmNvbYISKi5kcml2ZS5nb29n\n"
"bGUuY29tghMqLmdkYXRhLnlvdXR1YmUuY29tghAqLmdvb2dsZWFwaXMuY29tghMq\n"
"LnBob3Rvcy5nb29nbGUuY29tghcqLnlvdXR1YmUtM3JkLXBhcnR5LmNvbYIRdXBs\n"
"b2FkLmdvb2dsZS5jb22CEyoudXBsb2FkLmdvb2dsZS5jb22CEnVwbG9hZC55b3V0\n"
"dWJlLmNvbYIUKi51cGxvYWQueW91dHViZS5jb22CH3VwbG9hZHMuc3RhZ2UuZ2Rh\n"
"dGEueW91dHViZS5jb22CFWJnLWNhbGwtZG9uYXRpb24uZ29vZ4IbYmctY2FsbC1k\n"
"b25hdGlvbi1hbHBoYS5nb29nghxiZy1jYWxsLWRvbmF0aW9uLWNhbmFyeS5nb29n\n"
"ghliZy1jYWxsLWRvbmF0aW9uLWRldi5nb29nMBMGA1UdIAQMMAowCAYGZ4EMAQIB\n"
"MDYGA1UdHwQvMC0wK6ApoCeGJWh0dHA6Ly9jLnBraS5nb29nL3dyMi9vUTZueXI4\n"
"RjBtMC5jcmwwggEFBgorBgEEAdZ5AgQCBIH2BIHzAPEAdwCi4wrkRe+9rZt+OO1H\n"
"Z3dT14JbhJTXK14bLMS5UKRH5wAAAZOGuvnoAAAEAwBIMEYCIQCTyah3bl4u/aY1\n"
"ZoEEKEEiPi2WNSrmozhs7892BgZA5wIhAOd5azS9nKkOqKj4dx9c5QBqshMTSf0f\n"
"Rd/+SlkQngAPAHYATnWjJ1yaEMM4W2zU3z9S6x3w4I4bjWnAsfpksWKaOd8AAAGT\n"
"hrr52QAABAMARzBFAiEA2QMwl01kgs98qDd4xT7V4uzMw/YrYHByJ7slXdzz0m0C\n"
"ICGquc6jKZMvDGJvx63QKfxNnmlOveDdU195CbsAeEuTMA0GCSqGSIb3DQEBCwUA\n"
"A4IBAQCn4ONC4fGp/IsbnBgxbeS6Yd5fqcQN4jhJTmR4eWomWwXffi6ouvQtNlE6\n"
"7rcKeegQjOeNnnyPXX7QkVYQm6ISJWEI162N/XOeGsQaciTKXPW/HUmG4LxNA/Lv\n"
"9YYugtl4QQ+vvKcX4sc8U7nKLwOp1YW2TDAcWsU68KIZN8bzxBJ9Vr69P2ahoY5J\n"
"d70ASB+l53XqQCt6eylgwIADSfKUb79bX/KH3XEWEPeZ87CgW0Ne3rpNv3kTcpbE\n"
"9gS9VaCkbr3BsrfQlVb96mQpUcpyJuHnrFMZ9Gw4gDDME6nIuwtPHJ/8Q+cheweN\n"
"5LRumKAFhu0i1PIquW6gWPBRgmxF\n"
"-----END CERTIFICATE-----;";


// It is also possible to use "API Key" instead of "Access Token". It doesn't have time limit.
const String ApiKey = "";



