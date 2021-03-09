//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo PUSH;

#define PUSHDefault 90
#define PUSHNow 20

#define BUZZER 7
#define GreenLED 5

int lysSensorPin = A0;
int lysSensorValue = 0;
int lysLimit = 650;

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup(){
  pinMode(BUZZER,   OUTPUT);
  pinMode(GreenLED, OUTPUT);
  radio.begin();
  Serial.begin(9600);
  
  PUSH.attach(6);
  PUSH.write(PUSHDefault);
  
}
void loop(){
digitalWrite(BUZZER, LOW);
digitalWrite(GreenLED, LOW);
lysSensorValue = analogRead(lysSensorPin);
  
if (lysSensorValue <= lysLimit){
  //Send message to receiver
  radio.openWritingPipe(address);    //set the address
  radio.write(&lysSensorValue, sizeof(lysSensorValue));
  radio.stopListening();             //Set module as transmitter
  PUSH.write(PUSHNow);
  digitalWrite(BUZZER, HIGH);
  digitalWrite(GreenLED, HIGH);
}
if (lysSensorValue > lysLimit){
  PUSH.write(PUSHDefault);
}
Serial.println(lysSensorValue);
}
