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
	@desc 
	@param
	@return 
*/
void UnoBlueTooth::readFromSerial() {
	
}

/*
	@desc 
	@param
	@return 
*/
void UnoBlueTooth::readFromBlueTooth() {
	
}

/*
	@desc 
	@param
	@return 
*/
void UnoBlueTooth::sendToBlueTooth() {
	
}

/*
	@desc 
	@param
	@return 
*/
void UnoBlueTooth::processArray(String dataSet[], int arraySize) {
	
}


