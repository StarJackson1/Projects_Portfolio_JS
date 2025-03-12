//Libraries
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPL115A2.h>
#include <Encoder.h>
#include <math.h>
//Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C  // Connect: A4(SDA), A5(SCL)
//Object Definitions
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPL115A2 sensor1;
//knob declarations
int DAT = 3;
int CLK = 2;
Encoder knob(DAT, CLK); // DAT(Pin 3), CLK(Pin 2)
//Variables
float pressureKPA;
float adjustedKPA;
float temperatureC;
int counter = 0;
int knobStepsPer1 = 4;
int displayNum;
float baroADJ = 1.23;
String displayType;
String displayInfo1;
String displayInfo2;
String displayInfo3;

void setup() {
  Serial.begin(9600);
  knob.write(0);
  if(!sensor1.begin()) {
    Serial.println("Sensor no worky");
    while(1);
  }
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.cp437(true);
}

void loop() {
  //Sensor Code
  pressureKPA = sensor1.getPressure();
  adjustedKPA = (pressureKPA*baroADJ);
  temperatureC = sensor1.getTemperature();
  //Knob control
  if (counter < 0) {
    knob.write(0);
  }
  if (counter > 3) {
    knob.write(3*knobStepsPer1);
  }
  counter = knob.read()/4;
  //baro if
  if (counter <= 0) {
    displayType = "SL-Baro";
    displayInfo1 = (String(adjustedKPA) + "KPA");
    displayInfo2 = (String(millibars(adjustedKPA)) + "MB");
    displayInfo3 = (String(inHG(adjustedKPA)) + "inHg");
  }
  //baro non-sea level if
  if (counter == 1) {
    displayType = "NSL-Baro";
    displayInfo1 = (String(pressureKPA) + "KPA");
    displayInfo2 = (String(millibars(pressureKPA)) + "MB");
    displayInfo3 = (String(inHG(pressureKPA)) + "inHg");
  }
  //temp if
  if (counter == 2) {
    displayType = "Temp";
    displayInfo1 = (String(temperatureC) + "C");
    displayInfo2 = (String(farenheit(temperatureC)) + "F");
    displayInfo3 = "";
  }
  //alt if
  if (counter >= 3) {
    displayType = "Altimeter";
    displayInfo1 = (String(altitude(pressureKPA)));
    displayInfo2 = "Feet";
    displayInfo3 = "";

  }
  //Display control
  display.setTextSize(2);
  display.println(displayType);
  display.println(displayInfo1);
  display.println(displayInfo2);
  display.println(displayInfo3);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setCursor(0,0);
}
float farenheit(float in) {
  float mult = (9.0/5.0);
  return (in*mult + 32);
}
float millibars(float in) {
  return (in*10); 
}
float inHG(float in) {
  return (in/3.386);
}
float altitude(float in) {
  float mb = millibars(in);
  float math1 = (mb/1013.25);
  float math2 = pow(math1, 0.190284);
  return 145366.45*(1-math2);
}

