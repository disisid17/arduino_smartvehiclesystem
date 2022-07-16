#include <ArduinoBLE.h>

int alarmState;
int connectState;

void setup() {
  pinMode(12,INPUT);
  alarmState=0;
  connectState=0;
  Serial.begin(9600);
  while (!Serial);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  Serial.println("BLE Central scan");
  BLE.scan();
}
void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
  if (peripheral.localName() == "ChildSafetyPeripheral" && alarmState == 0) {//if the peripheral found is called childsafetyperopheral and the alarm has not activated yet
   //next few lines display some info to the serial
    Serial.print("RSSI: ");
    Serial.println(peripheral.rssi());
    Serial.println("Peripheral Name:  " + peripheral.localName());
    Serial.println();
    if (peripheral.connect()) { Serial.println("Successfully connected to " + peripheral.localName());} //Write a message when successful connection  
    while (peripheral.connect()) { //after the connection, while it is still connected
      BLE.stopScan();
      connectState=1; } // defines the variable as 1 so that when it is called later it confirms the connection was established at least once
      if(!peripheral.connect()) { //after the peripheral is disconnected and while it is not connected
        Serial.println("Disconnected from ChildSafetyPeripheral!!"); } //Write a message when peripheral is disconnected
   BLE.scan();//start scanning again
  } 
    else if (peripheral.localName() == "ChildSafetyPeripheral" && alarmState == 1 ) {//if the peripheral is childsafetyperipheral but the alarm is on it will turn it off and stay connected
    if (peripheral.connect()) { Serial.println("Successfully re-connected to " + peripheral.localName()); 
                               Serial.println("Disabling alarms!");} //Write a message when successful connection  
    while (peripheral.connect()) {
      BLE.stopScan();
      digitalWrite(11,LOW);   //turn off the level 2 alarm since we have reconnect now
      alarmState = 0;         //since we have reconnected, reset the alaram state
      connectState=1; }
   BLE.scan();
   }
    else {
    if (digitalRead(12) == HIGH && connectState == 1){//if the devices were connected previously and then disconnected and now motion is detected, sound the level 2 alarm
      digitalWrite(11,HIGH);   
      alarmState = 1; //signal that level 2 alarm has been raised
      connectState=0; //reset connected state
      Serial.println("WARNING!!!!! ChildSafetyPeripheral not found, but motion detected!!!!");}
    Serial.println("Failed to connect since ChildSafetyPeripheral not found! Will keep trying to connect.");
    BLE.scan();
  }

    } else{
      BLE.scan();
    }
  }
