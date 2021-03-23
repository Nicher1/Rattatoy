//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Stepper.h> // Include the header file

// change this to the number of steps on your motor
#define STEPS 32
#define step1 6
#define step2 7

// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 3, 5, 4, 6);

int val = 0;
//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";
//void setup()
void setup()
{
  while (!Serial);
    Serial.begin(9600);
  stepper.setSpeed(200);
  radio.begin();
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);

  //set the address
  radio.openReadingPipe(0, address);

  //Set module as receiver
  radio.startListening();
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  if (Serial.available()>0)
  {
    val = Serial.parseInt();
    stepper.step(val);
    Serial.println(val); //for debugging
  }
  delay(1);
  digitalWrite(step1, HIGH);
  delay(1);
  digitalWrite(step1, LOW);
}
