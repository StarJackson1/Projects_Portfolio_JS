#include <Arduino.h>
#include <Wire.h>
#include <BleKeyboard.h>

#define Xpin 34
#define Ypin 35

int jvalX = 0;
int jvalY = 0;

BleKeyboard bleKeyboard;

void playPause() {
  Serial.println("Sending Play/Pause");
  bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
  delay(300);
}

void skip() {
  Serial.println("Sending Skip Song");
  bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
  delay(300);
}

void rewind() {
  Serial.println("Sending Rewind Song");
  bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
  delay(300);
}

void stop() {
  Serial.println("Sending Stop");
  bleKeyboard.write(KEY_MEDIA_STOP);
  delay(300);
}

int readJoystickX() {
  //Serial.println("Reading Joystick X Data");
  int rawDataX = analogRead(Xpin);
  int formattedDataX;
  formattedDataX = map(rawDataX, 0, 4095, -100, 100);
  return formattedDataX;

}

int readJoystickY() {
  //Serial.println("Reading Joystick Y Data");
  int rawDataY = analogRead(Ypin);
  int formattedDataY;
  formattedDataY = map(rawDataY, 0, 4095, -100, 100);
  return formattedDataY;

}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing a Bitch");
  bleKeyboard.setName("ESP32 Media Controller");
  bleKeyboard.begin();
  bleKeyboard.setBatteryLevel(100);
}

void loop() {
    int jvalX = readJoystickX();
    int jvalY = readJoystickY();


  //Serial.print("X value: ");
  //Serial.println(jvalX);
  //Serial.print("Y value: ");
  //Serial.println(jvalY);
  //delay(500);
  //if statement for joystick being pushed rightward
  if (jvalX > 50) {
    skip();
  }
  //if statement for joystick being pushed leftward
  if (jvalX < -50) {
    rewind();
  }
  //if statement for joystick being pushed upward
  if (jvalY > 50) {
    playPause();
  }
  //if statement for joystick being pushed downward
}

