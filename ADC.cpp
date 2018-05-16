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

#include "ADC.h"

#include <fstream>
#include <iostream>
#include <sstream>

#define ADC_SYSFS_PATH "/sys/devices/iio:device0/in_voltage"
#define ADC_SYSFS_VOLTAGE_RAW "_raw"

namespace bbbkit {

    ADC::ADC(ADC::PIN pin) {
        this->pin = pin;
        this->name = this->nameMap[this->pin];

        std::stringstream adcPathStream;
        adcPathStream << ADC_SYSFS_PATH << pin << ADC_SYSFS_VOLTAGE_RAW;
        this->path = adcPathStream.str();
    }

    ADC::~ADC() {}
    
    // Read raw voltage value (in millivolts)
    int ADC::readVoltage() {
        int voltageMV = -1;
        std::ifstream adcValueStream;
        adcValueStream.open(this->path.c_str(), std::ios::in);
        if (!adcValueStream.is_open()) {
            perror("ADC: Failed to open stream.");
            return -1;
        }
        adcValueStream >> voltageMV;
        adcValueStream.close();
        return voltageMV;
    }

    // Read voltage as a ratio between min and max
    float ADC::readPercent() {
        int voltageMV = this->readVoltage();
        float ratio = ((float)voltageMV - float(this->voltageMinMV)) / (float(this->voltageMaxMV) - float(this->voltageMinMV));
        return ratio;
    }

} /* namespace bbbkit */

