#include <UnoBlueTooth.h>

UnoBlueTooth conn; // BlueTooth connection

// Used to periodically check BlueTooth connection status
unsigned long timePrev = millis();
unsigned long timeCur;
unsigned long timeLapsed;
unsigned long period = 5000;  // Time (ms) between checks

void setup() {
  // put your setup code here, to run once:
  
  conn.begin(9600); // Initialise
  
  String dataSet[10] = {"one", "two", "three", "four", "special"};
  conn.readArray(dataSet, 10);

    
}

void loop() {
  // put your main code here, to run repeatedly:

}


/*
 * Print connection status of BTLE
 */
void getBTStatus () {
  
  timeCur = millis();
  timeLapsed = timeCur - timePrev;
  
  if (timeLapsed >= period) {
    timePrev = millis();
    int connectionStatus = conn.getConnectionStatus();
    if (connectionStatus) {
      Serial.println("Connected");
    } else if (connectionStatus) {
      Serial.println("No Connection");
    }
  }
}
