// WIFI
const char* ssid = "VALENTINA";
const char* password = "1197val200826";

// Cloud iot details.
const char* project_id = "mcd20201120tnt";
const char* location = "us-central1";
const char* registry_id = "TNT";
const char* device_id = "HNTNTT0004D000001";

// Configuration for NTP
const char* ntp_primary = "time.google.com";
const char* ntp_secondary = "pool.ntp.org";

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
/*const unsigned char private_key[] = {
  0xe8, 0xe4, 0x37, 0xb9, 0x2b, 0xca, 0x04, 0x15, 0xa9, 0x57, 0x0d, 0x8f, 0x24, 0x4c, 0x7a,
0x1e, 0x91, 0x05, 0x77, 0xc1, 0x26, 0xb0, 0x4e, 0x97, 0x00, 0x0b, 0xcc, 0x10, 0x7f, 0xbd, 
0xb2, 0xb8
  };*/
const char* private_key =
"-----BEGIN PRIVATE KEY-----"
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDDk1h0WhX0kAZf"
"VCZnEsgT84ausJ8CimmZ5nZ/aa5iaGsYFyIV8YGN2nPQImJX5g/9yZ9Bcb3/vuzc"
"a903MX+rP6CKzRfdgguQ60Xj/xjPN0iR5Nvm/UCPrBUvIkXlvHk2E2+ZAbWY1C27"
"G+WRADm9PGIoXsKhR/tgQljgq8QpXTsQuHRALDNSTNm0LBgsRm0ElydMNjY+lMQK"
"PXBAqzgkhe2ZBYEdn7TaLAVKE+RL4NHIM+V5TzpGlNVWjEBr0EjmjkIwvDeYA3ls"
"LaVDxF/4NqPq7Lm55egysaEVOk7hUmBdysUEN7jk2kr7n8zY+92g/LTdd9w3ubjl"
"aeBwhvnpAgMBAAECggEAHhMkgQ/xXGBhentYRHVw6xCB348thJiAULhN9J3twRhJ"
"j7TO+wzeXVAAizseYGHyh5ZftRKmoxNbHTJBHGGyPkVUjMOpLdj3qltjrgHmcMcR"
"k83buxOqv+04fL0Kfp+TDCvE94R5xnyqY6GQldGxD1afyZF44JBPKahKPpJrYei9"
"cZjZJR/viD8grB1HkK8ZwHKc0v+Oe03Qlvk5Fa+MsH1cwAyDkTFyaSDaqSPB4f1o"
"LjmmKTs3xZyYEFwyCvKPW4/AXDgILcb+eK574ba4gXp/hIoFtICOm72IVe8gH8Lo"
"+ipcIFvO/y3VMmgpj8XIKUSRKSMhudeglomNVSWu4QKBgQDpV/9k11u9aDei6Xdr"
"ZROBfDS4nFWdkeiFyXIWw6uODM28IyrcYQ6epW9C+OczOYUAriz/fWjJ74B03Vng"
"WYVlpyiYFlRlNs5yneffsMRr8jApnZoA6VxYo9+5IOyGWovLn+zV243J0NrocZDz"
"k8wvDUyToI276/fibjTkh2E4zQKBgQDWkJSddRTTY3ytihWq2TeiPARpxNl/QDMH"
"Nm9ksVx5KuwIRgQB6wbCHHjJ/RVyyvJYC7RiID2XsSUzfOYb0CF5kqLii/4j0qxB"
"sg1dhe2OgzsiyqrpN5yIfRX7K/KW0WFPeAdU84ZWPEON0WcYeMTK5hK6nzZcUjnG"
"QUz3eRl1jQKBgQDhRL/ddX8ugr87epJ8To8fMW6gTuTkgqjtB8Ypresf7sJrf7Pw"
"H48Cn4mjKTHipQP+3Se52zNfx+TGba9HxITg1xh8gwok0F1MD597t2ZZI+1VL74u"
"ZRtuw33Hdr6BEkblZNY6u4PNsuCzCHg8fpHfVsJ3vsc2BZ9ArM5JVV0JfQKBgBDa"
"spHbzJL+UaDgI+uTvIcF5+LXD66rQFbks8NZG0Z5VKkhttbfu1wsGGDlfHXT801v"
"PfAnUC9GPwHYYcr1gmumVR0umfv+ZIGUlvAtwQdcz5xgByTitW0TRlLAlQG72MUB"
"TLIjnv3PJg9Rz6NgeTTm9bmrH/4Ih3lHT+BwpstRAoGAN9FzGybFucb8ro9TlC5x"
"I3XLRfpJinqu8voVI8r//wFyUvQxTao/hZNA0SeOdonJaNNpP+0POH1R7/wmexqm"
"I3Ha2rtMJEOIinxy+rGBZTjcrvpYJ72b/JUyPWWbEish2nvNH2gR86yqqbOj7Stg"
"p1tSFxv7a+B8sryKe8hQcn0="
"-----END PRIVATE KEY-----";

