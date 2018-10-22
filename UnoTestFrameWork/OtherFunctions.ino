
// Used to periodically check BlueTooth connection status
unsigned long timePrev = millis();
unsigned long timeCur;
unsigned long timeLapsed;
unsigned long period = 5000;  // Time (ms) between checks


void printBTStatus () {
  timeCur = millis();
  timeLapsed = timeCur - timePrev;

  if (timeLapsed >= period) {
    if (getConnectionStatus()) {
      Serial.println("Connected");
    } else {
      Serial.println("No Connection");
    }
    timePrev = millis();
  }
}

/*
  Generates a string of given length consisting only of alphanumeric characters
*/
String randomString(int len) {
  randomSeed(analogRead(0));
  String s = "";

  for (int i = 0; i < len; i++) {
    s.concat((char)random(32, 127));
  }
  return s;
}

/*
  Generates a string of given length consisting any character as defined by 
  ASCII chart reference at:
  https://www.arduino.cc/en/Reference/ASCIIchart 
  https://en.cppreference.com/w/cpp/language/ascii
*/
String randomStringOfAnyASCII(int len) {
  randomSeed(analogRead(0));
  String s = "";

  for (int i = 0; i < len; i++) {
    s.concat((char)random(0, 128));
  }
  return s;
}

int randomValue(int minValue, int maxValue) {
  randomSeed(analogRead(0));
  return (char)random(minValue, maxValue + 1);
}

