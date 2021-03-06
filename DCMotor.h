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

#ifndef DCMOTOR_H
#define DCMOTOR_H

#include "PWM.h"

namespace bbbkit {

class DCMotor {

private:
    // PWM for motor speed
    PWM *pwm;
    // Motor speed as a percentage
    float speedPercent;

public:
    DCMotor(PWM::PIN pinPWM, int dutyCyclePeriodNS=1000, float speedPercent=0.0);
    virtual ~DCMotor();
    
    // Getters and setters

    virtual int getDutyCyclePeriod() { return this->pwm->getPeriod(); }
    virtual int setDutyCyclePeriod(unsigned int periodNS);

    virtual int getSpeedPercent() { return this->speedPercent; }
    virtual int setSpeedPercent(float speedPercent);

    // Motor control

    virtual int start();
    virtual bool isRunning();
    virtual int stop();
};

} /* namespace bbbkit */

#endif /* DCMOTOR_H */

