//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 2, 3);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

byte aa[8] {B00100,B00000,B01110,B00001,B01111,B10001,B01111};  // "å" til lcd
byte oe[8] {B00000,B00001,B01110,B10011,B10101,B11001,B01110};  // "ø" til lcd

int ldrValue = 0;
int nutsLeft = 5;

int pos = 266;
int addPos = 266;
byte address[6] = {"willy"};
RF24 radio(7, 8);  // CE, CSN

void setup(){ 
  lcd.init();
  lcd.backlight();

  refreshLCD();
  
  lcd.createChar(1,aa);
  lcd.createChar(2,oe);
  Serial.begin(9600);
  
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(800);
  
  radio.begin();
  radio.openReadingPipe(1, address);
}

void loop(){ 
  radio.startListening();
  radio.read(&ldrValue,sizeof(ldrValue));
    
  Serial.println(ldrValue);

  if (ldrValue == 1 && nutsLeft != 0){
    stepper.run();
    stepper.moveTo(pos);
    
    //Serial.println(pos);
    //Serial.println(stepper.distanceToGo());
    //Serial.println(nutsLeft);

    if (stepper.distanceToGo() == 0 && nutsLeft != 0){
      pos = pos + addPos;
      nutsLeft = nutsLeft - 1;
      refreshLCD();
      delay(5000);
    }
    
    if (nutsLeft == 0){
      pos = addPos * 5;
    }
  }
}

void refreshLCD(){
//Fortæller hvor mange nædder der er tilbage i dispenseren.
  if (nutsLeft > 0){
    lcd.setCursor(0, 0);
    lcd.print("Antal n");
    lcd.setCursor(7,0);
    lcd.write(byte(2)); // ø
    lcd.setCursor(8,0);
    lcd.print("dder:");
    lcd.setCursor(0, 15);
    lcd.print(nutsLeft);
  }
  if (nutsLeft == 0){
//Fortæller at man skal påfylde nødder.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print('P');
    lcd.write(byte(1)); // å
    lcd.setCursor(2,0);
    lcd.print("fyld n");
    lcd.setCursor(8,0);
    lcd.write(byte(2));
    lcd.setCursor(9,0);
    lcd.print("dder:");
  }
}
