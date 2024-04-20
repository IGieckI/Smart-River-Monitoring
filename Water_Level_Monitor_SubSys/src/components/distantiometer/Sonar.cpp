#include "Sonar.h"

Sonar::Sonar(unsigned short echoP, unsigned short trigP, long maxTime) : DistanceSensor(new Pin[2]{Pin(trigP, OUTPUT), Pin(echoP, INPUT)}, 2) { }

float Sonar::getDistance() {
    digitalWrite(pins[0].getPinNumber(), LOW);
    delayMicroseconds(2);
    digitalWrite(pins[0].getPinNumber(), HIGH);
    delayMicroseconds(10);
    digitalWrite(pins[0].getPinNumber(), LOW);
    long duration = pulseIn(pins[1].getPinNumber(), HIGH);
    long distance= duration*speed_velocity;
    return distance;
}
