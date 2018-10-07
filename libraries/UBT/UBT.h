/*
   Header file for UNO BlueTooth library

   Identifies functions that are used.
*/


// Handles library being included twice.
#ifndef UBT_h
#define UBT_h


#include <AltSoftSerial.h>

class UBT {
  public:
	UBT();
	void sendStaticData();
	void begin(int baudRate);
	void sendFromSerial();
	void receiveBTSerial();
  private:
	AltSoftSerial BTSerial;
};


#endif
