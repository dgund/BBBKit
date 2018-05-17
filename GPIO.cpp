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

#include "GPIO.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Readwrite.h"

#define GPIO_SYSFS_PATH "/sys/class/gpio/"
#define GPIO_SYSFS_GPIO "gpio"
#define GPIO_SYSFS_DELAY 300000

#define GPIO_SYSFS_EXPORT "export"
#define GPIO_SYSFS_UNEXPORT "unexport"

#define GPIO_SYSFS_ACTIVE_LOW "active_low"
#define GPIO_SYSFS_ACTIVE_LOW_FALSE 0
#define GPIO_SYSFS_ACTIVE_LOW_TRUE 1

#define GPIO_SYSFS_DIRECTION "direction"
#define GPIO_SYSFS_DIRECTION_INPUT "in"
#define GPIO_SYSFS_DIRECTION_OUTPUT "out"

#define GPIO_SYSFS_EDGE "edge"
#define GPIO_SYSFS_EDGE_RISING "rising"
#define GPIO_SYSFS_EDGE_FALLING "falling"
#define GPIO_SYSFS_EDGE_BOTH "both"
#define GPIO_SYSFS_EDGE_NONE "none"

#define GPIO_SYSFS_VALUE "value"

namespace bbbkit {

GPIO::GPIO(GPIO::PIN pin, GPIO::DIRECTION direction, GPIO::VALUE activeState) {
    this->pin = pin;
    this->debounce = 0;
    this->threadCallbackFunction = NULL;
    this->threadRunning = false;

    // Format the name and path for the BeagleBone
    std::stringstream nameStream;
    nameStream << GPIO_SYSFS_GPIO << static_cast<int>(this->pin);
    this->name = nameStream.str();
    this->path = GPIO_SYSFS_PATH + this->name + "/";

    // Export the pin (delay to give Linux some time)
    this->exportPin();
    usleep(GPIO_SYSFS_DELAY);

    // Set direction and active state
    this->setDirection(direction);
    this->setActiveState(activeState);
}

GPIO::~GPIO() {
    this->unexportPin();
}

// Getters and setters

int GPIO::getDebounce() {
    return this->debounce;
}

int GPIO::setDebounce(int debounce) {
    this->debounce = debounce;
    return 0;
}

GPIO::DIRECTION GPIO::getDirection() {
    std::string directionString = read(this->path, GPIO_SYSFS_DIRECTION);
    if (directionString == GPIO_SYSFS_DIRECTION_INPUT) {
        return INPUT;
    } else {
        return OUTPUT;
    }
}

int GPIO::setDirection(GPIO::DIRECTION direction) {
    switch (direction) {
    case INPUT:
        return write(this->path, GPIO_SYSFS_DIRECTION, GPIO_SYSFS_DIRECTION_INPUT);
    case OUTPUT:
        return write(this->path, GPIO_SYSFS_DIRECTION, GPIO_SYSFS_DIRECTION_OUTPUT);
    }
    return -1;
}

GPIO::VALUE GPIO::getActiveState() {
    if (std::stoi(read(this->path, GPIO_SYSFS_ACTIVE_LOW)) == GPIO_SYSFS_ACTIVE_LOW_TRUE) {
        return GPIO::VALUE::LOW;
    } else {
        return GPIO::VALUE::HIGH;
    }
}

int GPIO::setActiveState(GPIO::VALUE activeState) {
    switch (activeState) {
    case LOW:
        return write(this->path, GPIO_SYSFS_ACTIVE_LOW, GPIO_SYSFS_ACTIVE_LOW_TRUE);
    case HIGH:
        return write(this->path, GPIO_SYSFS_ACTIVE_LOW, GPIO_SYSFS_ACTIVE_LOW_FALSE);
    }
    return -1;
}

// General input

GPIO::VALUE GPIO::getValue() {
    if (std::stoi(read(this->path, GPIO_SYSFS_VALUE)) == static_cast<int>(GPIO::VALUE::LOW)) {
        return GPIO::VALUE::LOW;
    } else {
        return GPIO::VALUE::HIGH;
    }
}

// General output

int GPIO::setValue(GPIO::VALUE value) {
    return write(this->path, GPIO_SYSFS_VALUE, static_cast<int>(value));
}

// Edge input

GPIO::EDGE GPIO::getEdgeType() {
    std::string edgeTypeString = read(this->path, GPIO_SYSFS_EDGE);
    if (edgeTypeString == GPIO_SYSFS_EDGE_RISING) {
        return RISING;
    } else if (edgeTypeString == GPIO_SYSFS_EDGE_FALLING) {
        return FALLING;
    } else if (edgeTypeString == GPIO_SYSFS_EDGE_BOTH) {
        return BOTH;
    } else {
        return NONE;
    }
}

int GPIO::setEdgeType(GPIO::EDGE edgeType) {
    switch(edgeType) {
    case NONE:
        return write(this->path, GPIO_SYSFS_EDGE, GPIO_SYSFS_EDGE_NONE);
    case RISING:
        return write(this->path, GPIO_SYSFS_EDGE, GPIO_SYSFS_EDGE_RISING);
    case FALLING:
        return write(this->path, GPIO_SYSFS_EDGE, GPIO_SYSFS_EDGE_FALLING);
    case BOTH:
        return write(this->path, GPIO_SYSFS_EDGE, GPIO_SYSFS_EDGE_BOTH);
    }
    return -1;
}

int GPIO::waitForEdge() {
    // Check if direction is input
    if (this->getDirection() != INPUT) {
        std::cout << "GPIO: Edge pin must be input." << std::endl;
        return -1;
    }

    // Create epoll instance
    int epollfd = epoll_create(1);
    if (epollfd < 0) {
        std::cout << "GPIO: Failed to create epoll instance." << std::endl;
        return -1;
    }

    // Create file descriptor
    int fd = open((this->path + GPIO_SYSFS_VALUE).c_str(), O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        std::cout << "GPIO: Failed to open file." << std::endl;
        return -1;
    }

    // Configure epoll event
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET | EPOLLPRI;
    event.data.fd = fd;

    // Register event and file descriptor on epoll instance
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) < 0) {
        std::cout << "GPIO: Failed to register epoll event." << std::endl;
        close (fd);
        return -1;
    }

    // Wait for an edge trigger (ignoring the first one)
    int edgeCount = 0;
    while (edgeCount <= 1) {
        int trigger = epoll_wait(epollfd, &event, 1, -1);
        if (trigger < 0){
            std::cout << "GPIO: epoll_wait failed." << std::endl;
            close (fd);
            return -1;
        }
        edgeCount++;
    }

    close(fd);
    return 0;
}

