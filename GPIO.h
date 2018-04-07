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
    enum PIN {
        // P9 Header
        GPIO_30  =30,  GPIO_60  =60,
        GPIO_31  =31,  GPIO_50  =50,
        GPIO_48  =48,  GPIO_51  =51,
        GPIO_5   =5,   GPIO_4   =4,
        GPIO_3   =3,   GPIO_2   =2,
        GPIO_49  =49,  GPIO_15  =15,
        GPIO_117 =117, GPIO_14  =14,
        GPIO_115 =115, GPIO_113 =114,
        GPIO_111 =111, GPIO_112 =112,
        GPIO_110 =110,
        GPIO_20  =20,  GPIO_7   =7,
        // P8 Header
        GPIO_38  =38,  GPIO_39  =39,
        GPIO_34  =34,  GPIO_35  =35,
        GPIO_66  =66,  GPIO_67  =67,
        GPIO_69  =69,  GPIO_68  =68,
        GPIO_45  =45,  GPIO_44  =44,
        GPIO_23  =23,  GPIO_26  =26,
        GPIO_47  =47,  GPIO_46  =46,
        GPIO_27  =27,  GPIO_65  =65,
        GPIO_22  =22,  GPIO_63  =63,
        GPIO_62  =62,  GPIO_37  =37,
        GPIO_36  =36,  GPIO_33  =33,
        GPIO_32  =32,  GPIO_61  =61,
        GPIO_86  =86,  GPIO_88  =88,
        GPIO_87  =87,  GPIO_89  =89,
        GPIO_10  =10,  GPIO_11  =11,
        GPIO_9   =9,   GPIO_81  =81,
        GPIO_8   =8,   GPIO_80  =80,
        GPIO_78  =78,  GPIO_79  =79,
        GPIO_76  =76,  GPIO_77  =77,
        GPIO_74  =74,  GPIO_75  =75,
        GPIO_72  =72,  GPIO_73  =73,
        GPIO_70  =70,  GPIO_71  =71,
    };
    enum DIRECTION { INPUT, OUTPUT };
    enum VALUE { LOW=0, HIGH=1 };
    enum EDGE { NONE=0, RISING=1, FALLING=2, BOTH=3 };

private:
    GPIO::PIN pin;
    std::string path;
    std::string name;
    int debounce;
    CallbackFunction_t threadCallbackFunction;
    pthread_t thread;
    bool threadRunning;
    std::ofstream stream;

public:
    GPIO(GPIO::PIN pin, GPIO::DIRECTION direction, GPIO::VALUE activeState=HIGH);
    virtual ~GPIO();

    // Getters and setters
    
    virtual GPIO::PIN getPin() { return this->pin; }
    virtual std::string getPath() { return this->path; }
    virtual std::string getName() { return this->name; }
    
    // Minimum delay between GPIO reads, in milliseconds
    virtual int getDebounce() { return this->debounce; }
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

