/*
Header file for Uno BlueTooth library.
Outlines the functions implemented in the library.
*/

// Handles cases where this library is included more than once.
#ifndef UnoBlueTooth_h
#define UnoBlueTooth_h

class UnoBlueTooth {
	// Functions that are accessible by other programs.
	public:
		UnoBlueTooth();
		
		void begin(int baudRate);
		boolean getConnectionStatus();	
		void connect();
		
		boolean sendData(String data);
		boolean sendArray(String data[]);
		String recieveData();		
		
		// Testing functions. These are used to test concepts.
		// Delete these afterwards to conserve system memory.
		void readFromSerial();
		void readFromBlueTooth();
		void sendToBlueTooth();
		void readArray(String dataSet[], int arraySize);
	
	// Functions that can only be accessed internally by the Uno BlueTooth library.
	private:
		AltSoftSerial BTSerial;
		int connectionStatusPin;
		
		String encrypt(String data);
		String decrypt(String data);
		
		String addMarker(String data);
		String removeMarker(String data);
		
		byte addCheckSum(byte data);
		boolean confirmCheckSum(byte data);
		
		void doATCommandSetup();
		
}