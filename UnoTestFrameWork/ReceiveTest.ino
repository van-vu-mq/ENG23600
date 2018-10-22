int receiveCount = 0;

/*
  Write to serial monitor over COM port. Using processing program, read the data from serial
  and write to file.
*/
void writeRecievedToFile() {
  // called after receivedNewData() 
  // global variables should have been changed if above funciton returned true
  Serial.println(String(receiveCount) + ": " + String(redCansError) + ", " + String(greenCansError) + ", " + String(blueCansError));
}

