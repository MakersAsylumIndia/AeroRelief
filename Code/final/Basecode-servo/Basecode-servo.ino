int servoPin = 18;

void setup() {
  pinMode(servoPin, OUTPUT);
}

void loop() {

delay(2000);
 
  myServo_write(0);     // Move to 0 degrees
  delay(1000);

  myServo_write(90);    // Move to 90 degrees
  delay(1000);

  myServo_write(180);   // Move to 180 degrees
  delay(1000);
}

// Custom servo write function like myServo.write(angle)
void myServo_write(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2500);  // microseconds

  // Send PWM pulses for about 500ms (25 pulses at 50Hz)
  for (int i = 0; i < 25; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(servoPin, LOW);
    delayMicroseconds(20000 - pulseWidth);
  }
}
