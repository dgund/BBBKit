/*
    Copyright 2018 Devin Gund (https://dgund.com)
    https://github.com/dgund/BBBKit/

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "StepperMotor.h"

#include <cmath>
#include <unistd.h>

namespace bbbkit {

StepperMotor::StepperMotor(GPIO::PIN pinStep, GPIO::PIN pinDirection, GPIO::PIN pinSleep,
                            StepperMotor::DIRECTION direction,
                            int stepsPerRevolution, float revolutionsPerMinute, int stepFactor) {
    // Initialize GPIO pins
    this->gpioStep = new GPIO(pinStep, GPIO::DIRECTION::OUTPUT);
    this->gpioDirection = new GPIO(pinDirection, GPIO::DIRECTION::OUTPUT);
    this->gpioSleep = new GPIO(pinSleep, GPIO::DIRECTION::OUTPUT);

    // Set delay
    this->stepsPerRevolution = stepsPerRevolution;
    this->revolutionsPerMinute = revolutionsPerMinute;
    this->stepFactor = stepFactor;
    this->updateStepDelay();

    // Set direction
    this->setDirection(direction);

    this->setIsSleeping(false);
}

StepperMotor::~StepperMotor() {}

// Getters and setters

int StepperMotor::setDirection(StepperMotor::DIRECTION direction) {
    this->direction = direction;

    if (this->direction == StepperMotor::DIRECTION::CLOCKWISE){
        return this->gpioDirection->setValue(GPIO::VALUE::HIGH);
    }
    else {
        return this->gpioDirection->setValue(GPIO::VALUE::LOW);
    }
}

int StepperMotor::setStepFactor(int stepFactor) {
    this->stepFactor = stepFactor;
    return 0;
}

int StepperMotor::setStepsPerRevolution(int stepsPerRevolution) {
    this->stepsPerRevolution = stepsPerRevolution;
    return this->updateStepDelay();
}

int StepperMotor::setRevolutionsPerMinute(float revolutionsPerMinute) {
    this->revolutionsPerMinute = revolutionsPerMinute;
    return this->updateStepDelay();
}

int StepperMotor::setIsSleeping(bool isSleeping) {
    this->isSleeping = isSleeping;
    if (this->isSleeping) {
        return this->gpioSleep->setValue(GPIO::VALUE::HIGH);
    }
    else {
        return this->gpioSleep->setValue(GPIO::VALUE::LOW);
    }
}

// Stepping

void StepperMotor::step() {
    this->gpioStep->setValue(GPIO::VALUE::LOW);
    this->gpioStep->setValue(GPIO::VALUE::HIGH);
}

void StepperMotor::step(int numberOfSteps) {
    int delay = this->stepDelayUS / this->stepFactor;
    for (int steps = 0; steps < numberOfSteps; steps++) {
        this->step();
        usleep(delay);
    }
}

void StepperMotor::rotate(float angle) {
    float stepAngle = 360.0f/(float)(this->stepsPerRevolution);
    int numberOfSteps = floor((float)this->stepFactor * angle / stepAngle + 0.5);
    this->step(numberOfSteps);
}

// Private

int StepperMotor::updateStepDelay() {
    // Assumes that the step signal takes no time (false but negligible)
    float stepDelay = (60.0f / this->revolutionsPerMinute) / this->stepsPerRevolution;
    this->stepDelayUS = (int)(stepDelay * 1000.0f * 1000.0f);
    return 0;
}

} /* namespace bbbkit */

