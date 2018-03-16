#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <Stepper.h>

typedef enum {
  RARE,
  MED_RARE,
  MED,
  MED_WELL,
  WELL_DONE
} steak_doneness;

typedef enum {
    DEVICE_PAN_NOT_READY,
    DEVICE_PAN_READY,
    DEVICE_LOWERED
} device_state;
const int switch_pin_1 = 2;
const int switch_pin_3 = 3;
const int switch_pin_4 = 4;
const int switch_pin_6 = 5;
const int led_red = 6;
const int led_blue = 7;
const int led_green = 8; 
int TEST = 0;
const int STEPS_PER_REVOLUTION = 800;
// State for steak doneness selection
steak_doneness set_doneness;
device_state current_state = DEVICE_PAN_NOT_READY; //initially set state to idle
int timeout_interval = 0;
Adafruit_MLX90614 thermometer = Adafruit_MLX90614();
Stepper linearSteppers(STEPS_PER_REVOLUTION, 28, 29, 30, 31);
Stepper rotationalStepper(STEPS_PER_REVOLUTION, 24, 25, 26, 27);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); //FOR TESTING 
  configure_switch();
  configure_led();
  configure_motors();
  
  Serial.begin(9600);
  thermometer.begin();
  wait_for_pan();
  lower_cage();
  delay(255000);
  raise_cage();
  delay(15000);
  rotate_cage();
  delay(1000);
  lower_cage();
  delay(255000);
  raise_cage();
}

void wait_for_pan() {
  do {
    Serial.println(thermometer.readObjectTempC());
    if ((thermometer.readObjectTempC() >= 125) && (thermometer.readObjectTempC() <= 135)) {
      for (int n=0; n<5; n++) {
        delay(10000);
        if (!(thermometer.readObjectTempC() >= 125) && (thermometer.readObjectTempC() <= 135)) {
          current_state = DEVICE_PAN_READY;
          break;
        } 
        digitalWrite(led_blue, LOW);
        digitalWrite(led_red, LOW);
        current_state = DEVICE_PAN_READY;   
      }  
    }
    delay(1000);
  } while (current_state == DEVICE_PAN_NOT_READY);
}

void lower_cage()  {
  rotationalStepper.step(STEPS_PER_REVOLUTION * -17);
}

void raise_cage() {
  rotationalStepper.step(STEPS_PER_REVOLUTION * 17);
}

void rotate_cage() {
  linearSteppers.step(STEPS_PER_REVOLUTION * -0.52);
}

// Configuring switches pins as digital inputs
void configure_switch() {
  pinMode(switch_pin_1, INPUT);
  pinMode(switch_pin_3, INPUT);
  pinMode(switch_pin_4, INPUT);
  pinMode(switch_pin_6, INPUT);
}

//Configuring LED pins as digital outputs
void configure_led() {
  pinMode(led_red, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_green, OUTPUT);  
}

void set_steak_doneness() {
  if (digitalRead(switch_pin_1)) {
      set_doneness = RARE; 
  } else if (digitalRead(switch_pin_3)) {
      set_doneness = MED_RARE;
  } else if(digitalRead(switch_pin_4)) {
      set_doneness = MED_WELL;
  } else if(digitalRead(switch_pin_6)){
      set_doneness = WELL_DONE;
  } else {
    set_doneness = MED;
  }
}

void configure_motors() {
  linearSteppers.setSpeed(35);
  rotationalStepper.setSpeed(75);
}

void loop() {}
