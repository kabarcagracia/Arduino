#define LED_BUILTIN 13

// credenciales wi-fi
const char *ssid = "VALENTINA";
const char *password = "1197val-RNG";

// credenciales iot core
const char* project_id = "mcd-demos-331211"; 
const char* location = "us-central1";
const char* registry_id = "TEST";
const char* device_id = "HNTSTT0001D000001"; 

// url's servidor NTP
const char* ntp_primary = "time.google.com";
const char* ntp_secondary = "pool.ntp.org";

const char* private_key_str = 
  "b5:41:0b:18:dd:c9:a3:42:0b:d4:2e:b0:1a:40:9e:"
  "f7:70:37:d3:5f:c0:8d:08:15:67:fd:de:a7:43:bd:"
  "5d:8b";

// tiempo jwt 60 seg * 60 min = 1 hora
const int jwt_exp_secs = 60*60*4; // Maximum 24H (3600*24)

// certificado root
const char *root_cert =
"-----BEGIN CERTIFICATE-----"
"MIIBxTCCAWugAwIBAgINAfD3nVndblD3QnNxUDAKBggqhkjOPQQDAjBEMQswCQYD"
"VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G"
"A1UEAxMIR1RTIExUU1IwHhcNMTgxMTAxMDAwMDQyWhcNNDIxMTAxMDAwMDQyWjBE"
"MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM"
"QzERMA8GA1UEAxMIR1RTIExUU1IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATN"
"8YyO2u+yCQoZdwAkUNv5c3dokfULfrA6QJgFV2XMuENtQZIG5HUOS6jFn8f0ySlV"
"eORCxqFyjDJyRn86d+Iko0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUw"
"AwEB/zAdBgNVHQ4EFgQUPv7/zFLrvzQ+PfNA0OQlsV+4u1IwCgYIKoZIzj0EAwID"
"SAAwRQIhAPKuf/VtBHqGw3TUwUIq7TfaExp3bH7bjCBmVXJupT9FAiBr0SmCtsuk"
"miGgpajjf/gFigGM34F9021bCWs1MbL0SA=="
"-----END CERTIFICATE-----"
"-----BEGIN CERTIFICATE-----"
"MIIB4TCCAYegAwIBAgIRKjikHJYKBN5CsiilC+g0mAIwCgYIKoZIzj0EAwIwUDEk"
"MCIGA1UECxMbR2xvYmFsU2lnbiBFQ0MgUm9vdCBDQSAtIFI0MRMwEQYDVQQKEwpH"
"bG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWduMB4XDTEyMTExMzAwMDAwMFoX"
"DTM4MDExOTAzMTQwN1owUDEkMCIGA1UECxMbR2xvYmFsU2lnbiBFQ0MgUm9vdCBD"
"QSAtIFI0MRMwEQYDVQQKEwpHbG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWdu"
"MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEuMZ5049sJQ6fLjkZHAOkrprlOQcJ"
"FspjsbmG+IpXwVfOQvpzofdlQv8ewQCybnMO/8ch5RikqtlxP6jUuc6MHaNCMEAw"
"DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFFSwe61F"
"uOJAf/sKbvu+M8k8o4TVMAoGCCqGSM49BAMCA0gAMEUCIQDckqGgE6bPA7DmxCGX"
"kPoUVy0D7O48027KqGx2vKLeuwIgJ6iFJzWbVsaj8kfSt24bAgAXqmemFZHe+pTs"
"ewv4n4Q="
"-----END CERTIFICATE-----";
  
  const int ex_num_topics = 0;
  const char* ex_topics[ex_num_topics];
