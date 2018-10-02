/*
  Code file for UNO BlueTooth library
  Functionality is defined here.
*/
#include <AltSoftSerial.h>
#include <UBT.h>
AltSoftSerial BTSerial;

UBT::UBT() {
}
void UBT::begin(int br) {
	Serial.begin(br);
	while(!Serial);
	Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
	
	BTSerial.begin(br);
	Serial.println("BTserial started at " + String(br));	
}
void UBT::sendStaticData() {
  String staticData = "TestData132";
	BTSerial.print(staticData);
	Serial.print("sent");
}
void UBT::sendFromSerial() {
  // Read from the Serial Monitor and send to the Bluetooth module
  char c;
  boolean NL = true;
  if (Serial.available()) {
    c = Serial.read();

    // do not send line end characters to the HM-10
    if (c != 10 & c != 13 ) {
      BTSerial.write(c);
    }

    // Echo the user input to the main window.
    // If there is a new line print the ">" character.
    if (NL) {
      Serial.print("\r\n>");
      NL = false;
    }
    Serial.write(c);
    if (c == 10) {
      NL = true;
    }
  }
}
void UBT::receiveBTSerial() {
  char c;
  if (BTSerial.available()) {
    c = BTSerial.read();
    Serial.write(c);
  }
}