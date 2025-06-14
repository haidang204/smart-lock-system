#include <Arduino.h>
#include "smartlocksystem.h"

void setup() {
  smartHomeSystemInit();
}

void loop() {
  smartHomeSystemUpdate();
}
