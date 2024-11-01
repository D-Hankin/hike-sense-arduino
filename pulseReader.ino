#include <ArduinoBLE.h>
#include <PulseSensorPlayground.h>
#define USE_ARDUINO_INTERRUPTS true

 
// Constants
const int PULSE_SENSOR_PIN = 0;  // Analog PIN where the PulseSensor is connected
const int LED_PIN = 13;          // On-board LED PIN
const int THRESHOLD = 550;  
// Custom BLE service and characteristic UUIDs
BLEService myService("abcdefab-1234-5678-1234-56789abcdef0");   // Replace with your own service UUID
BLEIntCharacteristic bpmCharacteristic("abcdefab-4321-5678-1234-56789abcdef0", BLERead | BLENotify); // Replace with your own characteristic UUID
PulseSensorPlayground pulseSensor;

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
  myService.addCharacteristic(bpmCharacteristic);
  BLE.addService(myService);

  // Start advertising
  BLE.advertise();
  
  Serial.println("BLE device is now advertising");

  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);
 
  // Check if PulseSensor is initialized
  if (pulseSensor.begin()) 
  {
    Serial.println("PulseSensor object created successfully!");
  }
}

void loop() {
  // Check for a central device connection
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // While the central is connected
    while (central.connected()) {
      // Check if a heartbeat was detected
      if (pulseSensor.sawStartOfBeat()) {
        int currentBPM = pulseSensor.getBeatsPerMinute();
        
        Serial.println("♥ A HeartBeat Happened!");
        Serial.print("BPM: ");
        Serial.println(currentBPM);

        // Update the characteristic with the current BPM
        bpmCharacteristic.setValue(currentBPM);
        Serial.print("BPM sent: ");
        Serial.println(currentBPM);
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
