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

#ifndef ADC_H
#define ADC_H

#include <string>

namespace bbbkit {

class ADC {
public:
    enum PIN {
        AIN0 = 0,
        AIN1 = 1,
        AIN2 = 2,
        AIN3 = 3,
        AIN4 = 4,
        AIN5 = 5,
        AIN6 = 6,
        P9_33 = AIN4,
        P9_35 = AIN6,
        P9_36 = AIN5,
        P9_37 = AIN2,
        P9_38 = AIN3,
        P9_39 = AIN0,
        P9_40 = AIN1,
    };

private:
    ADC::PIN pin;
    std::string path;
    std::string name;
    int voltageMinMV;
    int voltageMaxMV;

public:
    ADC(ADC::PIN pin, int voltageMinMV=0, int voltageMaxMV=1800);
    virtual ~ADC();
    
    virtual ADC::PIN getPin() { return this->pin; }
    virtual std::string getPath() { return this->path; }
    virtual std::string getName() { return this->name; }

    // Read raw voltage value in millivolts (averaged over count)
    virtual int readVoltage(int count=1);

    // Read voltage as a ratio between min and max (averaged over count)
    virtual float readRatio(int count=1);
};

} /* namespace bbbkit */

#endif /* ADC_H */