/*
3082010a0282010100c39358745a15f490065f54266712c813f386aeb09f
028a6999e6767f69ae62686b18172215f1818dda73d0226257e60ffdc99f
4171bdffbeecdc6bdd37317fab3fa08acd17dd820b90eb45e3ff18cf3748
91e4dbe6fd408fac152f2245e5bc7936136f9901b598d42dbb1be5910039
bd3c62285ec2a147fb604258e0abc4295d3b10b874402c33524cd9b42c18
2c466d0497274c36363e94c40a3d7040ab382485ed9905811d9fb4da2c05
4a13e44be0d1c833e5794f3a4694d5568c406bd048e68e4230bc37980379
6c2da543c45ff836a3eaecb9b9e5e832b1a1153a4ee152605dcac50437b8
e4da4afb9fccd8fbdda0fcb4dd77dc37b9b8e569e07086f9e90203010001
*/
// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 3600; // Maximum 24H (3600*24)

// Certificates for SSL on the LTS server
const char* primary_ca = "-----BEGIN CERTIFICATE-----\n"
    "MIIBxTCCAWugAwIBAgINAfD3nVndblD3QnNxUDAKBggqhkjOPQQDAjBEMQswCQYD\n"
    "VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G\n"
    "A1UEAxMIR1RTIExUU1IwHhcNMTgxMTAxMDAwMDQyWhcNNDIxMTAxMDAwMDQyWjBE\n"
    "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n"
    "QzERMA8GA1UEAxMIR1RTIExUU1IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATN\n"
    "8YyO2u+yCQoZdwAkUNv5c3dokfULfrA6QJgFV2XMuENtQZIG5HUOS6jFn8f0ySlV\n"
    "eORCxqFyjDJyRn86d+Iko0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUw\n"
    "AwEB/zAdBgNVHQ4EFgQUPv7/zFLrvzQ+PfNA0OQlsV+4u1IwCgYIKoZIzj0EAwID\n"
    "SAAwRQIhAPKuf/VtBHqGw3TUwUIq7TfaExp3bH7bjCBmVXJupT9FAiBr0SmCtsuk\n"
    "miGgpajjf/gFigGM34F9021bCWs1MbL0SA==\n"
    "-----END CERTIFICATE-----\n";

const char* backup_ca = "-----BEGIN CERTIFICATE-----\n"
    "MIIB4TCCAYegAwIBAgIRKjikHJYKBN5CsiilC+g0mAIwCgYIKoZIzj0EAwIwUDEk\n"
    "MCIGA1UECxMbR2xvYmFsU2lnbiBFQ0MgUm9vdCBDQSAtIFI0MRMwEQYDVQQKEwpH\n"
    "bG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWduMB4XDTEyMTExMzAwMDAwMFoX\n"
    "DTM4MDExOTAzMTQwN1owUDEkMCIGA1UECxMbR2xvYmFsU2lnbiBFQ0MgUm9vdCBD\n"
    "QSAtIFI0MRMwEQYDVQQKEwpHbG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWdu\n"
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEuMZ5049sJQ6fLjkZHAOkrprlOQcJ\n"
    "FspjsbmG+IpXwVfOQvpzofdlQv8ewQCybnMO/8ch5RikqtlxP6jUuc6MHaNCMEAw\n"
    "DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFFSwe61F\n"
    "uOJAf/sKbvu+M8k8o4TVMAoGCCqGSM49BAMCA0gAMEUCIQDckqGgE6bPA7DmxCGX\n"
    "kPoUVy0D7O48027KqGx2vKLeuwIgJ6iFJzWbVsaj8kfSt24bAgAXqmemFZHe+pTs\n"
    "ewv4n4Q=\n"
    "-----END CERTIFICATE-----\n";

// In case we ever need extra topics
const int ex_num_topics = 0;
const char* ex_topics[ex_num_topics];
//const int ex_num_topics = 1;
//const char* ex_topics[ex_num_topics] = {
//  "/devices/my-device/tbd/#"
//};
