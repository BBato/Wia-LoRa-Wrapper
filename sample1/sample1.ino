#include "Wia_LoRaWAN_Library.h"


Wia_LoRaWAN device;



void setup() {

  device.sendMessage("First message");
  
}

void loop() {

  device.onLoop();
  
}
