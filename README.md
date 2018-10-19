

#ENGG23600 Bluetooth System ReadMe

##How To



###Transmitting Data
```
boolean sendIntArray(int data[])
```

```
@description	Takes an array of integers to send to other device
@parameter		Array of integers. Must be of size 3 (number of elements)
@return 		boolean - whether the data was successfully sent
```


**Example**
```
void doSomething() {
	int canRGBOrder[3] = {2, 3, 1};
	
	boolean sentOrder = sendIntArray(canRGBOrder);
	
	if (sentOrder) {
		// do stuff
		// Update front-end / user etc
	}
}
```




/***************************************************/
/****											****/
/****	  		Recieving Data					****/
/****											****/
/***************************************************/

/*
  @desc Checks if there is incoming transmission on the Bluetooth Serial. Writes to predetermined location (as assigned by backend and drive-base) if there is new data.
  @param
  @return boolean - true if there is incomming tranmission
  @return boolean - false if there is no incomming tranmission
*/
boolean receivedNewData() 

// Example
void loop() {
	if (receivedNewData()) {
		// do your team's logic here
		// read receieved data at designated location/variables
		// do stuff
	}
}




/***************************************************/
/****											****/
/****	  		Initialisation & Status			****/
/****											****/
/***************************************************/

/*
  @desc Initialize the BlueTooth connections. Includes initial pairing
  @param int baudRate (default is 9600)
  @return
*/
void beginBluetooth(int baudRate)

// Example
void setup() {
	//do your setup
	
	beginBluetooth();
	
	//do your setup
}


/*
  @desc Return the paired status of the BlueTooth module.
  @param
  @return boolean - true if paired, false if not paired
*/
boolean getConnectionStatus() 

// Example
void doSomething() {
	if (getConnectionStatus()) {		
		// do something
	}
}



/*
  @desc Pairs the Bluetooth shields. Available to Uno only due to hardware limitation (slave and master relationship between BLE shields)
  @param
  @return boolean - true if pairing successfull
  @return boolean - false if pairing unsuccessfull
*/
boolean connectBluetooth()

// Example
void notBusy() {
	while (!getConnectionStatus) {
		connectBluetooth();
		
		// We are still testing if this is ever necessary.
		// Auto reconnect is in development
		// For now, assume that this is required if there is no connection
	}
}



