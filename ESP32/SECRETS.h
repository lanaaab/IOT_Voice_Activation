#ifndef _SECRETS_H
#define _SECRETS_H


//for the SMS in help feature, replace the phone number and API key with your own
const String serverName = "https://api.callmebot.com/whatsapp.php?";
const String phoneNumber = ""; // Replace with your phone number
const String apiKey = "";          // Replace with your API key

//CloudSpeechClient.cpp
// insert your personal data here
String openWeatherMapApiKey = "";
String city = "";
String countryCode = "";


//weather
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKeyW = "";

// Replace with your location Credentials
String lat = "";
String lon = "";


//network_param.h

//Time:
// NTP Server
const char* ntpServer = "";
// Time Zone rule for Israel (IST to IDT daylight saving)
const char* time_zone = "";


const char*  server = "speech.googleapis.com";

// To get the certificate for your region run:
// openssl s_client -showcerts -connect speech.googleapis.com:443
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.
const char* root_ca = 
    "-----BEGIN CERTIFICATE-----\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "-----END CERTIFICATE-----;";



// Getting Access Token : 
// At first, you should get service account key (JSON file).
// Type below command in Google Cloud Shell to get AccessToken: 
// $ gcloud auth activate-service-account --key-file=KEY_FILE   (KEY_FILE is your service account key file)
// $ gcloud auth print-access-token
// The Access Token is expired in an hour.
// Google recommends to use Access Token.
//const String AccessToken = "";

// It is also possible to use "API Key" instead of "Access Token". It doesn't have time limit.
const String ApiKey = "";

// see https://cloud.google.com/docs/authentication?hl=ja#getting_credentials_for_server-centric_flow
// see https://qiita.com/basi/items/3623a576b754f738138e (Japanese)

#endif  // _SECRETS_H
