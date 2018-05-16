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

#ifndef I2C_H
#define I2C_H

#include <string>

namespace bbbkit {

class I2C {
public:
    enum BUS {
        I2C_1 = 1,
        I2C_2 = 2,
    };

private:
    I2C::BUS bus;
    std::string path;
    unsigned int deviceAddress;
    int file;

public:
    I2C(I2C::BUS bus, unsigned int deviceAddress);
    virtual ~I2C();

    // Open I2C bus to device
    virtual int open();

    // Check if I2C bus is open
    virtual bool isOpen();

    // Write value to I2C device
    virtual int write(unsigned char value);

    // Write value to I2C device register
    virtual int writeRegister(unsigned int address, unsigned char value);

    // Read value from I2C device register
    virtual unsigned char readRegister(unsigned int address);

    // Read value from multiple I2C device registers (in one read operation)
    virtual unsigned char *readRegisters(unsigned int startAddress, unsigned int count);

    // Close I2C bus to device
    virtual void close();
};

} /* namespace bbbkit */

#endif /* I2C_H */