int GPIO::waitForEdgeThread(CallbackFunction_t callbackFunction) {
    if (this->threadRunning) {
        std::cout << "GPIO: Edge thread already running." << std::endl;
        return -1;
    }
    
    this->threadCallbackFunction = callbackFunction;
    this->threadRunning = true;
    if(pthread_create(&this->thread, NULL, &threadEdgePoll, static_cast<void *>(this)) != 0) {
        std::cout << "GPIO: Failed to create the edge thread." << std::endl;
        this->threadRunning = false;
        return -1;
    }
    return 0;
}

int GPIO::stopWaitForEdgeThread() {
    this->threadRunning = false;
    return 0;
}

// Stream output

int GPIO::streamOpen() {
    stream.open((this->path + GPIO_SYSFS_VALUE).c_str());
    if (!stream.is_open()) {
        std::cout << "GPIO: Failed to open stream." << std::endl;
        return -1;
    }
    return 0;
}

int GPIO::streamSetValue(GPIO::VALUE value) {
    stream << value << std::flush;
    return 0;
}

int GPIO::streamClose() {
    stream.close();
    return 0;
}

// Private

int GPIO::exportPin() {
   return write(GPIO_SYSFS_PATH, GPIO_SYSFS_EXPORT, static_cast<int>(this->pin));
}

int GPIO::unexportPin() {
   return write(GPIO_SYSFS_PATH, GPIO_SYSFS_UNEXPORT, static_cast<int>(this->pin));
}

void *threadEdgePoll(void *value) {
    GPIO *gpio = static_cast<GPIO *>(value);
    while(gpio->threadRunning) {
        gpio->threadCallbackFunction(gpio->waitForEdge());
        usleep(gpio->debounce * 1000);
    }
    return 0;
}

} /* namespace bbbkit */

