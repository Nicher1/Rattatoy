//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo PUSH;

#define PUSHDefault 90
#define PUSHNow 20

#define BUZZER 7

int lysSensorPin = A0;
int lysSensorValue = 0;

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup(){
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER,LOW);
  radio.begin();
  Serial.begin(9600);
  
  PUSH.attach(6);
  PUSH.write(PUSHDefault);
  
  radio.openWritingPipe(address);    //set the address
  radio.stopListening();             //Set module as transmitter
}
void loop(){
digitalWrite(BUZZER, LOW);
lysSensorValue = analogRead(lysSensorPin);
  
if (lysSensorValue <=750){
  //Send message to receiver
  //const char text[] = "Hello World";
  //radio.write(&text, sizeof(text));
  PUSH.write(PUSHNow);
  digitalWrite(BUZZER, HIGH);
}
if (lysSensorValue > 750){
  PUSH.write(PUSHDefault);
}
Serial.println(lysSensorValue);
}
