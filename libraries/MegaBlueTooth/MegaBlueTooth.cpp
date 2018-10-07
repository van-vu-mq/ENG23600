/*
  Source code for the Mega BlueTooth library.
  Functions outlined in the header file is implement here.
*/

#include <AltSoftSerial.h>
#include <MegaBlueTooth.h>

int connectionStatusPin;
String UnoMAC;


/************************/
/*    Initialize    */
/************************/

MegaBlueTooth::MegaBlueTooth() {
  connectionStatusPin = 13;
  UnoMAC = "04B167086527";  // tomy's phone
}

/*
  @desc Initialize the BlueTooth connections
  @param int baudRate
  @return
*/
void MegaBlueTooth::begin(int baudRate) {
  pinMode(connectionStatusPin, INPUT);
  Serial.begin(baudRate);
  while (!Serial);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  
  Serial1.begin(baudRate);
  Serial.println("Serial started at " + String(baudRate));

  doATCommandSetup();
}

/*
  @desc Return the paired status of the BlueTooth module.
  @param
  @return boolean - true if paired, false if not paired
*/
int MegaBlueTooth::getConnectionStatus() {
  /*
    Polls the state pin several times and checks whether it is BLINKING or HIGH.
    HM-10 BLE module BLINKs every 500ms when not paired. Polling needs to have sufficient fidelity to account for this timing.
  */

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
      sPin = digitalRead(connectionStatusPin);
      if (sPin == 0) {
        return 0;
      }
      timePrev = millis();
      i++;
    }
  }
  return 1;
}

/*
  @desc executes a predefined set of AT commands
  @param
  @return
*/
void MegaBlueTooth::doATCommandSetup() {
  if (getConnectionStatus() == 0) {
    changeRole(1);
    changeName("Mega");
  } else {
    Serial.println("Error.\nBlueTooth is currently paired, unable to perform AT commands");
  }
}

/*
  @desc Change the name of the BLE 
  @param String name
  @return
*/
void MegaBlueTooth::changeName(String name) {
  // TODO
  // Timeout
  // merge reading response with other AT functions
	
  String res1 = "OK";
  String res2 = "Set";
  String response = "";

  Serial1.print("AT+NAME" + name);

  response = atResponse();
  if (response.indexOf(res1) > -1) {
    if (response.indexOf(res2) > -1) {
      if (response.indexOf(name) > -1) {
        Serial.println("BLE name changed to " + name);
      }
    }
  }
}

/*
  @desc Changes the role of the BLE
  @param int role. 0=slave, 1=master
  @return
*/
void MegaBlueTooth::changeRole(int role) {
	// TODO
	// Timeout
	// merge reading response with other AT functions
	
  String res1 = "OK";
  String res2 = "Set";
  String response = "";
  String r = "error";
  if (role == 0) {
    r = "slave";
  } else if (role == 1) {
    r = "master";
  }
  //unsigned long timeout = 10 000;

  Serial1.print("AT+ROLE" + String(role));

  response = atResponse();

  if (response.indexOf(res1) > -1) {
    if (response.indexOf(res2) > -1) {
      if (response.indexOf(String(role)) > -1) {
        Serial.println("BLE role changed to " + r);
      }
    }
  }
}

/*
  @desc Listens on the AltSoftSerial ports for a response. Reads response into a string.
  @param
  @return String - response
*/
String MegaBlueTooth::atResponse() {
  String response = "";
  while (!Serial1.available()) {}
  delay(150);
  while (Serial1.available()) {
    char c = Serial1.read();
    response.concat(c);
  }
  Serial.println(response);
  return response;
}


/************************/
/*    Transmit    */
/************************/

/*
  @desc Handles the transmission process for a single given string
  @param String data - message to be sent
  @return boolean - success  or failure to send or be recieved by paired device.
*/
boolean MegaBlueTooth::sendData(String data) {

}

/*
  @desc Handles the tranmission process for an array of data (String)
  @param String data[] - array of message to be sent
  @return boolean - success  or failure to send or be recieved by paired device.
*/
boolean MegaBlueTooth::sendArray(String data[]) {

}

/*
  @desc Encrypts a given string.
  @param String data
  @return String - the data in encrypted form
*/
String MegaBlueTooth::encrypt(String data) {

}

/*
  @desc Add markers to delimite end/beginning of file and lines
  @param String data
  @return String - data with added markers
*/
String MegaBlueTooth::addMarker(String data) {

}

/*
  @desc Add extra bits to the data to be used for error checking after transmission
  @param byte data - encrypted data in byte format
  @return byte - data with checksum bits
*/
byte MegaBlueTooth::addCheckSum(byte data) {

}

/************************/
/*    Receive     */
/************************/

/*
  @desc Handles incoming data from the BTLE and the necessary processes before passing the data to the next destination.
  @param
  @return String - the data after processing
*/
String MegaBlueTooth::receiveData() {

}

/*
  @desc Undoes the encryption on given String
  @param String data - encrypted datas
  @return String - unencrypted data
*/
String MegaBlueTooth::decrypt(String data) {

}

/*
  @desc Reads the checksum of given data
  @param byte data
  @return boolean - TRUE if data and checksum matches
  @return boolean - FALSE if data and checksum does not match
*/
boolean MegaBlueTooth::confirmCheckSum(byte data) {

}

/*
  @desc Removes the added markers for transmission from given data
  @param String data - data that contains markers
  @return String - data without markers
*/
String MegaBlueTooth::removeMarker(String data) {

}


/************************/
/*    	Test      		*/
/************************/

/*
  @desc Reads input from Serial Monitor and transmit it through BlueTooth
  @param
  @return
*/
void MegaBlueTooth::readFromSerialToBT() {
  char c;
  if (Serial.available()) {
    c = Serial.read();
    Serial1.print(c);
    Serial.print(c);
  }
}

/*
  @desc Reads data receieved from BTLE and prints it to Serial Monitor
  @param
  @return
*/
void MegaBlueTooth::readFromBlueTooth() {
  char c;
  if (Serial1.available()) {
    c = Serial1.read();
    Serial.write(c);
  }
}

/*
  @desc Reads a static array and prints it to Serial Monitor.
  This function is for testing purposes
  @param String dataSet[]
  @param arraySize - the array size to limit memory reading in case where a pointer is passed instead of an array
  @return
*/
void MegaBlueTooth::readArray(String dataSet[], int arraySize) {
  Serial.println("\nReceiving array test starting...");
  for (int i = 0; i < arraySize; i++) {
    String line = dataSet[i];
    if (!line.equals(NULL)) {
      Serial.println(String(i) + ": " + String(dataSet[i]));
    } else {
      Serial.println(String(i) + ": empty line in array");
    }
  }
  Serial.println("Receiving array test complete.");

}



