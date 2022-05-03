//credenciales movistar
String apn = "wap.tmovil.cl";
String userApn = "wap";
String pwdApn = "wap";

String topicinit = "devices/HNTSTT0006D000006/events";
String topicsubscribe = "/devices/HNTSTT0006D000006/commands/#";
String publishtopic = "/devices/HNTSTT0006D000006/events";
String msginit = "SIM7600 Connected!";

String broker = "mqtt.2030.ltsapis.goog";
String port = "8883";
String user = "ignored";
static unsigned long lastMillis = 0;
bool downloadCert = false;
bool flagsub = false;
bool publishdata = true;
bool flaggpsavailable = true;
bool flagdatagps = false;
bool flagsignalgprs = false;
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
bool flag5 = false;
String extract = "init extract";
String payload = "payload_test";
String signal_gprs = "";
