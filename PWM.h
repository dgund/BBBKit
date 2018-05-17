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

#ifndef PWM_H
#define PWM_H

#include <string>

namespace bbbkit {

class PWM {
public:
    enum PIN {
        EHRPWM0A = 0,
        EHRPWM0B = 1,
        EHRPWM1A = 2,
        EHRPWM1B = 3,
        EHRPWM2A = 4,
        EHRPWM2B = 5,
        ECAPPWM0 = 6,
        ECAPPWM2 = 7,
        P8_13 = EHRPWM2B,
        P8_19 = EHRPWM2A,
        P8_34 = EHRPWM1B,
        P8_36 = EHRPWM1A,
        P8_45 = EHRPWM2A,
        P8_46 = EHRPWM2B,
        P9_14 = EHRPWM1A,
        P9_16 = EHRPWM1B,
        P9_21 = EHRPWM0B,
        P9_22 = EHRPWM0A,
        P9_28 = ECAPPWM2,
        P9_42 = ECAPPWM0,
    };

    enum VALUE{ LOW=0, HIGH=1 };

private:
    PWM::PIN pin;
    std::string path;
    std::string name;

public:
    PWM(PWM::PIN pin, PWM::VALUE activeState=PWM::VALUE::HIGH);
    virtual ~PWM();

    // Getters and setters

    virtual PWM::PIN getPin() { return this->pin; }
    virtual std::string getPath() { return this->path; }
    virtual std::string getName() { return this->name; }

    virtual PWM::VALUE getActiveState();
    virtual int setActiveState(PWM::VALUE activeState);

    virtual unsigned int getPeriod();
    virtual int setPeriod(unsigned int periodNS);

    virtual float getFrequency();
    virtual int setFrequency(float frequencyHZ);
    
    virtual unsigned int getDutyCycle();
    virtual float getDutyCycleAsPercent();
    virtual int setDutyCycle(unsigned int dutyCycleNS);
    virtual int setDutyCycleAsPercent(float dutyCyclePercent);

    virtual int start();
    virtual bool isRunning();
    virtual int stop();

private:
    int exportPin();
    int unexportPin();
};

} /* namespace bbbkit */

#endif /* PWM_H */
