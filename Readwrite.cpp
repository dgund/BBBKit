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

#include "Readwrite.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace bbbkit {

std::string read(std::string filepath, std::string filename) {
    std::ifstream readStream;
    readStream.open((filepath + filename).c_str());
    if (!readStream.is_open()) {
        perror("Readwrite: Read failed to open file.");
        std::cout << filepath + filename << std::endl;
    }
    std::string value;
    std::getline(readStream, value);
    readStream.close();
    return value;
}

int write(std::string filepath, std::string filename, std::string value) {
    std::ofstream writeStream;
    writeStream.open((filepath + filename).c_str());
    if (!writeStream.is_open()) {
       perror("Readwrite: Write failed to open file.");
       std::cout << filepath + filename << std::endl;
       return -1;
    }
    writeStream << value;
    writeStream.close();
    return 0;
}

int write(std::string filepath, std::string filename, int value) {
    std::stringstream valueStream;
    valueStream << value;
    return write(filepath, filename, valueStream.str());
}

} /* namespace bbbkit */

