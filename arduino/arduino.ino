#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Stepper.h>

const int STEPS_PER_REVOLUTION = 800;

Adafruit_MLX90614 thermometer = Adafruit_MLX90614();
Stepper stepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  thermometer.begin();
  stepper.setSpeed(75);
}

void loop() {
  //Serial.println(thermometer.readObjectTempF());
  stepper.step(STEPS_PER_REVOLUTION * -30);
  delay(1000);
  stepper.step(STEPS_PER_REVOLUTION * 30);
  delay(1000);
}
