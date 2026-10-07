#include <AccelStepper.h>
#include <ctype.h>


#define MP1 8
#define MP2 9
#define MP3 10
#define MP4 11


#define MotorInterfaceType 8
AccelStepper stepper = AccelStepper(MotorInterfaceType, MP1, MP3, MP2, MP4);
const int SPR = 4096; //Steps per revolution


boolean isValidNumber(String str) {
  str.trim(); // Buang spasi, \r, atau \n
  if (str.length() == 0) return false;


  byte startIndex = 0;
  // Izinkan tanda minus di depan untuk koordinat negatif
  if (str.charAt(0) == '-') {
    if (str.length() == 1) return false; // Hanya tanda '-' saja bukan angka
    startIndex = 1;
  }


  for (byte i = startIndex; i < str.length(); i++) {
    if (!isDigit(str.charAt(i))) {
      return false; // Kalau ada satu pun karakter selain digit, batalkan
    }
  }
  return true;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  stepper.moveTo(0);
  stepper.runToPosition();
}


void loop() {
  Serial.println("Enter target position: ");
  while (Serial.available() == 0) {
    // This loop blocks the code until user inputs data and presses Enter
  }
  String input = Serial.readStringUntil('\n');


  if(isValidNumber(input)) {
    Serial.println("Moving to " + input);
    stepper.moveTo(input.toInt());
    stepper.runToPosition();
  } else {
    Serial.println("Error: Target input is not a number.");
  }


}
