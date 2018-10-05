#include <UnoBlueTooth.h>

UnoBlueTooth conn; // BlueTooth connection

// Used to periodically check BlueTooth connection status
unsigned long timePrev = millis();
unsigned long timeCur;
unsigned long timeLapsed;
unsigned long period = 5000;  // Time (ms) between checks

void setup() {
  conn.begin(9600); // Initialise

  // Example of handing arrays passed to a function in the library.
  String dataSet[10] = {"one", "two", "three", "four", "special"};
  conn.readArray(dataSet, 10);
}

void loop() {
  getBTStatus();
  conn.readFromSerialToBT();
  conn.readFromBlueTooth();
}

/*
  Print connection status of BTLE
*/
void getBTStatus () {

  timeCur = millis();
  timeLapsed = timeCur - timePrev;

  if (timeLapsed >= period) {
    timePrev = millis();
    int connectionStatus = conn.getConnectionStatus();
    if (connectionStatus == 1) {
      Serial.println("Connected");
    } else if (connectionStatus == 0) {
      Serial.println("No Connection");
    }
  }
}
