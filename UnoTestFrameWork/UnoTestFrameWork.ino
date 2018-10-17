int redCansError;
int greenCansError;
int blueCansError;

void setup() {
  beginBluetooth(9600); // Initialise
}

void loop() {
  printBTStatus();
  //readFromSerialToBT();
  //readFromBlueTooth();
}

