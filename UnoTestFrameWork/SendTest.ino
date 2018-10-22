int maxCan = 10;
int maxOneColour = 8;

void testAllOrders() {
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



void sendCorruptData() {
  int numOfFaults = randomValue(0, 5); // how many times to insert simulated corruption into packet
  int maxSizeOfFault = 5; // how many bytes the inserted corruption simulation should be
  int testSampleSize = 100;

  int arraySize = 4;
  String sampleData[arraySize] = {"INT", "",  "", ""};

  int sentCounter = 0;

  while (sentCounter < testSampleSize) {
    // generate random test data
    int r = maxOneColour;
    int g = maxOneColour;
    int b = maxOneColour;

    while ( r + g + b > maxCan) {
      r = randomValue(0, maxOneColour);
      g = randomValue(0, maxOneColour);
      b = randomValue(0, maxOneColour);
    }
    sampleData[1] = r;
    sampleData[2] = g;
    sampleData[3] = b;



    // Build packet for transmission as usual
    // Assume that the function on the same board always works

    addMarker(&sampleData[0], arraySize);

    // Convert to single String
    String packet = transformToString(sampleData, arraySize);

    // Prepend checksum
    packet = addCheckSum(packet);

    // add packet markers
    packet = "<" + packet + ">";


    // add corruption
    // simulates interference, bad pin connections, poor signal etc
    for (int i = 0; i < numOfFaults; i++) {
      int lengthOfFault = randomValue(0, maxSizeOfFault);
      String randomData = randomString(lengthOfFault);
      int posOfFault = randomValue(0, packet.length());

      // insert random data
      // left of pos + random data + right of pos
      packet = packet.substring(0, posOfFault) + randomData + packet.substring(posOfFault);
    }

    // print to serial for logging
    Serial.println(String(sentCounter) + ": " + packet);

    // transmit the corrupt data via bluetooth
    transmitData(packet);

    sentCounter++;

  }



}

