

# ENGG23600 Bluetooth System ReadMe - How To Send & Receive Data


### Transmitting Data	-------------------------
Function to call: `boolean sendIntArray(int data[])`

```
@description	Takes an array of integers to send to other device
@parameter	Array of integers. Must be of size 3 (number of elements)
@return 	boolean - whether the data was successfully sent
```


**Example:**
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




### Recieving Data	-------------------------
Function to call: `boolean receivedNewData()`

```
@desc	Checks if there is incoming transmission on the Bluetooth Serial. Writes to predetermined location (as assigned by backend and drive-base) if there is new data.
@param	NULL
@return	boolean - true if there is incomming tranmission
@return	boolean - false if there is no incomming tranmission
```

**Example:**
```
void loop() {
	if (receivedNewData()) {
		// do your team's logic here
		// read receieved data at designated location/variables
		// do stuff
	}
}
```


### Initialisation & Status	-------------------------

Function to call: `beginBluetooth()`

```
@desc Initialize the BlueTooth connections. Includes initial pairing
@param int baudRate (default is 9600)
@return
```

**Example:**
```
void setup() {
	//do your setup
	
	beginBluetooth;
	
	//do your setup
}
```

Function to call: `getConnectionStatus()`

```
@desc Return the paired status of the BlueTooth module.
@param
@return boolean - true if paired, false if not paired
```

**Example:**
```
void doSomething() {
	if (getConnectionStatus()) {		
		// do something
	}
}
```



Function to call: `boolean connectBluetooth()`

```
@desc Pairs the Bluetooth shields. Available to Uno only due to hardware limitation (slave and master relationship between BLE shields)
@param
@return boolean - true if pairing successfull
@return boolean - false if pairing unsuccessfull
```

**Example:**
```
void notBusy() {
	while (!getConnectionStatus) {
		connectBluetooth();
		
		// We are still testing if this is ever necessary.
		// Auto reconnect is in development
		// For now, assume that this is required if there is no connection
	}
}
```
