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

#ifndef GPIO_H_
#define GPIO_H_

#include <fstream>
#include <string>

namespace bbbkit {

typedef int (*CallbackFunction_t)(int);

class GPIO {
public:
    enum DIRECTION{ INPUT, OUTPUT };
    enum VALUE{ LOW=0, HIGH=1 };
    enum EDGE{ NONE=0, RISING=1, FALLING=2, BOTH=3 };

private:
    int pin;
    std::string path;
    std::string name;
    int debounce;
    CallbackFunction_t threadCallbackFunction;
    pthread_t thread;
    bool threadRunning;
    std::ofstream stream;

public:
    GPIO(int pin, GPIO::DIRECTION direction, GPIO::VALUE activeState=HIGH);
    virtual ~GPIO();

    // Getters and setters
    
    virtual int getPin() { return pin; }
    virtual std::string getPath() { return path; }
    virtual std::string getName() { return name; }
    
    // Minimum delay between GPIO reads, in milliseconds
    virtual int getDebounce() { return debounce; }
    virtual int setDebounce(int debounce) { this->debounce = debounce; return 0; }

    virtual GPIO::DIRECTION getDirection();
    virtual int setDirection(GPIO::DIRECTION direction);

    virtual GPIO::VALUE getActiveState();
    virtual int setActiveState(GPIO::VALUE activeState);

    // General input
    
    virtual GPIO::VALUE getValue();

    // General output
    
    virtual int setValue(GPIO::VALUE value);

    // Edge input
    
    virtual GPIO::EDGE getEdgeType();
    virtual int setEdgeType(GPIO::EDGE edgeType);
    virtual int waitForEdge();
    virtual int waitForEdgeThread(CallbackFunction_t callbackFunction);
    virtual int stopWaitForEdgeThread() { this->threadRunning = false; return 0; }

    // Stream output
    
    virtual int streamOpen();
    virtual int streamSetValue(GPIO::VALUE value);
    virtual int streamClose();

private:
    int exportPin();
    int unexportPin();
    friend void *threadEdgePoll(void *value);
};

void *threadEdgePoll(void *value);

} /* namespace bbbkit */

#endif /* GPIO_H_ */

