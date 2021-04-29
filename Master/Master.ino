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

int X = 6; //Antal af nødder der er tilbage i dispenseren

//Light sesor pins & values
int lysSensorPin = A0;
int lysSensorValue = 0;
int lysLimit = 650;

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

void setup(){
  pinMode(BUZZER,   OUTPUT);
  pinMode(GreenLED, OUTPUT);
  
  radio.begin();
  radio.setChannel(60);
  
  Serial.begin(9600);
  
  PUSH.attach(6);
  PUSH.write(PUSHDefault);
}

void loop(){
  lysSensorValue = analogRead(lysSensorPin); //Sætter LysSensorValue til LDR værdien
  radio.openWritingPipe(0,0xF0F0F0F0AALL);
  
  if (lysSensorValue > lysLimit && X > 0){   
    digitalWrite(BUZZER, LOW);
    digitalWrite(GreenLED, LOW);
    PUSH.write(PUSHDefault);
    timer2 = millis();
    X = X - 1;
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
    radio.write(lysSensorValue, sizeof(lysSensorValue));
    radio.stop();             //Set module as transmitter
    PUSH.write(PUSHNow);
  }
  
  Serial.println(lysSensorValue);
}
