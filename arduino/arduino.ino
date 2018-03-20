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

const int STEPS_PER_REVOLUTION = 800;

const int RARE_COOKING_TIME = 235000;
const int MEDIUM_RARE_COOKING_TIME = 255000;
const int MEDIUM_COOKING_TIME = 275000;
const int MEDIUM_WELL_COOKING_TIME = 295000;
const int WELL_DONE_COOKING_TIME = 315000;

steak_doneness set_doneness;
device_state current_state = DEVICE_PAN_NOT_READY;
Adafruit_MLX90614 thermometer = Adafruit_MLX90614();
Stepper linearSteppers(STEPS_PER_REVOLUTION, 28, 29, 30, 31);
Stepper rotationalStepper(STEPS_PER_REVOLUTION, 24, 25, 26, 27);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); //FOR TESTING
  configure_connections();
  
  Serial.begin(9600);
  thermometer.begin();
  
  //delay(15000);
  
  //wait_for_pan();
  int cooking_time = read_switch();
  cook_steak(cooking_time);
}

void configure_connections() {
  configure_switch();
  configure_led();
  configure_motors();  
}

void configure_switch() {
  pinMode(switch_pin_1, INPUT);
  pinMode(switch_pin_3, INPUT);
  pinMode(switch_pin_4, INPUT);
  pinMode(switch_pin_6, INPUT);
}

void configure_led() {
  pinMode(led_red, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_green, OUTPUT);
}

void configure_motors() {
  linearSteppers.setSpeed(35);
  rotationalStepper.setSpeed(75);
}

void wait_for_pan() {
  Serial.println("WAITING FOR PAN");
  do {
    float temperature = read_temperature();
    Serial.println(temperature);
    if (is_pan_temperature_within_range(temperature)) {
      for (int n=0; n<5; n++) {
        delay(10000);
        temperature = read_temperature();
        if (!!is_pan_temperature_within_range(temperature)) {
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
  Serial.println("PAN READY");
}

float read_temperature() {
  return thermometer.readObjectTempC();
}

boolean is_pan_temperature_within_range(float temperature) {
  return (thermometer.readObjectTempC() >= 125) && (thermometer.readObjectTempC() <= 135);
}

int read_switch() {
  int cooking_time = 0;
  if (digitalRead(switch_pin_1)) {
    Serial.println("RARE");
    cooking_time = RARE_COOKING_TIME;
  } else if (digitalRead(switch_pin_3)) {
    Serial.println("MEDIUM RARE");
    cooking_time = MEDIUM_RARE_COOKING_TIME;
  } else if(digitalRead(switch_pin_4)) {
    Serial.println("MEDIUM WELL");
    cooking_time = MEDIUM_WELL_COOKING_TIME;
  } else if(digitalRead(switch_pin_6)){
    Serial.println("WELL DONE");
    cooking_time = WELL_DONE_COOKING_TIME;
  } else {
    Serial.println("MEDIUM");
    cooking_time = MEDIUM_COOKING_TIME;
  }
  return cooking_time;
}

void cook_steak(int cooking_time) {
  //Serial.println("COOKING TIME: " + cooking_time);
  //lower_cage();
  //delay(cooking_time);
  raise_cage();
  /*delay(1000);
  rotate_cage();
  delay(1000);
  lower_cage();
  delay(cooking_time);
  raise_cage();*/
  delay(120000);
  digitalWrite(led_green, HIGH);
}

void lower_cage()  {
  Serial.println("LOWERING");
  rotationalStepper.step(STEPS_PER_REVOLUTION * -17);
}

void raise_cage() {
  Serial.println("RAISE CAGE");
  rotationalStepper.step(STEPS_PER_REVOLUTION * 17);
}

void rotate_cage() {
  Serial.println("ROTATE CAGE");
  linearSteppers.step(STEPS_PER_REVOLUTION * -0.52);
}

void loop() {}
