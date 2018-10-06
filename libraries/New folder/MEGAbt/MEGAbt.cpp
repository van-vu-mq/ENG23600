/*
  Code file for UNO BlueTooth library
  Functionality is defined here.
*/
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <MEGAbt.h>


const int statusPin;
const String UNObt;

AltSoftSerial BTSerial;

/*
  Constructor
*/
MEGAbt::MEGAbt() {
  UNObt = "04B167086527";  // tomy's phone
  statusPin = 13;
}

/*
  Initialize the BlueTooth connections

  @param int baudRate
*/
void MEGAbt::begin(int br) {
  pinMode(statusPin, INPUT);
  Serial.begin(br);
  while (!Serial);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);

  BTSerial.begin(br);
  Serial.println("BTserial started at " + String(br));


}

/*
  Read from the Serial Monitor and transmit through the BlueTooth serial
*/
void MEGAbt::sendFromSerial() {
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
    Serial.print(c);
    if (c == 10) {
      NL = true;
    }
  }
}

/*
  Read from the BlueTooth serial. Displays the data on the serial monitor.
*/
void MEGAbt::receiveBTSerial() {
  char c;
  if (BTSerial.available()) {
    c = BTSerial.read();
    Serial.write(c);
  }
}


/*
  Pair BlueTooth the with stored MAC address
*/
void MEGAbt::connect() {
  // Make MEGA BLE module the slave
  String command = "AT+ROLE0";
  Serial.print(command);
  
  command = "AT+CON" + UNObt;
  Serial.print(command);
}

/*
  Get the current connection status of the BlueTooth module.
  Return 0 if not paired.
  Return 1 if paired.
*/
int MEGAbt::getStatus() {
  int sPin = -1;  // BLE state
  int stateCount = 10;  // number of times to poll state pin
  unsigned long stateCheckPeriod = 100; // period (ms) between polls

  unsigned long timePrev = millis();
  unsigned long timeCur;
  unsigned long timeLapsed;

  int i = 0;
  while (i < stateCount) {
    timeCur = millis();
    timeLapsed = timeCur - timePrev;
    if (timeLapsed > stateCheckPeriod) {
      sPin = digitalRead(statusPin);
      if (sPin == 0) {
        return 0;
      }
      timePrev = millis();
      i++;
    }
  }
  return 1;
}

////////////////////////////////////////////////////////////
/////     Test Functions - Delete
////////////////////////////////////////////////////////////

/*
  Prints a pre-determined string to the serial monitor

  Used for testing
*/
void MEGAbt::sendStaticData() {
  String staticData = "TestData132";
  BTSerial.print(staticData);
  Serial.print("sent");
}

/*
  Print the stored BT MAC address to the serial monitor
*/
void MEGAbt::getOtherAddr() {
  Serial.print(UNObt);
}

/*
  How to handle arrays passed as a parameter

  @param dataSet[]
  @param arraySize
*/
void MEGAbt::arrayTest(String dataSet[], int size) {
  for (int i = 0; i < size; i++) {
    String line = dataSet[i];
    if (!line.equals(NULL)) {
      Serial.println(dataSet[i]);
    } else {
      Serial.println("emptyLine");
    }
  }
  Serial.println("ArrayTest");

}












