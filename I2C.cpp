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

#include "I2C.h"

#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_SYSFS_PATH "/dev/i2c-"

#define I2C_FILE_NULL -1

namespace bbbkit {

I2C::I2C(I2C::BUS bus, unsigned int deviceAddress) {
    this->bus = bus;

    // Format path for the BeagleBone
    std::stringstream pathStream;
    pathStream << I2C_SYSFS_PATH << static_cast<int>(this->bus);
    this->path = pathStream.str();

    this->deviceAddress = deviceAddress;
    this->file = I2C_FILE_NULL;
}

I2C::~I2C() {
    if (this->isOpen()) {
        this->close();
    }
}

int I2C::open() {
    // Open I2C bus
    if ((this->file = ::open((this->path).c_str(), O_RDWR)) < 0) {
        std::cout << "I2C: Error opening bus." << std::endl;
        return -1;
    }

    // Connect to I2C device
    if (ioctl(this->file, I2C_SLAVE, this->deviceAddress) < 0) {
        std::cout << "I2C: Error connecting to device." << std::endl;
        return -1;
    }

    return 0;
}

bool I2C::isOpen() {
    return (this->file != I2C_FILE_NULL);
}

int I2C::write(unsigned char value) {
    // Write value to device
    unsigned char buffer[1];
    buffer[0] = value;
    if (::write(this->file, buffer, 1) != 1) {
        std::cout << "I2C: Error writing to device." << std::endl;
        return -1;
    }
    return 0;
}

int I2C::writeRegister(unsigned int address, unsigned char value) {
    // Write register address and value to device
    unsigned char buffer[2];
    buffer[0] = address;
    buffer[1] = value;
    if (::write(this->file, buffer, 2) != 2) {
        std::cout << "I2C: Error writing to device register." << std::endl;
        return -1;
    }
    return 0;
}

unsigned char I2C::readRegister(unsigned int address) {
    // Request data from register address
    this->write(address);

    // Read response
    unsigned char buffer[1];
    if (::read(this->file, buffer, 1) != 1) {
        std::cout << "I2C: Error reading from device register." << std::endl;
        return -1;
    }

    // Return data
    return buffer[0];
}

unsigned char *I2C::readRegisters(unsigned int startAddress, unsigned int count) {
    // Request data from registers starting at startAddress
    this->write(startAddress);

    // Read response
    unsigned char *buffer = new unsigned char[count];
    if (::read(this->file, buffer, count) != (int)count) {
        std::cout << "I2C: Error reading from device registers." << std::endl;
        return NULL;
    }

    // Return data (must be freed externally)
    return buffer;
}

void I2C::close() {
    if (this->isOpen()) {
        std::cout << "I2C: Bus already closed." << std::endl;
    } else {
        ::close(this->file);
        this->file = I2C_FILE_NULL;
    }
}

} /* namespace bbbkit */
