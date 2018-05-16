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

DCMotor(PWM::PIN pinPWM, int dutyCyclePeriodNS, float speedPercent); {
    this->pwm = new PWM(pinPWM);

    this->setDutyCyclePeriod(dutyCyclePeriodNS);
    this->setSpeedPercent(speedPercent);
}

DCMotor::~DCMotor() {
    delete this->pwm;
}

// Getters and setters

int DCMotor::setDutyCyclePeriod(unsigned int periodNS)  {
    return this->pwm->setPeriod(periodNS);
}

int DCMotor::setSpeedPercent(float speedPercent)  {
    this->speedPercent = speedPercent;
    return this->pwm->setDutyCycleAsPercent(this->speedPercent);
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

