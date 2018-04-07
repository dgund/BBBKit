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

#include "DCMotor.h"

namespace bbbkit {

DCMotor::DCMotor(GPIO *gpio, PWM *pwm, DCMotor::Direction direction, float speedPercent, int dutyCyclePeriodNS) {
    this->gpio = gpio;
    this->gpio->setDirection(GPIO::OUTPUT);
    this->pwm = pwm;
    this->pwm->setActiveState(PWM::VALUE::LOW);

    this->setDirection(direction);
    this->setSpeedPercent(speedPercent);
    this->setDutyCyclePeriod(dutyCyclePeriodNS);
}

DCMotor::~DCMotor() {}

// Getters and setters

int DCMotor::setDirection(DCMotor::DIRECTION direction)  {
    this->direction = direction;

    if (this->direction == DCMotor::DIRECTION::CLOCKWISE){
        return this->gpio->setValue(GPIO::HIGH);
    }
    else {
        return this->gpio->setValue(GPIO::LOW);
    }
}

int DCMotor::setSpeedPercent(float speedPercent)  {
    this->speedPercent = speedPercent;
    return this->pwm->setDutyCycleAsPercent(this->speedPercent);
}

int DCMotor::setDutyCyclePeriod(unsigned int periodNS)  {
    return this->pwm->setPeriod(periodNS);
}

// Motor control

int DCMotor::start() {
    return this->pwm->start();
}

bool DCMotor::isRunning() {
    return this->pwm->isRunning();
}

int DCMotor::stop()  {
    return this->pwm->stop();
}

} /* namespace bbbkit */

