#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN

String lysSensorValue;

const byte address[] [6] = {"Fishy", "Tacos"};

void setup() {
  radio.begin();
  radio.openReadingPipe(1, address[0]);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(110);
  Serial.begin(9600);
}

void loop() {
  radio.startListening();
  if (radio.available()){
    radio.read(&lysSensorValue[0], sizeof(lysSensorValue));
    radio.stopListening();
  }
  if(lysSensorValue != 0){
    Serial.println(lysSensorValue);
  }
}
