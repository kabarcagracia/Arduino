#include <CloudIoTCoreMqtt.h>

// Cloud iot details.
const char *project_id = "testmqtt-325807";
const char *location = "us-central1";
const char *registry_id = "testMqtt";
const char *device_id = "sim7600";

const char *private_key_str ="31:55:06:11:87:b7:a3:ea:81:74:2e:f3:54:27:32:fd:5d:6c:a3:af:53:f7:67:01:c6:72:4f:c8:dc:37:40:27";

CloudIoTCoreDevice *device;

String createjwt(unsigned long iat, int jwt_exp_secs){
    device = new CloudIoTCoreDevice(project_id, location, registry_id, device_id, private_key_str);
    String jwt = device->createJWT(iat, jwt_exp_secs);
    return jwt;
}
