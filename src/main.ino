/*
 *   Tested with "WiFi Smart Socket ESP8266 MQTT"
 *   and "Sonoff - WiFi Wireless Smart Switch ESP8266 MQTT"
 *
 *   The Relay could be toggeled with the physical pushbutton
*/
#include "version.h"
#include "defaults.h"
#include "debug.h"
#include <Homie.h>


unsigned long buttonDownTime = 0;
byte lastButtonState = 1;
byte buttonPressHandled = 0;

HomieNode switchNode("switch", "switch");

bool switchOnHandler(HomieRange range, String value) {
  if (value != "true" && value != "false") return false;

  bool on = (value == "true");
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  switchNode.setProperty("on").send(value);
  Homie.getLogger() << "Switch is " << (on ? "on" : "off") << endl;
  return true;
}

void toggleRelay() {
  bool on = digitalRead(PIN_RELAY) == HIGH;
  digitalWrite(PIN_RELAY, on ? LOW : HIGH);
  switchNode.setProperty("on").send(on ? "false" : "true");
  Homie.getLogger() << "Switch is " << (on ? "off" : "on") << endl;
}

void loopHandler() {
  byte buttonState = digitalRead(PIN_BUTTON);
  if ( buttonState != lastButtonState ) {
    if (buttonState == LOW) {
      buttonDownTime     = millis();
      buttonPressHandled = 0;
    }
    else {
      unsigned long dt = millis() - buttonDownTime;
      if ( dt >= 90 && dt <= 900 && buttonPressHandled == 0 ) {
        toggleRelay();
        buttonPressHandled = 1;
      }
    }
    lastButtonState = buttonState;
  }
}

// -----------------------------------------------------------------------------
// Hardware
// -----------------------------------------------------------------------------

void setupHardware() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();
    pinMode(PIN_RELAY, OUTPUT);
    pinMode(PIN_BUTTON, INPUT);
    digitalWrite(PIN_RELAY, LOW);
}

// -----------------------------------------------------------------------------
// Homie
// -----------------------------------------------------------------------------

void setupHomie() {
  Homie_setFirmware("APP_NAME", "APP_VERSION");
  Homie.setLedPin(PIN_LED, LOW).setResetTrigger(PIN_BUTTON, LOW, 5000);
  switchNode.advertise("on").settable(switchOnHandler);
  Homie.setLoopFunction(loopHandler);
  Homie.setup();
}

void setup() {
  setupHardware();
  setupHomie();
  welcome();
  otaSetup();
}

void loop() {
  Homie.loop();
  otaLoop();
}
