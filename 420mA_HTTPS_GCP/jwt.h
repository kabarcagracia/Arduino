#include <CloudIoTCoreMqtt.h>

// Cloud iot details.
const char *project_id = "mcd-demos-331211";
const char *location = "us-central1";
const char *registry_id = "TEST";
const char *device_id = "HNTSTT0005D000005";

const char *private_key_str =
    "7a:fb:79:71:6e:21:0e:30:bb:09:f3:bd:83:00:3b:"
    "5d:cf:0f:df:06:d3:3b:cc:b5:ca:c7:c5:99:5b:88:"
    "3b:45";

CloudIoTCoreDevice *device;

String createJWT(unsigned long iat, int jwt_exp_secs){
    device = new CloudIoTCoreDevice(project_id, location, registry_id, device_id, private_key_str);
    String jwt = device->createJWT(iat, jwt_exp_secs);
    return jwt;
}
