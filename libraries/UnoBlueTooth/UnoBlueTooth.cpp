/*
	Source code for the Uno BlueTooth library.
	Functions outlined in the header file is implement here.
*/

#include <AltSoftSerial.h>
#include <UnoBlueTooth.h>

AltSoftSerial BTSerial;
int connectionStatusPin;
String MegaMAC;


/************************/
/*		Initialize		*/
/************************/

UnoBlueTooth::UnoBlueTooth() {
	connectionStatusPin = 13;
	MegaMAC = "04B167086527";  // tomy's phone
}

/*
  @desc Initialize the BlueTooth connections
  @param int baudRate
  @return
*/
void UnoBlueTooth::begin(int baudRate) {
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
boolean UnoBlueTooth::getConnectionStatus(){
	/*
		Polls the state pin several times and checks whether it is BLINKING or HIGH.
		HM-10 BLE module BLINKs every 500ms when not paired. Polling needs to have sufficient fidelity to account for this timing.
	*/
	
  int sPin = -1;  // BLE paired status
  int stateCount = 15;  // number of times to poll state pin
  unsigned long stateCheckPeriod = 100; // time (ms) between polls

  unsigned long timePrev = millis();
  unsigned long timeCur;
  unsigned long timeLapsed;

  int pollCounter = 0;
  while (pollCounter < stateCount) {
    timeCur = millis();
    timeLapsed = timeCur - timePrev;
    if (timeLapsed > stateCheckPeriod) {
      sPin = digitalRead(connectionStatusPin);
      if (sPin == 0) {
        return false;
      }
      timePrev = millis();
      pollCounter++;
    }
  }
  return true;	
}

/*
	@desc Pairs the BTLE with the device correseponding to the stored MAC address.
	@param
	@return
*/
void UnoBlueTooth::connect() {
	
}

/*
	@desc Changes BlueTooth module to become the master
	@param
	@return
*/
void UnoBlueTooth::doATCommandSetup() {
	
}


/************************/
/*		Transmit		*/
/************************/

/*
	@desc Handles the transmission process for a single given string
	@param String data - message to be sent
	@return boolean - success	 or failure to send or be recieved by paired device.
*/
boolean UnoBlueTooth::sendData(String data) {
	
}

/*
	@desc Handles the tranmission process for an array of data (String)
	@param String data[] - array of message to be sent
	@return boolean - success	 or failure to send or be recieved by paired device.
*/
boolean UnoBlueTooth::sendArray(String data[]) {
	
}

/*
	@desc Encrypts a given string.
	@param String data
	@return String - the data in encrypted form
*/
String UnoBlueTooth::encrypt(String data) {
	
}

/*
	@desc Add markers to delimite end/beginning of file and lines
	@param String data
	@return String - data with added markers
*/
String UnoBlueTooth::addMarker(String data) {
	
}

/*
	@desc Add extra bits to the data to be used for error checking after transmission
	@param byte data - encrypted data in byte format
	@return byte - data with checksum bits
*/
byte UnoBlueTooth::addCheckSum(byte data) {
	
}

/************************/
/*		Receive			*/
/************************/

/*
	@desc Handles incoming data from the BTLE and the necessary processes before passing the data to the next destination.
	@param	
	@return String - the data after processing
*/
String UnoBlueTooth::receiveData() {
	
}

/*
	@desc Undoes the encryption on given String
	@param String data - encrypted datas
	@return String - unencrypted data
*/
String UnoBlueTooth::decrypt(String data){
	
}

/*
	@desc Reads the checksum of given data
	@param byte data
	@return boolean - TRUE if data and checksum matches
	@return boolean - FALSE if data and checksum does not match
*/
boolean UnoBlueTooth::confirmCheckSum(byte data) {
	
}

/*
	@desc Removes the added markers for transmission from given data
	@param String data - data that contains markers
	@return String - data without markers
*/
String UnoBlueTooth::removeMarker(String data) {
	
}


/************************/
/*		Test			*/
/************************/

/*
	@desc Reads input from Serial Monitor and transmit it through BlueTooth
	@param
	@return 
*/
void UnoBlueTooth::readFromSerial() {
	
}

/*
	@desc Reads data receieved from BTLE and prints it to Serial Monitor
	@param
	@return 
*/
void UnoBlueTooth::readFromBlueTooth() {
	
}

/*
	@desc Transmit given data using BlueTooth
	@param
	@return 
*/
void UnoBlueTooth::sendToBlueTooth() {
	
}

/*
	@desc Reads a static array and prints it to Serial Monitor.
	This function is for testing purposes
	@param String dataSet[]
	@param arraySize - the array size to limit memory reading in case where a pointer is passed instead of an array
	@return 
*/
void UnoBlueTooth::readArray(String dataSet[], int arraySize) {
  for (int i = 0; i < arraySize; i++) {
    String line = dataSet[i];
    if (!line.equals(NULL)) {
      Serial.println(i + ": " + String(dataSet[i]));
    } else {
      Serial.println(i+": empty line in array");
    }
  }
  Serial.println("Receiving array test complete.");

}



