// Include libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Create servo element
Servo PUSH;

// Servo values
#define PUSHDefault 90
#define PUSHNow 20

// Sensory output pins
#define BUZZER 4
#define GreenLED 5

// Set values for timing
int timer  = 0;
int timer2 = 0;
int darkTime = 0;
int pauseTime = 0;
int period = 10;
int period2 = 2;

// Set toggle-variable for current timerstate
// (If timer is in middle of period,
// Variable switches alternate state)
int x = 1;

// Define light sensor pins & values
int lysSensorPin = A0;
byte lysSensorValue = 0;
int lysLimit = 125;
int ldrValue = 0;                                   // Define date for radio transmission

// Create an RF24 object
RF24 radio(7, 8);  // CE, CSN

// Address through which two modules communicate.
byte address[6] = {"willy"};

void setup(){
  // Set outputstates
  pinMode(BUZZER,   OUTPUT);
  pinMode(GreenLED, OUTPUT);

  // Begin radio transmittion
  radio.begin();
  // Open pipe for communication
  radio.openWritingPipe(address);
  // Set as transmitter
  radio.stopListening();
  Serial.begin(9600);
  // Set pin 6 as servo motor pin
  PUSH.attach(6);
  // Set servo to default position
  PUSH.write(PUSHDefault);
}

void loop(){
  // Set variable to current LDR-value
  lysSensorValue = analogRead(lysSensorPin)/4;


  // Detect current LDR value compared to LDR limit value
  if (lysSensorValue > lysLimit){
    // Set everything to default setting, if LDR sensor isn't covered
    timer = millis() / 1000 - pauseTime;        
    darkTime = millis() / 1000;                 // Define variable that shows if stepper is moving
    digitalWrite(BUZZER, LOW);
    digitalWrite(GreenLED, LOW);
    PUSH.write(PUSHDefault);
    ldrValue = false;
  }
  // When timer over period x is set to 0 & pauseTime reset 
  if (timer > period){
    x = 1;
    pauseTime = millis() / 1000;
  }

  if (lysSensorValue <= lysLimit && x == 1){
    timer2 = millis() / 1000 - darkTime;         // Set timer 2 = time
    radio.write(&ldrValue, sizeof(ldrValue));    // Send ldrValue to slave
    // Buzzer & LED on when timer2 under 1/2 period2
    if (timer2 < period2 / 2){
      digitalWrite(BUZZER, HIGH);
      digitalWrite(GreenLED, HIGH);
    }
    // Buzzer off & ldrValue true when timer 2 between 1/2 & 3 period2
    if (timer2 > period2 / 2 && timer < 3 * period2){
      digitalWrite(BUZZER, LOW);
      ldrValue = true;
    }
    // ldrValue false & LED off when timer2 over 3 period2
    if (timer2 > 3 * period2){
      ldrValue = false;
      digitalWrite(GreenLED, LOW);
      pauseTime = millis() / 1000;               // pauseTime reset
      PUSH.write(PUSHNow);                       // Stepper push ball away
      // x set to 0 when lysSensorValue nears lyslimit
      if (lysSensorValue >= lysLimit - 20){
        x = 0;
      }
    }
  }
}
