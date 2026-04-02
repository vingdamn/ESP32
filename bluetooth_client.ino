#include "BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"

BluetoothSerial SerialBT;

String targetName = "ESP32_BT_Server"; // CHANGE THIS
uint8_t targetAddress[6];
bool deviceFound = false;

void btScanCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
  if (event == ESP_BT_GAP_DISC_RES_EVT) {

    for (int i = 0; i < param->disc_res.num_prop; i++) {
      esp_bt_gap_dev_prop_t *p = &param->disc_res.prop[i];

      if (p->type == ESP_BT_GAP_DEV_PROP_EIR) {
        uint8_t *eir = (uint8_t *)(p->val);
        uint8_t len;
        uint8_t *name;

        name = esp_bt_gap_resolve_eir_data(
          eir,
          ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME,
          &len
        );

        if (name) {
          String devName = String((char*)name).substring(0, len);

          Serial.print("Found: ");
          Serial.println(devName);

          if (devName == targetName) {
            Serial.println("Target found!");

            memcpy(targetAddress, param->disc_res.bda, 6);
            deviceFound = true;

            esp_bt_gap_cancel_discovery();
            return;
          }
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  if (!SerialBT.begin("ESP32_Client", true)) {
    Serial.println("BT init failed");
    return;
  }

  esp_bt_gap_register_callback(btScanCallback);

  Serial.println("Scanning...");
  esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void loop() {
  if (deviceFound) {
    Serial.println("Connecting...");

    if (SerialBT.connect(targetAddress)) {
      Serial.println("Connected!");

      SerialBT.println("Hello from ESP32");

      while (SerialBT.connected()) {
        if (SerialBT.available()) {
          Serial.write(SerialBT.read());
        }
      }

      Serial.println("Disconnected");
    } else {
      Serial.println("Connection failed");
    }

    deviceFound = false;
  }

  delay(1000);
}