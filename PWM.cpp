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

#include <unistd.h>

#include "Readwrite.h"

/*
    The PWM interface configuration appears to change between kernel updates, so
    these paths may change. To determine the new PWM interface paths, see my
    post at https://stackoverflow.com/questions/50204329/pwm-chip-to-pin-mapping-on-beaglebone-black-v4-14.
*/

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define PWM_SYSFS_PATH_PWMCHIP(chip) "/sys/class/pwm/pwmchip" STR(chip)
#define PWM_SYSFS_PWM(chip, channel) "pwm-" STR(chip) ":" STR(channel)

#define PWM_SYSFS_EHRPWM0_CHIP 1
#define PWM_SYSFS_EHRPWM0A_CHANNEL 0
#define PWM_SYSFS_EHRPWM0B_CHANNEL 1
#define PWM_SYSFS_EHRPWM1_CHIP 3
#define PWM_SYSFS_EHRPWM1A_CHANNEL 0
#define PWM_SYSFS_EHRPWM1B_CHANNEL 1
#define PWM_SYSFS_EHRPWM2_CHIP 6
#define PWM_SYSFS_EHRPWM2A_CHANNEL 0
#define PWM_SYSFS_EHRPWM2B_CHANNEL 1
#define PWM_SYSFS_ECAP0_CHIP 0
#define PWM_SYSFS_ECAP0_CHANNEL 0

#define PWM_SYSFS_PATH_EHRPWM0 PWM_SYSFS_PATH_PWMCHIP(PWM_SYSFS_EHRPWM0_CHIP)
#define PWM_SYSFS_PATH_EHRPWM1 PWM_SYSFS_PATH_PWMCHIP(PWM_SYSFS_EHRPWM1_CHIP)
#define PWM_SYSFS_PATH_EHRPWM2 PWM_SYSFS_PATH_PWMCHIP(PWM_SYSFS_EHRPWM2_CHIP)
#define PWM_SYSFS_PATH_ECAP0   PWM_SYSFS_PATH_PWMCHIP(PWM_SYSFS_ECAP0_CHIP)

#define PWM_SYSFS_EHRPWM0A PWM_SYSFS_PWM(PWM_SYSFS_EHRPWM0_CHIP, PWM_SYSFS_EHRPWM0A_CHANNEL)
#define PWM_SYSFS_EHRPWM0B PWM_SYSFS_PWM(PWM_SYSFS_EHRPWM0_CHIP, PWM_SYSFS_EHRPWM0B_CHANNEL)
#define PWM_SYSFS_EHRPWM1A PWM_SYSFS_PWM(PWM_SYSFS_EHRPWM1_CHIP, PWM_SYSFS_EHRPWM1A_CHANNEL)
#define PWM_SYSFS_EHRPWM1B PWM_SYSFS_PWM(PWM_SYSFS_EHRPWM1_CHIP, PWM_SYSFS_EHRPWM1B_CHANNEL)
#define PWM_SYSFS_EHRPWM2A PWM_SYSFS_PWM(PWM_SYSFS_EHRPWM2_CHIP, PWM_SYSFS_EHRPWM2A_CHANNEL)
#define PWM_SYSFS_EHRPWM2B PWM_SYSFS_PWM(PWM_SYSFS_EHRPWM2_CHIP, PWM_SYSFS_EHRPWM2B_CHANNEL)
#define PWM_SYSFS_ECAP0    PWM_SYSFS_PWM(PWM_SYSFS_ECAP0_CHIP,   PWM_SYSFS_ECAP0_CHANNEL)

#define PWM_SYSFS_DELAY 200000
#define PWM_SYSFS_EXPORT "export"
#define PWM_SYSFS_DUTY "duty"
#define PWM_SYSFS_PERIOD "period"
#define PWM_SYSFS_POLARITY "polarity"
#define PWM_SYSFS_RUN "run"
#define PWM_SYSFS_UNEXPORT "unexport"

#define PERCENT_MIN 0.0
#define PERCENT_MAX 100.0
#define NANO 1000000000.0

const std::string pathMap[7] = {
    PWM_SYSFS_PATH_EHRPWM0,
    PWM_SYSFS_PATH_EHRPWM0,
    PWM_SYSFS_PATH_EHRPWM1,
    PWM_SYSFS_PATH_EHRPWM1,
    PWM_SYSFS_PATH_EHRPWM2,
    PWM_SYSFS_PATH_EHRPWM2,
    PWM_SYSFS_PATH_ECAP0,
};

const std::string nameMap[7] = {
    PWM_SYSFS_EHRPWM0A,
    PWM_SYSFS_EHRPWM0B,
    PWM_SYSFS_EHRPWM1A,
    PWM_SYSFS_EHRPWM1B,
    PWM_SYSFS_EHRPWM2A,
    PWM_SYSFS_EHRPWM2B,
    PWM_SYSFS_ECAP0,
};

const int channelMap[7] = {
    PWM_SYSFS_EHRPWM0A_CHANNEL,
    PWM_SYSFS_EHRPWM0B_CHANNEL,
    PWM_SYSFS_EHRPWM1A_CHANNEL,
    PWM_SYSFS_EHRPWM1B_CHANNEL,
    PWM_SYSFS_EHRPWM2A_CHANNEL,
    PWM_SYSFS_EHRPWM2B_CHANNEL,
    PWM_SYSFS_ECAP0_CHANNEL,
};

namespace bbbkit {

PWM::PWM(PWM::PIN pin, PWM::VALUE activeState) {
    this->pin = pin;
    this->name = nameMap[static_cast<int>(this->pin)];
    this->path = pathMap[static_cast<int>(this->pin)] + this->name + "/";

    // Export the pin (delay to give Linux some time)
    this->exportPin();
    usleep(PWM_SYSFS_DELAY);

    // Set active state
    this->setActiveState(activeState);
}

PWM::~PWM() {
    this->unexportPin();
}

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

// Private

int PWM::exportPin() {
   return write(pathMap[static_cast<int>(this->pin)], PWM_SYSFS_EXPORT, channelMap[static_cast<int>(this->pin)]);
}

int PWM::unexportPin() {
   return write(pathMap[static_cast<int>(this->pin)], PWM_SYSFS_UNEXPORT, channelMap[static_cast<int>(this->pin)]);
}

} /* namespace bbbkit */
