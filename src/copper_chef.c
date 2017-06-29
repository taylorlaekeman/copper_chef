#include<stdio.h>
#include<time.h>
#include<stdlib.h>

const int COOKING_TEMPERATURE = 325, STABLE_THRESHOLD = 10, WITH_STABILIZATION = 1, WITHOUT_STABILIZATION = 0;

int readTargetTemperature() {/* not yet implemented */}

int senseTemperature() {/* not yet implemented */}

void signalPanTooCold() {/* not yet implemented */}

void signalPanTooHot() {/* not yet implemented */}

void signalPanJustRight() {/* not yet implemented */}

void waitForTemperature(int targetTemperature, int withStabilization) {
  int temperature = 0, stableCount = 0, stableThreshold = (withStabilization) ? STABLE_THRESHOLD : 1;

  while (stableCount < stableThreshold) {
    temperature = senseTemperature();
    if (temperature < targetTemperature)
      signalPanTooCold();
    else if (temperature > targetTemperature)
      signalPanTooHot();
    else {
      signalPanJustRight();
      stableCount++;
    }
  }
}

void lowerSteak() {/* not yet implemented */}

void raiseSteak() {/* not yet implemented */}

void rotateSteak() {/* not yet implemented */}

void signalCompletion() {/* not yet implemented */}

void main(int argc, char *argv[]) {
  int targetTemperature = readTargetTemperature(), startTemperature;
  time_t firstSideStartTime, firstSideEndTime, secondSideStartTime, secondSideEndTime;

  waitForTemperature(COOKING_TEMPERATURE, WITH_STABILIZATION);

  lowerSteak();
  startTemperature = senseTemperature();

  firstSideStartTime = clock();
  waitForTemperature((targetTemperature - startTemperature) / 2, WITHOUT_STABILIZATION);
  firstSideEndTime = clock();

  raiseSteak();
  rotateSteak();
  lowerSteak();

  secondSideStartTime = clock();
  secondSideEndTime = secondSideStartTime + (firstSideEndTime - firstSideStartTime);
  while (clock() < secondSideEndTime) {}
  
  raiseSteak();

  signalCompletion();
}
