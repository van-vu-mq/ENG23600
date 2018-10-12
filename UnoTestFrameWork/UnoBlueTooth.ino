/*
  Source code for the Uno BlueTooth library.
  Functions outlined in the header file is implement here.
*/

#include <AltSoftSerial.h>
#include <CRC32.h>

#define connectionStatusPin 13

AltSoftSerial BTSerial;
String MegaMAC = "";


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
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);

  BTSerial.begin(baudRate);
  Serial.println("BTserial started at " + String(baudRate));

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
  if (!canDoAT()) {
    return false;
  }
  String successFlags[] = {"OK", "Set"};
  String response = "";

  BTSerial.print("AT+CON" + MegaMAC);

  response = atResponse();

  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (isATSucessfull(response, successFlags, numFlags)) {
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
  if (canDoAT()) {
    changeRole(0);
    changeName("UnoBluetooth");
    connectBluetooth();
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

  BTSerial.print("AT+NAME" + newName);
  response = atResponse();

  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (isATSucessfull(response, successFlags, numFlags)) {
    Serial.println("BLE name changed to " + newName);
  } else {
    Serial.println("Failed to change name");
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

  BTSerial.print("AT+ROLE" + String(role));
  response = atResponse();
  int numFlags = sizeof(successFlags) / sizeof(successFlags[0]);
  if (isATSucessfull(response, successFlags, numFlags)) {
    Serial.println("BLE role changed to " + r);
  } else {
    Serial.println(r);
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
  while (!BTSerial.available()) {
    if ((millis() - timeStart) > timeout) {
      Serial.println("AT Response Timeout");
      return "TIMEOUT";
    }
  }

  // Allow full response to load into buffer
  delay(150);

  // Read response
  while (BTSerial.available()) {
    char c = BTSerial.read();
    response.concat(c);
  }
  Serial.println("\n" + response);
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
    Serial.println("Error.\nBlueTooth is currently paired, unable to perform AT commands");
    return false;
  }


  /************************************************************************************************************************/
  /************************/
  /*      Transmit        */
  /************************/
  /************************************************************************************************************************/

  /*
    @desc Handles the tranmission process for an array of data (String)
    @param String data[] - array of message to be sent
    @return boolean - success  or failure to send or be recieved by paired device.
  */
  boolean sendData(String data[]) {
    // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

    // Add markers

    // Convert to single String

    // Encrypt string

    // Add checksum to string

    // Wrap whole string in marker : '<', '>'

    // Write to BTSerial

    // Listen for acknowlegde

    // if (acknowledge received) {
    //   return true;
    // } else {
    //   return false;
    // }

    return true;
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
    @desc Add markers to delimite end/beginning of file and lines
    @param String data
    @return String - data with added markers
  */
  void addMarker(String * dataArray, int arraySize) {
    // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

    // Add marker to every line in the array
    // markers: startData, endData, startLine, endLine
    //char markers = {'!', '@', '#', '$'};
  }

  /*
    @desc Add extra bits to the data to be used for error checking after transmission
    @param byte data - encrypted data in byte format
    @return byte - data with checksum bits
  */
  String addCheckSum(String data) {
    // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

    // calculate checksum

    // wrap checksum with marker: example '&', '*'

    // append checksum to data String

    //return data with the checksum included

    return "";

  }


  /************************************************************************************************************************/
  /************************/
  /*     Receive          */
  /************************/

  /************************************************************************************************************************/

  /*
    @desc Handles incoming data from the BTLE and the necessary processes before passing the data to the next destination.
    @param
    @return pointer to string of the data after processing
  */
  boolean receiveData() {
    // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

    // Check BTSerial if there is data
    // timeout if no data for a set amount of time
    //return false

    // Read and ignore until you find the packetStart marker
    // Read and store until you find the packetEnd marker
    // timeout if too much time has passed and you have no new data before packetEnd marker is found
    //return false

    // Remove packet Start/End marker

    // confirm checksum
    // if (checksum is wrong) {
    //   send error message to other bluetooth
    // } else {
    // send acknowledge to other bluetooth
    // remove checksum
    // decrypt
    // rebuild into an array
    // remove markers
    // depending on requirement from other team
    // we either return a pointer, a string, fill THEIR array with the data
    // return true

    // }

    char c;
    String data = "";
    if (BTSerial.available()) {
      c = BTSerial.read();
      data.concat(c);
    }

    return true;
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

    // Read the data and extract the part that is the checksum using the checksum markers
    // example markers start of checksum '&', end of checksum '*'

    // Caclculate the checksum on the data

    // Compare the extracted checksum and the calculated checksum

    // if (both checksum is the same) {
    //   return true
    // } else {
    //   return false
    // }

    return true;
  }

  /*
    @desc Removes the added markers for transmission from given data
    @param String data - data that contains markers
    @return String - data without markers
  */
  void removeMarker(String * dataArray, int arraySize, String markersToRemove[]) {
    // TODO /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

    // edit the array at the pointer

    // for (every marker in the markerList) {
    //   for (every line in the dataArray) {
    //     remove the marker
    // }

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
      BTSerial.print(c);
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
    if (BTSerial.available()) {
      c = BTSerial.read();
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
  void readArray(String dataSet[], int arraySize) {
    Serial.println("\nReceiving array test starting...");
    for (int i = 0; i < arraySize; i++) {
      String line = dataSet[i];
      if (!line.equals(NULL)) {
        Serial.println(String(i) + ": " + line);
      } else {
        Serial.println(String(i) + ": empty line in array");
      }
    }
    Serial.println("Receiving array test complete.");

  }

  /*
    @desc Reads and prints out data of an array from given pointer
    @param *data - pointer to the array
    @param arraySize
    @return
  */
  void readArrayFromPointer(String * data, int arraySize) {
    Serial.println("\nReceiving array pointer test starting...");
    for (int i = 0; i < arraySize; i++) {
      String line = *(data + i);
      if (!line.equals(NULL)) {
        Serial.println(String(i) + ": " + line);
      } else {
        Serial.println(String(i) + ": empty line in array");
      }
    }
    Serial.println("Receiving array pointer test complete.");
  }

  /*
    @desc Calculate checksum for static data.
    Example of how to use the CRC32 library
    @param
    @return
  */
  void sampleCheckSum() {
    // Checksum of "Hello World"
    // Use to compare the function output.
    const uint32_t KNOWN_CHECKSUM = 0x4A17B156;

    // Data to calculate checksum
    uint8_t byteBuffer[] = "Hello World";
    size_t numBytes = sizeof(byteBuffer) - 1;

    // Checksum library instance.
    CRC32 crc;

    // store calculated checksum
    uint32_t checksum;

    // calculate checksum for the entire buffer at once
    checksum = crc.calculate(byteBuffer, numBytes);

    // print out to compare
    Serial.println("Known checksum:   " + String(KNOWN_CHECKSUM));
    Serial.println("Generated checksum: " + String(checksum));
    Serial.println("\nCheckSums as HEX");
    Serial.println(KNOWN_CHECKSUM, HEX);
    Serial.println(checksum, HEX);

  }












