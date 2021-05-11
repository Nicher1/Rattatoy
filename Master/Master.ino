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
#define BUZZER 4   //PIN 4 NÅR VI ER DONE!!!!!!!!
#define GreenLED 5

//Values for timing
int timer  = 0;
int timer2 = 0;
int darkTime = 0;
int pauseTime = 0;

int x = 1;

int period = 10;
int period2 = 2;

//Light sesor pins & values
int lysSensorPin = A0;
byte lysSensorValue = 0;
int lysLimit = 125;
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

void loop(){
  lysSensorValue = analogRead(lysSensorPin)/4;
  //Serial.println(lysSensorValue);
  
  if (lysSensorValue > lysLimit){
    timer = millis() / 1000 - pauseTime;
    darkTime = millis() / 1000;
  }
  //Serial.println(ldrValue);
  //Serial.println(x);
  //Serial.println();
  
  if (lysSensorValue > lysLimit){
    digitalWrite(BUZZER, LOW);
    digitalWrite(GreenLED, LOW);
    PUSH.write(PUSHDefault);
    ldrValue = false;  
  }
  //Serial.println(pauseTime);
  Serial.println(x);
  if (timer > period){
    x = 1;
    pauseTime = millis() / 1000;
  }

  if (lysSensorValue <= lysLimit && x == 1){
    timer2 = millis() / 1000 - darkTime;
    radio.write(&ldrValue, sizeof(ldrValue));
    //Serial.println(ldrValue);
    if (timer2 < period2 / 2){
      digitalWrite(BUZZER, HIGH);
      digitalWrite(GreenLED, HIGH);  
    }
    if (timer2 > period2 / 2 && timer < 3 * period2){
      digitalWrite(BUZZER, LOW);
      ldrValue = true;
    }
    if (timer2 > 3 * period2){
      ldrValue = false;
      digitalWrite(GreenLED, LOW);
      pauseTime = millis() / 1000;
      PUSH.write(PUSHNow);
      if (lysSensorValue >= lysLimit - 20){
        x = 0;
      }
    }
  }
}
