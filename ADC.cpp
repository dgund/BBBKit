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

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

//#define ADC_SYSFS_PATH(input) "/sys/bus/iio/iio:device0/in_voltage" STR(input) "_raw"
#define ADC_SYSFS_PATH(input) "/sys/bus/iio/devices/iio:device0/in_voltage" STR(input) "_raw" // For old sysfs (pre-4.14)
#define ADC_SYSFS_ADC(input) "AIN" STR(input)

const std::string pathMap[7] = {
    ADC_SYSFS_PATH(0),
    ADC_SYSFS_PATH(1),
    ADC_SYSFS_PATH(2),
    ADC_SYSFS_PATH(3),
    ADC_SYSFS_PATH(4),
    ADC_SYSFS_PATH(5),
    ADC_SYSFS_PATH(6),
};

const std::string nameMap[7] = {
    ADC_SYSFS_ADC(0),
    ADC_SYSFS_ADC(1),
    ADC_SYSFS_ADC(2),
    ADC_SYSFS_ADC(3),
    ADC_SYSFS_ADC(4),
    ADC_SYSFS_ADC(5),
    ADC_SYSFS_ADC(6),
};

namespace bbbkit {

    ADC::ADC(ADC::PIN pin, int voltageMinMV, int voltageMaxMV) {
        this->pin = pin;
        this->path = pathMap[static_cast<int>(this->pin)];
        this->name = nameMap[static_cast<int>(this->pin)];
        this->voltageMinMV = voltageMinMV;
        this->voltageMaxMV = voltageMaxMV;
    }

    ADC::~ADC() {}
    
    // Read raw voltage value in millivolts (averaged over count)
    int ADC::readVoltage(int count) {
        if (count < 1) {
            return 0;
        }

        // Open stream
        std::ifstream adcStream;
        adcStream.open(this->path.c_str(), std::ios::in);
        if (!adcStream.is_open()) {
            std::cout << "ADC: Failed to open stream." << std::endl;
            return -1;
        }

        // Sum ADC voltage over count
        long voltageSumMV = -1;
        for (int i = 0; i < count; i++) {
            // Read ADC
            int voltageMV = -1;
            adcStream >> voltageMV;
            // Add to sum
            voltageSumMV += voltageMV;
        }

        // Close stream
        adcStream.close();

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

