/*
   Header file for UNO BlueTooth library

   Identifies functions that are used.
*/


// Handles library being included twice.
#ifndef UNObt_h
#define UNObt_h


#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

class UNObt {
  public:
    UNObt();  // constructor
    void begin(int baudRate); // initialise
    void sendFromSerial();  // transmit data entered into serial monitor
    void receiveBTSerial(); // displays data received from BTSerial
    boolean transmitArray(String *data);  // send array of
    boolean transmit(String data);  // send single string
    int getStatus();

    // Test functions. Delete after to conseve memory
    void getOtherAddr();  // print to serial monitor stored MAC address
    void sendStaticData();  // transmit static test data
    void arrayTest(String dataSet[], int size);

  private:
    AltSoftSerial BTSerial;
    int statusPin;
    void encrypt(String *data); // encrypt data at given pointer
    void decrypt(String *data);
    void addMarker(); // add markers denoting end/beginning of file and line
    boolean addCheckSUM();  // add error checking
    boolean checkError(); // confirm data is correct
    void connect();   // BT pairing with designated device / MAC address


};


#endif
