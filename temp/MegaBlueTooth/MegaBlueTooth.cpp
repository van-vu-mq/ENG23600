
/*
  Source code for the Uno BlueTooth library.
  Functions outlined in the header file is implement here.
*/

#include <MegaBlueTooth.h>
#include <CRC32.h>

//Reads from Serial1
//Sends to Serial

String UnoMAC;


/************************/
/*    Initialize    	*/
/************************/

MegaBlueTooth::MegaBlueTooth() {
  UnoMAC = "04B167086527";  // tomy's phone
}

/*
  @desc Initialize the BlueTooth connections
  @param int baudRate
  @return
*/
void UnoBlueTooth::begin(int baudRate) {
  //pinMode(connectionStatusPin, INPUT);
  Serial.begin(baudRate);
  while (!Serial);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  
  Serial1.begin(baudRate);
  
  Serial.println("Serial1 started at " + String(baudRate));

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
  @desc Pairs the BTLE with the device correseponding to the stored MAC address.
  @param
  @return
*/
void MegaBlueTooth::connect() {
	// TODO
	// Timeout
	// merge reading response with other AT functions
	
  String res1 = "OK";
  String res2 = "Set";
  String response = "";

  BTSerial.print("AT+CON" + MegaMAC);

  response = atResponse();
}

/*
  @desc executes a predefined set of AT commands
  @param
  @return
*/
void MegaBlueTooth::doATCommandSetup() {
  if (canDoAT()) {
    changeRole(1);
    changeName("MegaCommsF");
  }
}

/*
  @desc Change the name of the BLE 
  @param String name
  @return
*/
void MegaBlueTooth::changeName(String name) {
  int flagCount = 3;
  String successFlags[flagCount] = {"OK", "Set", name};
  String response = "";
  BTSerial.print("AT+NAME" + name);

  response = atResponse();
  if (isATSucessfull(response, &successFlags[0], flagCount)) {
    Serial.println("BLE name changed to " + name);
  }
}

/*
  @desc Changes the role of the BLE
  @param int role. 0=slave, 1=master
  @return
*/
void MegaBlueTooth::changeRole(int role) {
  int flagCount = 3;
  String successFlags[flagCount] = {"OK", "Set", String(role)};
  String response = "";
  String r = "error";
  
  if (role == 0) 		{ r = "slave" ;} 
  else if (role == 1) 	{ r = "master";}
  
  BTSerial.print("AT+ROLE" + String(role));
  response = atResponse();

  if (isATSucessfull(response, &successFlags[0], flagCount)) {
    Serial.println("BLE role changed to " + r);
  }
}

/*
  @desc Check that the given response string contains all the given success
  @param String response - reponse given by AT commands
  @param String successFlags[]
  @return boolean - whether all flags are found in the response
*/
boolean MegaBlueTooth::isATSucessfull(String response, String *successFlags, int numFlags) {
	for (int index=0; index<numFlags; index++) {
		if (response.indexOf(*(successFlags+index)) == -1) {
			return false;
		}
	}
	return true;
}

/*
  @desc Listens on the AltSoftSerial ports for a response. Reads response into a string.
  @param
  @return String - response
*/
String MegaBlueTooth::atResponse() {
	
  if (!canDoAT()) { return "ERROR"; }
  
  String response = "";
  unsigned long timeout = 2000;
  unsigned long timeStart = millis();
  
  while (!BTSerial.available()) {
	  if ((millis() - timeStart) > timeout) {
		  Serial.println("AT Response Timeout");
		  return "TIMEOUT";
	  }
  }
  delay(150);
  while (BTSerial.available()) {
    char c = BTSerial.read();
    response.concat(c);	
  }
  Serial.println("\n"+response);
  return response;
}

/*
  @desc 
  @param
  @return
*/
boolean MegaBlueTooth::canDoAT() {
	if (getConnectionStatus() == 0) {
		return true;
	} else {
		Serial.println("Error.\nBlueTooth is currently paired, unable to perform AT commands");
		return false;
	}
}


/************************/
/*    	Transmit    	*/
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
/*   	 Receive    	*/
/************************/

/*
  @desc Handles incoming data from the BTLE and the necessary processes before passing the data to the next destination.
  @param
  @return String - the data after processing
*/
String MegaBlueTooth::receiveData() {
	char c;
	String data = "";
  	if (Serial1.available()) {
    		c = Serial1.read();
    		data.concat(c);
	}
	return data;
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
	String t = "";
	String data = "";
	data = data.receiveData();
	data = decrypt(data);
	int i=0;
	if(confirmCheckSum(data)){
		while(i<data.length()){
			if(data.charAt(i)!='<' && data.charAt(i)!='>' && data.charAt(i)!='%'){
				t.concat(data.charAt(i));
			}
			i++;
		}
	}
	return t;
}


/************************/
/*    	Test      		*/
/* 		TO BE DELETED 	*/
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
    c = Serial.read();
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
void MegaBlueTooth::readArrayFromPointer(String *data, int arraySize) {
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
void MegaBlueTooth::sampleCheckSum() {
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
	Serial.println("Known checksum: 	" + String(KNOWN_CHECKSUM));
	Serial.println("Generated checksum:	" + String(checksum));
	Serial.println("\nCheckSums as HEX");
	Serial.println(KNOWN_CHECKSUM, HEX);
	Serial.println(checksum, HEX);
	
}











