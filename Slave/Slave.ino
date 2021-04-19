#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 2, 3);

float pos = 133.3333;

void setup()
{ 
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);
}

void loop()
{
  if (stepper.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);\
  }
  stepper.run();
}
