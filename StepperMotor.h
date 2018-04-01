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

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

#include "GPIO.h"

namespace bbbkit {

class StepperMotor {

public:
    enum DIRECTION { CLOCKWISE, COUNTERCLOCKWISE };

private:
    // PLS
    GPIO *gpio_PLS;
    // DIR
    GPIO *gpio_DIR;
    // AWO
    GPIO *gpio_AWO;
    // CS
    GPIO *gpio_CS;
    // ALM
    GPIO *gpio_ALM;
    // TIM
    GPIO *gpio_TIM;

    DIRECTION direction;
    int stepsPerRevolution;
    float revolutionsPerMinute;
    bool sleeping;
    // Delay between steps, in microseconds
    int stepDelay;

    pthread_t thread;
    bool threadRunning;
    CallbackFunction_t threadCallbackFunction;
    int threadStepCount;

public:
    StepperMotor(int pin_PLS, int pin_DIR, int pin_AWO, int pin_CS,
                 int pin_ALM, int pin_TIM, int stepsPerRevolution = 1000,
                 int revolutionsPerMinute = 60);
    virtual ~StepperMotor();
    
    // Getters and setters
    
    virtual DIRECTION getDirection() { return this->direction; }
    virtual int setDirection(DIRECTION direction);

    virtual int getStepsPerRevolution() { return this->stepsPerRevolution; }
    virtual int setStepsPerRevolution(int stepsPerRevolution);

    virtual float getRevolutionsPerMinute() { return this->revolutionsPerMinute; }
    virtual int setRevolutionsPerMinute(float revolutionsPerMinute);
    
    virtual bool getSleeping() { return this->sleeping; }
    virtual int setSleeping(bool sleeping);

    virtual GPIO::VALUE getAlarm();
    virtual GPIO::VALUE getTimer();

    // Stepping

    virtual void step();
    virtual void step(int numberOfSteps);
    virtual void rotate(float degrees);

    // Threaded stepping

    virtual int threadStep(int numberOfSteps, CallbackFunction_t callbackFunction);
    virtual int threadStepForever();
    virtual void stopThreadStep() { this->threadRunning = false; }

private:
    virtual int updateStepDelay();
    friend void *threadStepInternal(void *value);
};

void *threadStepInternal(void *value);

} /* namespace bbbkit */

#endif /* STEPPERMOTOR_H_ */
