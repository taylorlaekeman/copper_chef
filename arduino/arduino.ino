#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Stepper.h>

const int STEPS_PER_REVOLUTION = 800;

Adafruit_MLX90614 thermometer = Adafruit_MLX90614();
Stepper linearSteppers(STEPS_PER_REVOLUTION, 8, 9, 10, 11);
Stepper rotationalStepper(STEPS_PER_REVOLUTION, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  thermometer.begin();
  linearSteppers.setSpeed(75);
  rotationalStepper.setSpeed(75);
  
  demonstrate();
}

void loop() {
  Serial.println(thermometer.readObjectTempC());
  delay(500);
}

void demonstrate() {
  linearSteppers.step(STEPS_PER_REVOLUTION * 10);
  delay(100);
  rotationalStepper.step(STEPS_PER_REVOLUTION * 0.5);
  delay(100);
  linearSteppers.step(STEPS_PER_REVOLUTION * -10);
  delay(100);
}
