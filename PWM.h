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

#ifndef PWM_H_
#define PWM_H_

#include <string>

namespace bbbkit {

class PWM {
public:
    enum PIN { P8_13    = 0,
               P8_19    = 1,
               P9_14    = 2,
               P9_16    = 3,
               P9_21    = 4,
               P9_22    = 5,
               P9_42    = 6,
               EHRPWM2B = P8_13,
               EHRPWM2A = P8_19,
               EHRPWM1A = P9_14,
               EHRPWM1B = P9_16,
               EHRPWM0B = P9_21,
               EHRPWM0A = P9_22,
               ECAP0    = P9_42,
             };

    enum VALUE{ LOW=0, HIGH=1 };

private:
    const std::string nameMap[7] = { "P8_13",
                                     "P8_19",
                                     "P9_14",
                                     "P9_16",
                                     "P9_21",
                                     "P9_22",
                                     "P9_42",
                                   };

    PWM::PIN pin;
    std::string path;
    std::string name;

public:
    PWM(PWM::PIN pin, PWM::VALUE activeState=PWM::VALUE::HIGH);
    virtual ~PWM();

    // Getters and setters

    virtual PWM::PIN getPin() { return pin; }
    virtual std::string getPath() { return path; }
    virtual std::string getName() { return name; }

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
};

} /* namespace bbbkit */

#endif /* PWM_H_ */
