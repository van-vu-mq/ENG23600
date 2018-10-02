#include <UBT.h>

UBT conn;

void setup() {
  // put your setup code here, to run once:
  conn.begin(9600);
  //conn.sendStaticData();
}

void loop() {
  // put your main code here, to run repeatedly:
  conn.receiveBTSerial();
  conn.sendFromSerial();
}
