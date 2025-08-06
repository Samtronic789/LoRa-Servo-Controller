
#include <SPI.h>
#include <LoRa.h>

// Joystick pins
const int joystickX = A1;
const int joystickY = A2;
const int joystickZ = A3;

// LoRa control pins
const int LORA_SS = 10;
const int LORA_RST = 9;
const int LORA_DIO0 = 2;

// Previous joystick values
int prevX = 550;
int prevY = 550;
int prevZ = 550;

// Minimum change required to send new data
const int threshold = 5;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(joystickZ, INPUT);

  Serial.println("LoRa Joystick Transmitter Ready");
}

void loop() {
  int valX = analogRead(joystickX);
  int valY = analogRead(joystickY);
  int valZ = analogRead(joystickZ);

  bool changed = false;

  // Check for significant change
  if (abs(valX - prevX) > threshold) {
    prevX = valX;
    changed = true;
  }
  if (abs(valY - prevY) > threshold) {
    prevY = valY;
    changed = true;
  }
  if (abs(valZ - prevZ) > threshold) {
    prevZ = valZ;
    changed = true;
  }

  // Only send if any axis changed
  if (changed) {
    LoRa.beginPacket();
    LoRa.write(prevX >> 8); LoRa.write(prevX & 0xFF);
    LoRa.write(prevY >> 8); LoRa.write(prevY & 0xFF);
    LoRa.write(prevZ >> 8); LoRa.write(prevZ & 0xFF);
    LoRa.endPacket();

    Serial.print("Sent X: "); Serial.print(prevX);
    Serial.print(" | Y: "); Serial.print(prevY);
    Serial.print(" | Z: "); Serial.println(prevZ);
  }

  delay(100); // Adjust as needed for responsiveness
}

