#include <ArduinoBLE.h>

void setup() {

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
pinMode(12, OUTPUT);
  // begin initialization
  if (!BLE.begin()) {
    
    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("ChildSafetyPeripheral");

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
     digitalWrite(12,LOW);
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);
    // set the LED on
    // while the central is connected:
    while (central.connected()) {
     digitalWrite(12,LOW);
digitalWrite(LED_BUILTIN, HIGH);
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(12,HIGH);
   
    //return;
  }
}
