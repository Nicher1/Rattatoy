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

byte lysSensorValue = 1;
int NutsLeft = 50;

int pos = 133;
byte address[6] = {"69420"};
RF24 radio(7, 8);  // CE, CSN

void setup(){ 
  lcd.init();
  lcd.backlight();

  lcd.createChar(1,aa);
  lcd.createChar(2,oe);
  Serial.begin(9600);
  
  stepper.setMaxSpeed(200);
  stepper.setAcceleration(50);
  
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(110);
}

void refreshLCD(){
//Fortæller hvor mange nædder der er tilbage i dispenseren.
  if (NutsLeft > 0){
    lcd.setCursor(0, 0);
    lcd.print("Antal n");
    lcd.setCursor(7,0);
    lcd.write(byte(2));
    lcd.setCursor(8,0);
    lcd.print("dder:");
    lcd.setCursor(0, 15);
    lcd.print(NutsLeft);
  }
  if (NutsLeft == 0){
//Fortæller at man skal påfylde nødder.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print('P');
    lcd.write(byte(1));
    lcd.setCursor(2,0);
    lcd.print("fyld n");
    lcd.setCursor(8,0);
    lcd.write(byte(2));
    lcd.setCursor(9,0);
    lcd.print("dder:");
  }
}

void loop(){ 
  radio.startListening();
  refreshLCD();
  
  if (radio.available()){
    radio.read(&lysSensorValue,sizeof(lysSensorValue));
    //Serial.println(lysSensorValue);
    radio.stopListening();
  }
  
  stepper.run();
  stepper.moveTo(pos);  
  
  Serial.println(pos);
  Serial.println(stepper.distanceToGo());
  Serial.println(NutsLeft);

  if (stepper.distanceToGo() == 0 && NutsLeft != 0){
    pos = pos + 133;
    NutsLeft = NutsLeft - 1;
  }
}
