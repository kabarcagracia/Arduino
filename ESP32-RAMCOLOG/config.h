
const char *ssid = "VALENTINA";
const char *password = "1197val-RNG";

// credenciales iot core TEST
const char* project_id = "mcd-demos-331211"; 
const char* location = "us-central1";
//const char* registry_id = "TEST";
//const char* device_id = "HNTSTT0001D000001"; 


// credenciales iot core LOCAL
const char* registry_id = "LOCAL";
const char* device_id = "HNLOCT0014D000007"; 

// url's servidor NTP
const char* ntp_primary = "time.google.com";
const char* ntp_secondary = "pool.ntp.org";
const char* private_key_str = 
  "02:0d:41:3f:6d:4e:3a:ee:76:b0:6d:0c:c7:85:43:"
  "56:88:a0:be:f4:76:ff:b2:6d:b6:9e:7d:bb:5d:fa:"
  "3b:15";


const int jwt_exp_secs = 60*20; // Maximum 24H (3600*24)

const char *root_cert =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIDDDCCArKgAwIBAgIUXIRd61ARosjr5tpYAQK1udlptnswCgYIKoZIzj0EAwIw\n"
  "RDELMAkGA1UEBhMCVVMxIjAgBgNVBAoTGUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBM\n"
  "TEMxETAPBgNVBAMTCEdUUyBMVFNYMB4XDTIxMDUyNTAwMDAwMFoXDTIyMDUyNDAw\n"
  "MDAwMFowbTELMAkGA1UEBhMCVVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFjAUBgNV\n"
  "BAcMDU1vdW50YWluIFZpZXcxEzARBgNVBAoMCkdvb2dsZSBMTEMxHDAaBgNVBAMM\n"
  "EyouMjAzMC5sdHNhcGlzLmdvb2cwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARR\n"
  "VX2FBT6/ZCFlCwom7Pr7jtlh99RHfH0cxO51PZ0gifi8mo2UasKfsw0ikuZvaEkG\n"
  "busnKgGwa6TrBElBabLNo4IBVzCCAVMwEwYDVR0lBAwwCgYIKwYBBQUHAwEwDgYD\n"
  "VR0PAQH/BAQDAgeAMB4GA1UdEQQXMBWCEyouMjAzMC5sdHNhcGlzLmdvb2cwDAYD\n"
  "VR0TAQH/BAIwADAfBgNVHSMEGDAWgBSzK6ugSBx+E4rJCMRAQiKiNlHiCjBpBggr\n"
  "BgEFBQcBAQRdMFswLwYIKwYBBQUHMAKGI2h0dHA6Ly9wa2kuZ29vZy9ndHNsdHNy\n"
  "L2d0c2x0c3guY3J0MCgGCCsGAQUFBzABhhxodHRwOi8vb2NzcC5wa2kuZ29vZy9H\n"
  "VFNMVFNYMCEGA1UdIAQaMBgwDAYKKwYBBAHWeQIFAzAIBgZngQwBAgIwMAYDVR0f\n"
  "BCkwJzAloCOgIYYfaHR0cDovL2NybC5wa2kuZ29vZy9HVFNMVFNYLmNybDAdBgNV\n"
  "HQ4EFgQUxp0CLjzIieJCqFTXjDc9okXUP80wCgYIKoZIzj0EAwIDSAAwRQIgAIuJ\n"
  "1QvJqFZwy6sZCP1+dXOX4YTWAbum6FtqyJwOKIACIQDENBALkXPS9jo0g8X5+eT9\n"
  "MlOQcPMMtbXGtK/ENpE2rw==\n"
  "-----END CERTIFICATE-----\n"
  "-----BEGIN CERTIFICATE-----\n"
  "MIIC0TCCAnagAwIBAgINAfQKmcm3qFVwT0+3nTAKBggqhkjOPQQDAjBEMQswCQYD\n"
  "VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G\n"
  "A1UEAxMIR1RTIExUU1IwHhcNMTkwMTIzMDAwMDQyWhcNMjkwNDAxMDAwMDQyWjBE\n"
  "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n"
  "QzERMA8GA1UEAxMIR1RTIExUU1gwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARr\n"
  "6/PTsGoOg9fXhJkj3CAk6C6DxHPnZ1I+ER40vEe290xgTp0gVplokojbN3pFx07f\n"
  "zYGYAX5EK7gDQYuhpQGIo4IBSzCCAUcwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQW\n"
  "MBQGCCsGAQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMB0GA1Ud\n"
  "DgQWBBSzK6ugSBx+E4rJCMRAQiKiNlHiCjAfBgNVHSMEGDAWgBQ+/v/MUuu/ND49\n"
  "80DQ5CWxX7i7UjBpBggrBgEFBQcBAQRdMFswKAYIKwYBBQUHMAGGHGh0dHA6Ly9v\n"
  "Y3NwLnBraS5nb29nL2d0c2x0c3IwLwYIKwYBBQUHMAKGI2h0dHA6Ly9wa2kuZ29v\n"
  "Zy9ndHNsdHNyL2d0c2x0c3IuY3J0MDgGA1UdHwQxMC8wLaAroCmGJ2h0dHA6Ly9j\n"
  "cmwucGtpLmdvb2cvZ3RzbHRzci9ndHNsdHNyLmNybDAdBgNVHSAEFjAUMAgGBmeB\n"
  "DAECATAIBgZngQwBAgIwCgYIKoZIzj0EAwIDSQAwRgIhAPWeg2v4yeimG+lzmZAC\n"
  "DJOlalpsiwJR0VOeapY8/7aQAiEAiwRsSQXUmfVUW+N643GgvuMH70o2Agz8w67f\n"
  "SX+k+Lc=\n"
  "-----END CERTIFICATE-----\n";
  
const int ex_num_topics = 0;
const char* ex_topics[ex_num_topics];
