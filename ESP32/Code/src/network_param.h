#ifndef _NETWORK_PARAM_H
#define _NETWORK_PARAM_H

const char *ssid = "";
const char *password = "";

// NTP Server
//const char* ntpServer = "ntp.technion.ac.il";
const char* ntpServer = "time.nist.gov";
// Time Zone rule for Israel (IST to IDT daylight saving)
const char* time_zone = "";



//google's api
const char*  server = "speech.googleapis.com";

// To get the certificate for your region run:
// openssl s_client -showcerts -connect speech.googleapis.com:443
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.
const char* root_ca= 
"-----BEGIN CERTIFICATE-----\n"

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


#endif  // _NETWORK_PARAM_H
