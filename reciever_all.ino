#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>

// Servo pins
const int servoPin1 = 3;
const int servoPin2 = 6;
const int servoPin3 = 2;

// Servo objects
Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  Serial.begin(9600);
  // Avoid blocking if Serial is not open
  LoRa.setPins(10, 9, 4);  // NSS, RESET, DIO0

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  Serial.println("LoRa Servo Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize == 6 && LoRa.available() >= 6) {
    byte buffer[6];
    for (int i = 0; i < 6; i++) {
      buffer[i] = LoRa.read();
    }

    int joyX = (buffer[0] << 8) | buffer[1];
    int joyY = (buffer[2] << 8) | buffer[3];
    int joyZ = (buffer[4] << 8) | buffer[5];

    int angle1 = map(joyX, 0, 1023, 0, 180);
    int angle2 = map(joyY, 0, 1023, 0, 120);
    int angle3 = map(joyZ, 0, 1023, 50, 180);

    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);

    Serial.print("X: "); Serial.print(joyX);
    Serial.print(" | Y: "); Serial.print(joyY);
    Serial.print(" | Z: "); Serial.println(joyZ);
  }

  // Optional: small delay to allow stability
  delay(5);
}
