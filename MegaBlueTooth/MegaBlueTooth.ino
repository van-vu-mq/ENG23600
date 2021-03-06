/*
  Source code for the Mega BlueTooth library.
  Functions outlined in the header file is implement here.
*/

// https://www.arduino.cc/reference/en/language/functions/communication/serial/
// Serial3 needs pins 15(RX) and 14(TX)


#define connectionStatusPin      13

#define packetStartMarker       '<'
#define packetEndMarker         '>'

#define dataStartMarker         '!'
#define dataEndMarker           '@'

#define lineStartMarker         '#'
#define lineEndMarker           '$'

#define checksumStartMarker     '&'
#define checksumEndMarker       '*'


String UNOMAC = "";

String *storedTransmission;
int storedSize = 0;

int redCansError;
int greenCansError;
int blueCansError;

// Change to false to reduce global variables
boolean includeErrorMessage = false;
boolean testingMessages = true;
boolean receiveTesting = false;


/************************************************************************************************************************/
/************************/
/*    Initialize        */
/************************/
/************************************************************************************************************************/

/*
  @desc Initialize the BlueTooth connections
  @param int baudRate
  @return
*/
void beginBluetooth(int baudRate) {
  Serial.begin(baudRate);
  while (!Serial);
  
  if (includeErrorMessage) {
    Serial.print("\nSketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
  }

  Serial3.begin(baudRate);
  if (includeErrorMessage) {
    Serial.println("Serial3 started at " + String(baudRate));
  }
  doATCommandSetup();
}



/************************************************************************************************************************/
/************************/
/*  Settings & Status   */
/************************/
/************************************************************************************************************************/

/*
  @desc Returns a pointer to the array holding the last received transmission
  @param
  @return String *pointer
*/
String * getBTData() {
  return storedTransmission;
}

/*
  @desc Returns the number of elements in the last received transmission
  @param
  @return int storedSize
*/
int getBTDataSize() {
  return storedSize;
}

/*
  @desc Delete the last received transmission to free up memory
  @param
  @return
*/
void clearMemory() {
  /*
     Assign empty arrays to all storage
     Reset size tracking to null
  */
  storedTransmission = new String[0];
  storedSize = 0;
}

/*
  @desc Returns the paired status of the BlueTooth module.
  @param
  @return boolean - true if paired, false if not paired
*/
boolean getConnectionStatus() {
  /*
     Polls the state pin several times and checks whether it is BLINKING or HIGH.
     HM-10 BLE module BLINKs every 500ms when not paired.
     Polling needs to have sufficient fidelity to account for this timing.
  */
  int pollCount = 7;  // number of times to poll state pin
  int pollDelay = 100; // period (ms) between polls

  while (pollCount > 0) {
    if (!digitalRead(connectionStatusPin)) {
      return false;
    }
    pollCount--;
    delay(pollDelay);
  }
  return true;
}

/*
  @desc Pairs the BTLE with the device correseponding to the stored MAC address.
  @param
  @return boolean - true if pairing successfull
  @return boolean - false if pairing unsuccessfull
*/
boolean connectBluetooth() {
  if (!canDoAT()) {
    return false;
  }
  String successFlags[] = {"OK", "Set"};
  String response = "";

  Serial3.print("AT+CON" + UNOMAC);

  response = atResponse();

  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (isATSucessfull(response, successFlags, numFlags)) {
    if (includeErrorMessage) {
      Serial.println("Bluetooth has been connected");
    }
    return true;
  } else {
    if (includeErrorMessage) {
      Serial.println("Bluetooth failed to connect");
    }
    return false;
  }
}

/*
  @desc Executes a predefined set of AT commands
  @param
  @return
*/
void doATCommandSetup() {
  if (canDoAT()) {
    changeRole(0);
    changeName("MegaBluetooth");
  }
}

/*
  @desc Changes the name of the Bluetooth module to the string given.
  Max name length is 12 characters
  @param String name
  @return
*/
void changeName(String newName) {
  String successFlags[] = {"OK", "Set", newName};
  String response = "";

  Serial3.print("AT+NAME" + newName);
  response = atResponse();

  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (includeErrorMessage) {
    if (isATSucessfull(response, successFlags, numFlags)) {
      Serial.println("BLE name changed to " + newName);
    } else {
      Serial.println("Failed to change name");
    }
  }
}

/*
  @desc Changes the role of the Bluetooth module
  @param int role. 0=slave, 1=master
  @return
*/
void changeRole(int role) {
  String successFlags[] = {"OK", "Set", String(role)};
  String response = "";
  String r = "Failed to change role";

  if (role == 0)      {
    r = "slave" ;
  }
  else if (role == 1) {
    r = "master";
  }

  Serial3.print("AT+ROLE" + String(role));
  response = atResponse();
  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (includeErrorMessage) {
    if (isATSucessfull(response, successFlags, numFlags)) {
      Serial.println("BLE role changed to " + r);
    } else {
      Serial.println(r);
    }
  }
}

/*
  @desc Check that the given response string contains all the given success flags
  @param String response - reponse given back by AT commands
  @param String successFlags[] - array of success flags
  @return boolean - true if all flags are found in the response
  @return boolean - false if not all flags are found in the response
*/
boolean isATSucessfull(String response, String successFlags[], int numFlags) {
  for (int index = 0; index < numFlags; index++) {
    if (response.indexOf(successFlags[index]) < 0) {
      return false;
    }
  }
  return true;
}

/*
  @desc Listen on the Serial port for a response, reads it and returns it as a single string.
  @param
  @return String - response
*/
String atResponse() {
  if (!canDoAT()) {
    return "ERROR";
  }

  String response = "";
  unsigned long timeout = 2000;
  unsigned long timeStart = millis();

  // Check if there is a response within timeout period
  while (!Serial3.available()) {
    if ((millis() - timeStart) > timeout) {
      if (includeErrorMessage) {
        Serial.println("AT Response Timeout");
      }
      return "TIMEOUT";
    }
  }

  // Allow full response to load into buffer
  delay(150);

  // Read response
  while (Serial3.available()) {
    char c = Serial3.read();
    response.concat(c);
  }
  if (includeErrorMessage) {
    Serial.println("\n" + response);
  }
  return response;
}

/*
  @desc Checks whether the conditions are met to execute AT commands
  @param
  @return boolean - false if not able to execute AT commands
  @return boolean - true if able to execute AT commands
*/
boolean canDoAT() {
  if (!getConnectionStatus()) {
    return true;
  } else {
    if (includeErrorMessage) {
      Serial.println("Error.\nBlueTooth is currently paired, unable to perform AT commands");
    }
    return false;
  }
}


/************************************************************************************************************************/
/************************/
/*      Transmit        */
/************************/
/************************************************************************************************************************/

/*
  @desc Handles the conversion of int array of 3 elements to be sent
  @param int data[] - array
  @return boolean - true if message is sent and received by other paired device
  @return boolean - false if message is unable to be sent or not confirmed to be received by other paired device
*/
boolean sendIntArray(int intData[]) {
  // hardcoded, predetermined size of communicated data
  // Refer to Uno back-end and Mega drive-base team
  // +1 contains information about original data type for rebuilding
  int arraySize = 4;
  String convertedData[arraySize];

  // mark original data type
  convertedData[0] = "INT";

  String s = "";
  s = s.concat(String(intData[0]));
  s = s.concat(String(intData[1]));
  s = s.concat(String(intData[2]));
  //Serial.println(s);
  for (int i = 1; i < arraySize; i++) {
    convertedData[i] = String(intData[i - 1]);
  }
  return sendData(convertedData, arraySize);
}

/*
  @desc Handles the transmission process for an array of Strings
  @param String data[] - array of message to be sent
  @param int arraySize
  @return boolean - true if message is sent and received by other paired device
  @return boolean - false if message is unable to be sent or not confirmed to be received by other paired device
*/
boolean sendData(String data[], int arraySize) {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

  // clone array so we can edit the data in memory
  String copyData[arraySize];
  memcpy(copyData, data, sizeof(data[0])*arraySize);

  // Add markers
  addMarker(&copyData[0], arraySize);
  if (testingMessages) {
    Serial.println("\nAfter adding markers");
    for (int i = 0; i < arraySize; i++) {
      Serial.println(copyData[i]);
    }
  }

  // Convert to single String
  String packet = transformToString(copyData, arraySize);
  if (testingMessages) {
    Serial.println("\nAfter transforming into a single string:");
    Serial.println(packet);
  }

  // Encrypt data
  packet = encrypt(packet);
  if (testingMessages) {
    Serial.println("\nAfter encrypting:");
    Serial.println(packet);
  }

  // Prepend checksum
  packet = addCheckSum(packet);
  if (testingMessages) {
    Serial.println("\nAfter Adding checksum:");
    Serial.println(packet);
  }

  // add packet markers
  packet = packetStartMarker + packet + packetEndMarker;

  // Write to Serial3
  int transmitAttempts = 1;
  for (int i = 0; i < transmitAttempts; i++) {
    transmitData(packet);
    if (receivedAcknowlegement()) {
      return true;
    }
  }
  return false;
}

/*
  @desc Listens on the Bluetooth Serial for acknowledgement of receiving data
  @param
  @return boolean - true if acknowledgement received
  @return boolean - false if acknowledgement not received
*/
boolean receivedAcknowlegement() {

  int timeout = 1500;
  unsigned long timePrev = millis();

  while (timePrev - millis() < timeout) {
    String ack = readFromBTBuffer();
    if (ack.equals("<ACK>")) {
      return true;
    }
  }
  return false;
}

/*
  @desc Transmit data using te Bluetooth module. Handles breaking down the data into appropriate packet lengths
  @param String packet - data to be sent
  @return boolean - true if acknowledgement received
  @return boolean - false if acknowledgement not received
*/
void transmitData(String data) {
  // NOTE: BLE 4.0 standards - can only transmit 20 bytes per packet
  // Send packet in groups of 20 bytes (equivalent of 20x char)
  int packetByteLimit = 20;
  int packetParts = data.length() /  packetByteLimit; // number of max size packets in given data

  if (testingMessages) {
    Serial.println("\nPacket being sent");
    Serial.println("Whole packet:");
    Serial.println(data);
    Serial.println("\nSent Packet as parts - BLE byte limit per packet");
  }
  //*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
  // NOTE: may need to delay time between each transmission
  // Transmit max sized packets using substring(start, end)
  for (int i = 0; i < packetParts; i++) {
    int packetStart = i * packetByteLimit;
    int packetEnd = packetStart + packetByteLimit;
    Serial3.print(data.substring(packetStart, packetEnd));
    if (testingMessages) {
      Serial.println(data.substring(packetStart, packetEnd));
    }
    //delay(100);
  }

  // Transmit tail end - non maxed sized packet using substring(start)
  Serial3.print(data.substring(packetParts * packetByteLimit));
  if (testingMessages) {
    Serial.println(data.substring(packetParts * packetByteLimit));
  }
}

/*
  @desc Generates string containing all the data within given array
  @param String dataArray[]
  @param int arraySize
  @return String
*/
String transformToString(String dataArray[], int arraySize) {
  String s = "";

  for (int i = 0; i < arraySize; i++) {
    s.concat(dataArray[i]);
  }
  return s;
}

/*
  @desc Encrypts a given string.
  @param String data
  @return String - the data in encrypted form
*/
String encrypt(String data) {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
  return data;
}

/*
  @desc Add markers to delimit end/beginning of file and lines
  @param String *data - pointer to data in memory
  @param int arraySize - number of elements in the array
  @return
*/
void addMarker(String * dataArray, int arraySize) {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

  // Add marker to every line in the array
  // markers: startData, endData, startLine, endLine

  // add marker to each line
  for (int d = 0; d < arraySize; d++) {
    *(dataArray + d) = lineStartMarker + (*(dataArray + d)) + lineEndMarker;
  }

  // add marker to beginning of data
  *(dataArray) = dataStartMarker + (*(dataArray));

  // add marker to ending of data
  *(dataArray + arraySize - 1) = (*(dataArray + arraySize - 1)) + dataEndMarker;
}

/*
  @desc Add checksum to the data to be used for error detection by recipient
  @param String data
  @return String - data prepended with checksum
*/
String addCheckSum(String data) {
  // Uses CRC8 CODE
  //CRC-8 - based on the CRC8 formulas by Dallas/Maxim
  //code released under the therms of the GNU GPL 3.0 license

  // convert data into byte representation
  uint8_t byteBuffer[data.length() + 1];
  data.getBytes(byteBuffer, data.length() + 1);
  size_t numBytes = sizeof(byteBuffer) - 1;

  // calculate checksum
  uint8_t checksum = CRC8(&byteBuffer[0], numBytes);

  // wrap checksum with markers
  // append checksum to data String
  return (checksumStartMarker + String(checksum) + checksumEndMarker + data);
}

//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
//code released under the therms of the GNU GPL 3.0 license
byte CRC8(const byte *data, byte len) {
  byte crc = 0x00;
  while (len--) {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}


/************************************************************************************************************************/
/************************/
/*     Receive          */
/************************/
/************************************************************************************************************************/

/*
  @desc Checks if there is incoming transmission on the Bluetooth Serial.
  If there is, handles the process to read and store it for access.
  @param
  @return boolean - true if there is incomming tranmission
  @return boolean - false if there is no incomming tranmission
*/

boolean receivedNewData() {
  
  // failed to get communication from Bluetooth
  String dataFromBT = readFromBTBuffer();
  if (dataFromBT.equals(""))        {
    return false;
  }
  if (dataFromBT.equals("TIMEOUT")) {
    return false;
  }

  if (testingMessages) {
    Serial.println("\nData read from BTSerial:");
    Serial.println(dataFromBT);
  }

  // remove packet markers
  dataFromBT.remove(dataFromBT.indexOf(packetStartMarker), 1);
  dataFromBT.remove(dataFromBT.indexOf(packetEndMarker), 1);

  if (testingMessages) {
    Serial.println("\nData after removing packet markers:");
    Serial.println(dataFromBT);
  }

  if (!confirmCheckSum(dataFromBT)) {
    Serial.println("failed checksum");
    return NULL;
  } else {
    // send acknowledge

    dataFromBT = removeCheckSum(dataFromBT);
    if (testingMessages) {
      Serial.println("\nData after confirming and removing checksum:");
      Serial.println(dataFromBT);
    }

    dataFromBT = decrypt(dataFromBT);

    if (testingMessages) {
      Serial.println("\nData after decrypting:");
      Serial.println(dataFromBT);
    }

    rebuildData(dataFromBT);
    if (testingMessages) {
      Serial.println("\nData after being rebuilt:");
      for (int i = 0; i < storedSize; i++) {
        Serial.println(*(storedTransmission + i));
      }
    }

    removeMarkers();
    if (testingMessages) {
      Serial.println("\nData after being removing markers:");
      for (int i = 0; i < storedSize; i++) {
        Serial.println(*(storedTransmission + i));
      }
    }
  }
  return storedTransmission;
}

/*
  @desc
  @param
  @return
*/
void sendAcknowledge() {
  Serial3.print("<ACK>");
}

/*
  @desc
  @param
  @return
*/
void writeToVariables () {
  /*
     Uno
     redCansError;
     greenCansError;
     blueCansError;
  */

  /*
     Mega
     canColours[3] = {r, g b}
  */
  if ((*storedTransmission).equals("INT")) {
    int arraySize = 3 + 1;
    redCansError = (*(storedTransmission + 1)).toInt();
    greenCansError = (*(storedTransmission + 2)).toInt();
    blueCansError = (*(storedTransmission + 3)).toInt();
  }


}

/*
  @desc
  @param
  @return
*/
void rebuildData(String dataFromBT) {
  storedSize=0;
  
  // calculate array size
  for (int i = 0; i < dataFromBT.length(); i++) {
    if (dataFromBT.charAt(i) == lineStartMarker) {
      storedSize++;
    }
  }

  // change array size storage
  storedTransmission = new String[storedSize];

  // store in parts
  for (int i = 0; i < storedSize; i++) {
    int startMarkerPos = dataFromBT.indexOf(lineStartMarker);
    int endMarkerPos = dataFromBT.indexOf(lineEndMarker);
    String line = dataFromBT.substring(startMarkerPos, endMarkerPos + 1);
    dataFromBT.remove(startMarkerPos, endMarkerPos - startMarkerPos + 1);
    *(storedTransmission + i) = line;
  }

}

/*
  @desc
  @param
  @return
*/
String readFromBTBuffer() {

  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  if (receiveTesting) {
    //temporary sample data
    String testData = "<&1999750704*!#one$#two$#test$#234324$#453sdf3243$@>";
    return testData;
  }
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


  // No communication on Bluetooth Serial
  if (Serial3.available() <= 0) {
    return "";
  }

  // markers: startPacket, endPacket
  String packet = "";
  char fromBT = Serial3.read();
  int timeout = 5000;
  unsigned long timePrev = millis();

  // read until start of packet marker is found
  while (fromBT != packetStartMarker) {
    if (millis() - timePrev >= timeout) {
      if (includeErrorMessage) {
        Serial.println("Read from buffer TIMEOUT1 - no start packet marker");
        Serial.println(packet);
      }
      return "TIMEOUT";
    }
    if (Serial3.available()) {
      fromBT = Serial3.read();
      packet = packet + fromBT;
      Serial.print(fromBT);
      timePrev = millis();
    }

  }

  // start of packet found
  // store
  packet = packet + fromBT;

  // read until end of packet marker is found
  while (fromBT != packetEndMarker) {
    if (millis() - timePrev >= timeout) {
      if (includeErrorMessage) {
        Serial.println("Read from buffer TIMEOUT2 - no end packet marker");
        Serial.println(packet);
        while (Serial3.available()) {
          Serial3.read();
        }
      }
      return "TIMEOUT";
    }
    if (Serial3.available()) {
      fromBT = Serial3.read();
      packet =  packet + fromBT;
      timePrev = millis();
    }
  }

  return packet;
}

/*
  @desc Undoes the encryption on given String
  @param String data - encrypted datas
  @return String - unencrypted data
*/
String decrypt(String data) {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
  return data;
}

/*
  @desc Reads the checksum of given data
  @param byte data
  @return boolean - TRUE if data and checksum matches
  @return boolean - FALSE if data and checksum does not match
*/
boolean confirmCheckSum(String data) {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
  // double check the index interaction of indexOf() and substring()

  // Read the data and extract the part that is the checksum using the checksum markers
  // example markers start of checksum '&', end of checksum '*'
  int csStart = data.indexOf(checksumStartMarker);
  int csEnd = data.indexOf(checksumEndMarker);
  String givenCheckSum = data.substring(csStart + 1, csEnd);

  // Caclculate the checksum on the data
  data = data.substring(csEnd + 1);

  uint8_t byteBuffer[data.length() + 1];
  data.getBytes(byteBuffer, data.length() + 1);
  size_t numBytes = sizeof(byteBuffer) - 1;

  uint8_t calculatedChecksum = CRC8(&byteBuffer[0], numBytes);

  // Compare the extracted checksum and the calculated checksum
  if (String(calculatedChecksum).equals(givenCheckSum)) {
    return true;
  } else {
    return false;
  }
}


/*
  @desc
  @param
  @return
*/
String removeCheckSum(String data) {
  int csStart = data.indexOf(checksumStartMarker);
  int csEnd = data.indexOf(checksumEndMarker);

  data.remove(csStart, csEnd - csStart + 1);
  return data;
}

/*
  @desc Removes the added markers for transmission from given data
  @param String data - data that contains markers
  @return String - data without markers
*/
void removeMarkers() {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
  char markers[] = {lineStartMarker, lineEndMarker};
  
  // for every line in the array
  for (int i = 0; i < storedSize; i++) {
    // for every marker
    for (int m = 0; m < sizeof(markers) / sizeof(markers[0]); m++) {
      // while line does not contain marker
      while ((*(storedTransmission + i)).indexOf(markers[m]) != -1) {
        // remove 1 character at index of where marker is found
        (*(storedTransmission + i)).remove((*(storedTransmission + i)).indexOf(markers[m]), 1);
      }
    }
  }
}

/************************************************************************************************************************/
/************************/
/*      Test            */
/*    TO BE DELETED     */
/************************/
/************************************************************************************************************************/


/*
  @desc Reads input from Serial Monitor and transmit it through BlueTooth
  @param
  @return
*/
void readFromSerialToBT() {
  char c;
  if (Serial.available()) {
    c = Serial.read();
    Serial3.print(c);
    Serial.print(c);
  }
}

/*
  @desc Reads data receieved from BTLE and prints it to Serial Monitor
  @param
  @return
*/
void readFromBlueTooth() {
  char c;
  if (Serial3.available()) {
    c = Serial3.read();
    Serial.write(c);
  }
}
