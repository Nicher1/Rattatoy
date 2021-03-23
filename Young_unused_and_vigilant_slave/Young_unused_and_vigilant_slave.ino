//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo PUSH;

#define PUSHDefault 90
#define PUSHNow 20

#define BUZZER 3   //PIN 4 NÅR VI ER DONE!!!!!!!!
#define GreenLED 5

int timer  = 0;
int timer2 = 0;
int period = 250;

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
  lysSensorValue = analogRead(lysSensorPin);
  radio.openWritingPipe(address);    //set the address  
  
  if (lysSensorValue > lysLimit){
    digitalWrite(BUZZER, LOW);
    digitalWrite(GreenLED, LOW);
    PUSH.write(PUSHDefault);
    timer2 = millis();
  }
    
  if (lysSensorValue <= lysLimit){
    timer = millis() - timer2;
    
    if (timer <= period){
    digitalWrite(BUZZER, HIGH);
    digitalWrite(GreenLED, HIGH);
    }
    
    if (timer > period){
    digitalWrite(BUZZER, LOW);
    }
    
    //Send message to receiver
    radio.write(&lysSensorValue, sizeof(lysSensorValue));
    radio.stopListening();             //Set module as transmitter
    PUSH.write(PUSHNow);
  }
  
  Serial.println(lysSensorValue);
}
