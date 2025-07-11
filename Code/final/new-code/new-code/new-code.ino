#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

int servoPin = 18;
BLECharacteristic* pCharacteristic;
bool newAngleAvailable = false;
int targetAngle = 90;  // default starting angle

// Your original manual PWM function
void myServo_write(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2500);  // microseconds

  // Send 25 pulses at 50Hz (~500 ms)
  for (int i = 0; i < 25; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(servoPin, LOW);
    delayMicroseconds(20000 - pulseWidth);
  }
}

// BLE callback handler
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    String value = String((char*)pCharacteristic->getData());
    value.trim();  // remove whitespace
    Serial.print("Received via BLE: ");
    Serial.println(value);

    int receivedAngle = value.toInt();
    if (receivedAngle >= 0 && receivedAngle <= 180) {
      targetAngle = receivedAngle;
      newAngleAvailable = true;
    } else {
      Serial.println("Invalid angle. Must be between 0 and 180.");
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(servoPin, OUTPUT);

  // BLE Setup
  BLEDevice::init("XIAO-Servo-BLE");
  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");

  pCharacteristic = pService->createCharacteristic(
    "abcd1234-5678-90ab-cdef-1234567890ab",
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Waiting for angle via BLE...");
}

void loop() {
  if (newAngleAvailable) {
    Serial.print("Moving to angle: ");
    Serial.println(targetAngle);
    myServo_write(targetAngle);
    newAngleAvailable = false;
  }

  delay(100); // Small delay to prevent CPU overload
}
 