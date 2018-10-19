

/*
  Write to serial monitor over COM port. Using processing program, read the data from serial 
  and write to file.
*/
void writeRevievedToFile() {
  Serial.println(String(redCansError) + ", " + String(greenCansError) + ", " + String(blueCansError));
}

