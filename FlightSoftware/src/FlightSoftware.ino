#include "FlightSoftware.h"

FlightSoftware flightSoftware;

void setup() {
    flightSoftware.begin();
}

void loop() {
    flightSoftware.update();
}
