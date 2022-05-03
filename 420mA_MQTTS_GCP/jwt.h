#include <CloudIoTCoreMqtt.h>

CloudIoTCoreDevice *device;

String createjwt(unsigned long iat, int jwt_exp_secs){
    device = new CloudIoTCoreDevice(
        project_id, 
        location, 
        registry_id, 
        device_id, 
        private_key_str
    );
    String jwt = device->createJWT(iat, jwt_exp_secs);
    return jwt;
}
