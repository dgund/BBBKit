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

#include "PWM.h"

#include "Readwrite.h"

#define PWM_SYSFS_PATH "/sys/devices/ocp.3/"
#define PWM_SYSFS_DUTY "duty"
#define PWM_SYSFS_PERIOD "period"
#define PWM_SYSFS_POLARITY "polarity"
#define PWM_SYSFS_RUN "run"

#define PERCENT_MIN 0.0
#define PERCENT_MAX 100.0
#define NANO 1000000000.0

namespace bbbkit {

PWM::PWM(PWM::PIN pin, PWM::VALUE activeState) {
    this->pin = pin;
    this->name = this->nameMap[this->pin];
    this->path = PWM_SYSFS_PATH + this->name + "/";
    this->setActiveState(activeState);
}

PWM::~PWM() {}

PWM::VALUE PWM::getActiveState() {
    if (std::stoi(read(this->path, PWM_SYSFS_POLARITY)) == PWM::VALUE::LOW) return PWM::VALUE::LOW;
    else return PWM::VALUE::HIGH;
}

int PWM::setActiveState(PWM::VALUE activeState) {
    return write(this->path, PWM_SYSFS_POLARITY, activeState);
}

unsigned int PWM::getPeriod() {
    return std::stoi(read(this->path, PWM_SYSFS_PERIOD));
}

int PWM::setPeriod(unsigned int periodNS) {
    return write(this->path, PWM_SYSFS_PERIOD, periodNS);
}

float PWM::getFrequency() {
    unsigned int periodNS = this->getPeriod();
    float periodS = (float)periodNS / NANO;
    float frequencyHZ = 1.0 / periodS;
    return frequencyHZ;
}

int PWM::setFrequency(float frequencyHZ) {
    float periodS = 1.0 / frequencyHZ;
    unsigned int periodNS = (unsigned int)(periodS * NANO);
    return this->setPeriod(periodNS);
}

unsigned int PWM::getDutyCycle() {
    return std::stoi(read(this->path, PWM_SYSFS_DUTY));
}

float PWM::getDutyCycleAsPercent() {
    return PERCENT_MAX * (float)this->getDutyCycle() / (float)this->getPeriod();
}

int PWM::setDutyCycle(unsigned int dutyCycleNS) {
    return write(this->path, PWM_SYSFS_DUTY, dutyCycleNS);
}

int PWM::setDutyCycleAsPercent(float dutyCyclePercent) {
    if (PERCENT_MIN > dutyCyclePercent || dutyCyclePercent > PERCENT_MAX) return -1;
    return this->setDutyCycle((float)this->getPeriod() * (dutyCyclePercent / PERCENT_MAX));
}

int PWM::start() {
    return write(this->path, PWM_SYSFS_RUN, PWM::VALUE::HIGH);
}

bool PWM::isRunning() {
    return std::stoi(read(this->path, PWM_SYSFS_RUN)) == PWM::VALUE::HIGH;
}

int PWM::stop() {
    return write(this->path, PWM_SYSFS_RUN, PWM::VALUE::LOW);
}

} /* namespace bbbkit */
