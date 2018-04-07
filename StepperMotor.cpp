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
#include <iostream>
#include <unistd.h>

namespace bbbkit {

StepperMotor::StepperMotor(GPIO *gpioPLS, GPIO *gpioDIR, GPIO *gpioAWO, GPIO *gpioCS,
                           GPIO *gpioALM, GPIO *gpioTIM,
                           StepperMotor::DIRECTION direction,
                           int stepsPerRevolution, int revolutionsPerMinute) {
    this->gpioPLS = gpioPLS;
    this->gpioDIR = gpioDIR;
    this->gpioAWO = gpioAWO;
    this->gpioCS = gpioCS;
    this->gpioALM = gpioALM;
    this->gpioTIM = gpioTIM;

    // Set delay
    this->stepsPerRevolution = stepsPerRevolution;
    this->revolutionsPerMinute = revolutionsPerMinute;
    this->updateStepDelay();

    // Set direction
    this->setDirection(direction);

    // Set default step angle to basic
    if (this->gpioCS != NULL) {
        this->gpioCS->setValue(GPIO::HIGH);
    }
}

StepperMotor::~StepperMotor() {}

// Getters and setters

int StepperMotor::setDirection(StepperMotor::DIRECTION direction) {
    this->direction = direction;

    if (this->direction == StepperMotor::DIRECTION::CLOCKWISE){
        return this->gpioDIR->setValue(GPIO::HIGH);
    }
    else {
        return this->gpioDIR->setValue(GPIO::LOW);
    }
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
        return this->gpioAWO->setValue(GPIO::HIGH);
    }
    else {
        return this->gpioAWO->setValue(GPIO::LOW);
    }
}

GPIO::VALUE StepperMotor::getAlarm() {
    return this->gpioALM->getValue();
}

GPIO::VALUE StepperMotor::getTimer() {
    return this->gpioTIM->getValue();
}

// Stepping

void StepperMotor::step() {
    this->gpioPLS->setValue(GPIO::HIGH);
    this->gpioPLS->setValue(GPIO::LOW);
}

void StepperMotor::step(int numberOfSteps) {
    for (int steps = 0; steps < numberOfSteps; steps++) {
        this->step();
        usleep(this->stepDelayUS);
    }
}

void StepperMotor::rotate(float angle) {
    float stepAngle = 360.0f/(float)(this->stepsPerRevolution);
    int numberOfSteps = round(angle / stepAngle);
    this->step(numberOfSteps);
}

// Private

int StepperMotor::updateStepDelay() {
    // Assumes that the step signal takes no time (false but negligible)
    float revolutionsPerMicrosecond = (60.0f * 1000.0f * 1000.0f) / (float)(this->revolutionsPerMinute);
    this->stepDelayUS = (int)(revolutionsPerMicrosecond / this->stepsPerRevolution);
    return 0;
}

} /* namespace bbbkit */

