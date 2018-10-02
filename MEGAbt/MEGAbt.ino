#include <MEGAbt.h>

MEGAbt conn; // BlueTooth connection

// Used to periodically check BlueTooth connection status
unsigned long timePrev = millis();
unsigned long timeCur;
unsigned long timeLapsed;
unsigned long period = 5000;  // Time (ms) between checks

void setup() {
  // put your setup code here, to run once:
  
  conn.begin(9600); // Initialise
  
  //String dataSet[10] = {"one", "two", "three", "four", "special"};
  //conn.arrayTest(dataSet, 10);

    
}

void loop() {
  // put your main code here, to run repeatedly:
  
  conn.receiveBTSerial(); // listen for inbound data
  conn.sendFromSerial();  // listen for outbound data

  getBTStatus();
}


/*
 * Print connection status of BTLE
 */
void getBTStatus () {
  
  timeCur = millis();
  timeLapsed = timeCur - timePrev;
  
  if (timeLapsed >= period) {
    timePrev = millis();
    int connectionStatus = conn.getStatus();
    if (connectionStatus == 1) {
      Serial.println("Connected");
    } else if (connectionStatus == 0) {
      Serial.println("No Connection");
    }
  }
}
