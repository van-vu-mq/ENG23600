int redCansError;
int greenCansError;
int blueCansError;

boolean updateFromRobot = false;

void setup() {
  // Initialise with system baudrate. Default is 9600
  beginBluetooth(9600); 

  // Get order from front-end
  int sampleOrder[3] = {1, 2, 3};

  // Send order to drive base
  boolean successfullySentOrder = sendIntArray(sampleOrder);

  if (!successfullySentOrder) {
    // Notify user
    // Handle failure to send order

    
    // e.g.
    if (!getConnectionStatus()) { // no bluetooth connection
      // return "Unable to connect with the robot"
    } else {  // there is a bluetooth connection, but communication isnt working
      // return "Error sending order"
    }
  }

  testAllOrders();
  
}

void loop() {
  // check if there is communication from the drive base
  if (receivedNewData()) {
    // rec/green/blueCansError has been updated using data received from the Arduino Mega (drive-base)
    // do whatever you need
  }

  // alternate
  updateFromRobot = receivedNewData();
  // now take action based on variable
}

