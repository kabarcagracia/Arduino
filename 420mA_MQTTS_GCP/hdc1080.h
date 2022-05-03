#include "ClosedCube_HDC1080.h"
ClosedCube_HDC1080 hdc1080;

void inithdc(){
    hdc1080.begin(0x40);
}

float gettemp(){
    return hdc1080.readTemperature();
}

float gethum(){
    return hdc1080.readHumidity();
}
