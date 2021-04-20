//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 2, 3);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

int NutsLeft = 6;
int X = 1;

float pos = 133.3333;

const byte address[6] = "69420";
RF24 radio(7, 8);  // CE, CSN

void setup(){
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  radio.begin();
  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);
}

void loop(){
  //radio.openReadingPipe(address);
  if (stepper.distanceToGo() == 0){
    delay(500);
    stepper.moveTo(pos);
    pos = pos + 133.3333;
    X = 0;
  }
  stepper.run();
  radio.stopListening();
  
  if (stepper.distanceToGo() <= 20 && stepper.distanceToGo() >= 5 && X == 0){
    NutsLeft = NutsLeft - 1;
    X = 1;
  }

  lcd.setCursor(0, 0);
  lcd.print("Antal Jordnodder");
  lcd.setCursor(0, 1);
  lcd.print(NutsLeft);
}
