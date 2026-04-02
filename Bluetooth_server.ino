#include "BluetoothSerial.h"

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Create BluetoothSerial instance
BluetoothSerial SerialBT;

// Bluetooth device name
const char* bt_device_name = "ESP32_BT_Server";

void setup() {
  Serial.begin(115200);
  
  // Start Bluetooth serial server
  SerialBT.begin(bt_device_name);
  
  Serial.println("Bluetooth Server Started");
  Serial.printf("Device name: %s\n", bt_device_name);
  Serial.println("Waiting for a client to connect...");
}

void loop() {
  // Check if a client is connected
  if (SerialBT.hasClient()) {
    // Check if data is available from the client
    if (SerialBT.available()) {
      // Read incoming data
      String received_data = SerialBT.readString();
      
      // Print received data to Serial Monitor
      Serial.print("Received: ");
      Serial.println(received_data);
      
      // Echo the data back to the client
      SerialBT.println("Echo: " + received_data);
    }
  } else {
    // Optional: Show that we're waiting for a connection
    static unsigned long last_print = 0;
    if (millis() - last_print > 5000) {
      last_print = millis();
      Serial.println("Waiting for Bluetooth connection...");
    }
  }
  
  // Optional: Send data from Serial Monitor to Bluetooth client
  if (Serial.available()) {
    String serial_data = Serial.readString();
    SerialBT.println(serial_data);
  }
  
  delay(10);
}