

void testAllOrders() {
  int maxCan = 10;
  int maxOneColour = 8;
  int counter = 0;
  int testCount = 500;

  for (int r = 0; r <= maxOneColour; r++) {
    for (int g = 0; g <= maxOneColour; g++) {
      for (int b = 0; b <= maxOneColour; b++) {

        if (r + g + b <= maxCan && counter < testCount) {
          int testData[3] = {r, g, b};
          Serial.println("<" + String(counter) + " " + String(r) + ", " + String(g) + ", " + String(b) + ">12312321");
          sendIntArray(testData);
          counter++;
          delay(500);
        }
      }
    }
  }
}

