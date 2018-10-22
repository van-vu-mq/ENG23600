

void testAllOrders() {
  int maxCan = 10;
  int maxOneColour = 8;
  int counter = 0;
  int testCount = 500;
  
  // clear serial
  while (Serial.available()) {
    Serial.read();
  }

  for (int r = 0; r <= maxOneColour; r++) {
    for (int g = 0; g <= maxOneColour; g++) {
      for (int b = 0; b <= maxOneColour; b++) {

        if (r + g + b <= maxCan && counter < testCount) {
          // print to serial so that it can be logged to text file
          Serial.println("<" + String(counter) + ": " + String(r) + ", " + String(g) + ", " + String(b) + ">");

          // test data passed to bluetooth
          int testData[3] = {r, g, b};
          sendIntArray(testData);
          counter++;
        }
      }
    }
  }
}

