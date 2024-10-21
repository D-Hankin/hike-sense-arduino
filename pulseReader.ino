#include <ArduinoBLE.h>

// BLE service and characteristic UUIDs
BLEService myService("abcdefab-1234-5678-1234-56789abcdef0");   
BLEIntCharacteristic myCharacteristic("abcdefab-4321-5678-1234-56789abcdef0", BLERead | BLENotify); 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1); // Loop forever if BLE initialization fails
  }

  // Set the local name for the device
  BLE.setLocalName("HikeSenseBLE");

  // Add the service and characteristic
  BLE.setAdvertisedService(myService);
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Start advertising
  BLE.advertise();
  
  Serial.println("BLE device is now advertising");
}

void loop() {
  // Check for a central device connection
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // While the central is connected
    while (central.connected()) {
      // Read the pulse data from the analog pin
      int pulse = analogRead(A0);
      Serial.print("Pulse reading: ");
      Serial.println(pulse); // Print the pulse reading to the Serial Monitor

      // Set the value of the pulse characteristic
      myCharacteristic.setValue(pulse); // Send the pulse as an integer

      delay(1000); // Delay before the next reading
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
