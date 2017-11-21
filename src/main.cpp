/*
 *
 *
 *   The Relay could be toggeled with the physical pushbutton
*/
#include "main.hpp"

// RelayNode
RelayNode relayNode("relay", PIN_RELAY, PIN_LED);

// Initialize button node with callback to button press
ButtonNode buttonNode("button", PIN_BUTTON, []() {
  relayNode.toggleRelay();
});


void setupHardware() {
      Serial.begin(SERIAL_BAUDRATE);
      Serial.println();
      Serial.println();


      Homie.getLogger() << "[setupHardware] finished\n\n\n" << endl;

}

// -----------------------------------------------------------------------------
// Setup Homie
// -----------------------------------------------------------------------------

void setupHomie() {
  Homie_setFirmware(APP_NAME, APP_VERSION);
  Homie.disableResetTrigger();
  Homie.setup();
  Homie.getLogger() << "[setupHomie] finished" << endl;
}


// -----------------------------------------------------------------------------
// setup
// -----------------------------------------------------------------------------

void setup() {
  setupHardware();
  welcome();
  otaSetup(OTA_PORT, OTA_PASS);
  setupHomie();

}


// -----------------------------------------------------------------------------
// Loop
// -----------------------------------------------------------------------------

void loop() {
  Homie.loop();
  otaLoop();
}
