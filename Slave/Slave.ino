//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 2, 3);

float pos = 133.3333;

const byte address[6] = "69420";
RF24 radio(7, 8);  // CE, CSN

void setup(){
  Serial.bein(9600);
  radio.begin();
   
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);
}

void loop()
{
  radio.openReadingPipe(address);
  if (stepper.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);\
  }
  stepper.run();
  radio.stopListening();
}
