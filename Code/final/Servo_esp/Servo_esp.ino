/*
 * BLE Servo Toggle for Seeed Studio XIAO ESP32C6
 * Control servo over Bluetooth Low Energy using any BLE app (iOS or Android)
 */

#include <Arduino.h>
#include <ESP32Servo.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

Servo myServo;
const int servoPin = 18;
bool isAtZero = true;

// BLE UUIDs
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-ab12-cd34-ef56-1234567890ab"

BLECharacteristic *pCharacteristic;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();

    if (value.length() > 0) {
      char command = value.charAt(0);

      if (command == 'A' || command == 'a') {
        if (isAtZero) {
          myServo.write(180);
          Serial.println("Moved to 180°");
        } else {
          myServo.write(0);
          Serial.println("Moved to 0°");
        }
        isAtZero = !isAtZero;
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(0); // Start at 0 degrees

  BLEDevice::init("ESP32-BLE-Servo");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Ready");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("BLE Servo ready. Send 'A' to toggle.");
}

void loop() {
  // Nothing here; BLE handles it
}
