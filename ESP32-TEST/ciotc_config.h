const char *ssid = "VALENTINA";
const char *password = "1197val200826";

const char *project_id = "testmqtt-325807";
const char *location = "europe-west1";
const char *registry_id = "testMqtt2";
const char *device_id = "testDevice3";

const char* ntp_primary = "pool.ntp.org";
const char* ntp_secondary = "time.nist.gov";

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

const char *private_key_str =
 "8d:a8:29:f3:8f:bd:57:d1:ff:24:15:4f:53:6d:16:"
 "50:6e:85:2f:ea:84:8b:50:14:de:37:53:f4:85:e7:"
 "99:93";

// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 60*20*24; // Maximum 24H (3600*24)

// To get the certificate for your region run:
//   openssl s_client -showcerts -connect mqtt.googleapis.com:8883
// for standard mqtt or for LTS:
//   openssl s_client -showcerts -connect mqtt.2030.ltsapis.goog:8883
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.

const char *root_cert =
    "-----BEGIN CERTIFICATE-----\n"
   "MIIEMjCCAxqgAwIBAgIBATANBgkqhkiG9w0BAQUFADB7MQswCQYDVQQGEwJHQjEb\n"
"MBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYDVQQHDAdTYWxmb3JkMRow\n"
"GAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UEAwwYQUFBIENlcnRpZmlj\n"
"YXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4MTIzMTIzNTk1OVowezEL\n"
"MAkGA1UEBhMCR0IxGzAZBgNVBAgMEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n"
"BwwHU2FsZm9yZDEaMBgGA1UECgwRQ29tb2RvIENBIExpbWl0ZWQxITAfBgNVBAMM\n"
"GEFBQSBDZXJ0aWZpY2F0ZSBTZXJ2aWNlczCCASIwDQYJKoZIhvcNAQEBBQADggEP\n"
"ADCCAQoCggEBAL5AnfRu4ep2hxxNRUSOvkbIgwadwSr+GB+O5AL686tdUIoWMQua\n"
"BtDFcCLNSS1UY8y2bmhGC1Pqy0wkwLxyTurxFa70VJoSCsN6sjNg4tqJVfMiWPPe\n"
"3M/vg4aijJRPn2jymJBGhCfHdr/jzDUsi14HZGWCwEiwqJH5YZ92IFCokcdmtet4\n"
"YgNW8IoaE+oxox6gmf049vYnMlhvB/VruPsUK6+3qszWY19zjNoFmag4qMsXeDZR\n"
"rOme9Hg6jc8P2ULimAyrL58OAd7vn5lJ8S3frHRNG5i1R8XlKdH5kBjHYpy+g8cm\n"
"ez6KJcfA3Z3mNWgQIJ2P2N7Sw4ScDV7oL8kCAwEAAaOBwDCBvTAdBgNVHQ4EFgQU\n"
"oBEKIz6W8Qfs4q8p74Klf9AwpLQwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQF\n"
"MAMBAf8wewYDVR0fBHQwcjA4oDagNIYyaHR0cDovL2NybC5jb21vZG9jYS5jb20v\n"
"QUFBQ2VydGlmaWNhdGVTZXJ2aWNlcy5jcmwwNqA0oDKGMGh0dHA6Ly9jcmwuY29t\n"
"b2RvLm5ldC9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDANBgkqhkiG9w0BAQUF\n"
"AAOCAQEACFb8AvCb6P+k+tZ7xkSAzk/ExfYAWMymtrwUSWgEdujm7l3sAg9g1o1Q\n"
"GE8mTgHj5rCl7r+8dFRBv/38ErjHT1r0iWAFf2C3BUrz9vHCv8S5dIa2LX1rzNLz\n"
"Rt0vxuBqw8M0Ayx9lt1awg6nCpnBBYurDC/zXDrPbDdVCYfeU0BsWO/8tqtlbgT2\n"
"G9w84FoVxp7Z8VlIMCFlA2zs6SFz7JsDoeA3raAVGI/6ugLOpyypEBMs1OUIJqsi\n"
"l2D4kF501KKaU73yqWjgom7C12yxow+ev+to51byrvLjKzg6CYG1a4XXvi3tPxq3\n"
"smPi9WIsgtRqAEFQ8TmDn5XpNpaYbg==\n"
    "-----END CERTIFICATE-----\n";

// In case we ever need extra topics
const int ex_num_topics = 0;
const char* ex_topics[ex_num_topics];
//const int ex_num_topics = 1;
//const char* ex_topics[ex_num_topics] = {
//  "/devices/my-device/tbd/#"
//};
