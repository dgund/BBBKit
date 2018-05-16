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

    ADC::ADC(ADC::PIN pin, int voltageMinMV, int voltageMaxMV) {
        this->pin = pin;
        this->name = this->nameMap[this->pin];
        this->voltageMinMV = voltageMinMV;
        this->voltageMaxMV = voltageMaxMV;

        std::stringstream adcPathStream;
        adcPathStream << ADC_SYSFS_PATH << pin << ADC_SYSFS_VOLTAGE_RAW;
        this->path = adcPathStream.str();
    }

    ADC::~ADC() {}
    
    // Read raw voltage value in millivolts (averaged over count)
    int ADC::readVoltage(int count) {
        if (count < 1) {
            return 0;
        }

        // Open stream
        std::ifstream adcValueStream;
        adcValueStream.open(this->path.c_str(), std::ios::in);
        if (!adcValueStream.is_open()) {
            perror("ADC: Failed to open stream.");
            return -1;
        }

        // Sum ADC voltage over count
        long voltageSumMV = -1;
        for (int i = 0; i < count; i++) {
            // Read ADC
            int voltageMV = -1;
            adcValueStream >> voltageMV;
            // Add to sum
            voltageSumMV += voltageMV;
        }

        // Close stream
        adcValueStream.close();

        // Average ADC voltage over count
        return static_cast<int>(voltageSumMV / count);
    }

    // Read voltage as a ratio between min and max (averaged over count)
    float ADC::readRatio(int count) {
        int voltageMV = this->readVoltage(count);
        float ratio = ((float)voltageMV - float(this->voltageMinMV)) / (float(this->voltageMaxMV) - float(this->voltageMinMV));
        return ratio;
    }

} /* namespace bbbkit */

