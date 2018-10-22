/*
  Source code for the Uno BlueTooth library.
  Functions outlined in the header file is implement here.
*/

#define connectionStatusPin 13

#define packetStartMarker   '<'
#define packetEndMarker     '>'

#define dataStartMarker     '!'
#define dataEndMarker       '@'

#define lineStartMarker     '#'
#define lineEndMarker       '$'

#define checksumStartMarker '&'
#define checksumEndMarker   '*'

String MegaMAC = "";

String *storedTransmission;
int storedSize = 0;

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
  pinMode(connectionStatusPin, INPUT);
  Serial.begin(baudRate);
  while (!Serial);
  doATCommandSetup();
}

/*
  @desc Return the paired status of the BlueTooth module.
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
  String successFlags[] = {"OK", "Set"};
  Serial.print("AT+CON" + MegaMAC);
  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (isATSucessfull(atResponse(), successFlags, numFlags)) {
    return true;
  } else {
    return false;
  }
}

/*
  @desc executes a predefined set of AT commands
  @param
  @return
*/
void doATCommandSetup() {
  changeRole(1);
  connectBluetooth();
}

/*
  @desc Changes the name of the Bluetooth module to the string given.
  Max name length is 12 characters
  @param String name
  @return
*/
void changeName(String newName) {
  String successFlags[] = {"OK", "Set", newName};
  Serial.print("AT+NAME" + newName);
}

/*
  @desc Changes the role of the Bluetooth module
  @param int role. 0=slave, 1=master
  @return
*/
void changeRole(int role) {
  String successFlags[] = {"OK", "Set", String(role)};
  Serial.print("AT+ROLE" + String(role));
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
  String response = "";
  if (getConnectionStatus()) {
    return response;
  }
  unsigned long timeout = 2000;
  unsigned long timeStart = millis();

  // Check if there is a response within timeout period
  while (!Serial.available()) {
    if ((millis() - timeStart) > timeout) {
      return response;
    }
  }

  // Allow full response to load into buffer
  delay(150);

  // Read response
  while (Serial.available()) {
    char c = Serial.read();
    response.concat(c);
  }
  return response;
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
boolean sendData(String copyData[], int arraySize) {
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

  // clone array so we can edit the data in memory
  //String copyData[arraySize];
  //memcpy(copyData, data, sizeof(data[0])*arraySize);

  // Add markers
  addMarker(&copyData[0], arraySize);

  // Convert to single String
  String packet = transformToString(copyData, arraySize);

  // Encrypt data
  packet = encrypt(packet);

  // Prepend checksum
  packet = addCheckSum(packet);

  // add packet markers
  packet = packetStartMarker + packet + packetEndMarker;

  // Write to Serial
  int transmitAttempts = 5;
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

  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
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

void transmitData(String data) {
  // NOTE: BLE 4.0 standards - can only transmit 20 bytes per packet
  // Send packet in groups of 20 bytes (equivalent of 20x char)
  int packetByteLimit = 20;
  int packetParts = data.length() /  packetByteLimit; // number of max size packets in given data

  //*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
  // NOTE: may need to delay time between each transmission
  // Transmit max sized packets using substring(start, end)
  for (int i = 0; i < packetParts; i++) {
    int packetStart = i * packetByteLimit;
    int packetEnd = packetStart + packetByteLimit;
    Serial.print(data.substring(packetStart, packetEnd));
  }

  // Transmit tail end - non maxed sized packet using substring(start)
  Serial.print(data.substring(packetParts * packetByteLimit));
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
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
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
  // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

  // failed to get communication from Bluetooth
  String dataFromBT = readFromBTBuffer();
  if (dataFromBT.equals(""))        {
    return false;
  }

  // remove packet markers
  dataFromBT.remove(dataFromBT.indexOf(packetStartMarker), 1);
  dataFromBT.remove(dataFromBT.indexOf(packetEndMarker), 1);


  // check data integrity
  if (!confirmCheckSum(dataFromBT)) {
    return false;
  }

  // send acknowledge
  sendAcknowledge();

  // remove checksum from the packet
  dataFromBT = removeCheckSum(dataFromBT);

  // decrypt
  dataFromBT = decrypt(dataFromBT);

  // rebuild the data into array
  rebuildData(dataFromBT);

  // scrub the array of any added data used in tranmission
  removeMarkers();

  // write data to designated location
  writeToVariables();
  
  return true;
}


/*
  @desc
  @param
  @return
*/
void sendAcknowledge() {
  Serial.print("<ACK>");
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
    redCansError    = (*(storedTransmission + 1)).toInt();
    greenCansError  = (*(storedTransmission + 2)).toInt();
    blueCansError   = (*(storedTransmission + 3)).toInt();
  }
}

/*
  @desc
  @param
  @return
*/
String readFromBTBuffer() {
  String packet = "";
  
  // No communication on Bluetooth Serial
  if (Serial.available() <= 0) {
    return packet;
  }

  char fromBT = Serial.read();
  int timeout = 1000;
  unsigned long timePrev = millis();

  // read until start of packet marker is found
  while (fromBT != packetStartMarker) {
    if (millis() - timePrev >= timeout) {
      return "";
    }
    if (Serial.available()) {
      fromBT = Serial.read();
      timePrev = millis();
    }

  }
  // start of packet found
  // store
  packet.concat(fromBT);

  // read until end of packet marker is found
  while (fromBT != packetEndMarker) {
    if (millis() - timePrev >= timeout) {
      return "";
    }
    if (Serial.available()) {
      fromBT = Serial.read();
      packet.concat(fromBT);
      timePrev = millis();
    }
  }
  return packet;
}

/*
  @desc
  @param
  @return
*/
void rebuildData(String dataFromBT) {
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
  // for every line in the array
  for (int i = 0; i < storedSize; i++) {
    (*(storedTransmission + i)).remove( (*(storedTransmission + i)).indexOf(lineStartMarker), 1);
    (*(storedTransmission + i)).remove( (*(storedTransmission + i)).indexOf(lineEndMarker), 1);
  }
}
