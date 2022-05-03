//credentials GPRS network
String apn = "wap.tmovil.cl";
String userApn = "wap";
String pwdApn = "wap";

//credentials gcp
const char *project_id = "mcd-demos-331211";
const char *location = "us-central1";
const char *registry_id = "TEST";
const char *device_id = "HNTSTT0006D000006";
const char *private_key_str =
    "05:5e:a2:4f:ec:ab:bf:08:36:bf:29:6f:17:e8:03:"
    "59:b1:dd:54:3c:5e:cc:aa:de:f1:c6:9f:89:38:f9:"
    "45:89";

//mqtt configurations
String topicpublish = "devices/" +String(device_id)+  "/events";
String topicsubscribe = "/devices/" +String(device_id)+  "/commands/#";
String msginit = "SIM7600 Connected!";
String broker = "mqtt.2030.ltsapis.goog";
String port = "8883";
String user = "ignored";

//download certificates
bool downloadCert = false;

String statusmqtt = "";
static unsigned long lastMillis = 0;
