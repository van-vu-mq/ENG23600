/*
  Header file for Uno BlueTooth library.
  Outlines the functions implemented in the library.
*/

// Handles cases where this library is included more than once.
#ifndef UnoBlueTooth_h
#define UnoBlueTooth_h

#include <AltSoftSerial.h>

class UnoBlueTooth {
    // Functions that are accessible by other programs.
  public:
    // initialize
    UnoBlueTooth();
    void begin(int baudRate);
    void connect();

    // communications
    boolean sendData(String data);
    boolean sendArray(String data[]);
    String receiveData();

    // settings and status
    void changeName(String name);
    void changeRole(int role);
    int getConnectionStatus();

    // Testing functions. These are used to test concepts.
    // Delete these afterwards to conserve system memory.
    void readFromSerialToBT();
    void readFromBlueTooth();
    void readArray(String dataSet[], int arraySize);
	void readArrayFromPointer(String *data, int size);
	void sampleCheckSum();
	

    // Functions that can only be accessed internally by the Uno BlueTooth library.
  private:
    // globals
    AltSoftSerial BTSerial;
    int connectionStatusPin;
    String MegaMAC;

    // security
    String encrypt(String data);
    String decrypt(String data);

    // transmit utility for communication
    String addMarker(String data);
    String removeMarker(String data);

    // error detection / correction
    String addCheckSum(String data);
    boolean confirmCheckSum(String data);

    // uncategorised
    void doATCommandSetup();
    String atResponse();
	boolean canDoAT();
	boolean isATSucessfull(String response, String *sucessFlags, int numFlags);
};

#endif