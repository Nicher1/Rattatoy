//Inkludere Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Create Servo element
Servo PUSH;

//Servo values
#define PUSHDefault 90
#define PUSHNow 20

//Sensory output pins
#define BUZZER 3   //PIN 4 NÅR VI ER DONE!!!!!!!!
#define GreenLED 5

//Values for timing
int timer  = 0;
int timer2 = 0;
int period = 250;
int period2 = 2000;

//Light sesor pins & values
int lysSensorPin = A0;
byte lysSensorValue = 0;
int lysLimit = 120;
int ldrValue = 0;

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
byte address[6] = {"willy"};

void setup(){
  //radio.startListening();
  pinMode(BUZZER,   OUTPUT);
  pinMode(GreenLED, OUTPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
  Serial.begin(9600);
  PUSH.attach(6);
  PUSH.write(PUSHDefault);
}

void sendSignals(){
  int tal = 1;
  radio.write(&tal, sizeof(tal));
}


void loop(){
  sendSignals();
  lysSensorValue = analogRead(lysSensorPin)/4;
  Serial.println(lysSensorValue);

  if (lysSensorValue > lysLimit){
    digitalWrite(BUZZER, LOW);
    digitalWrite(GreenLED, LOW);
    PUSH.write(PUSHDefault);
    timer2 = millis();
    ldrValue = false;
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
    
    if (timer > period2){
    ldrValue = true;  
    }

    //Send message to receiver
    PUSH.write(PUSHNow);
  }
  
}
