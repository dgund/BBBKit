# BBBKit
Library for simple and extensible I/O functionality on BeagleBone Black. Used for the TIDS martian ice drilling robot at Carnegie Mellon University.

The [Tartan Ice Drilling System](https://github.com/TartanIceDrillingSystem/tids-control) (TIDS), developed by students at Carnegie Mellon University for [NASA's Mars Ice Challenge](http://specialedition.rascal.nianet.org/), was a bold and innovative design for a Martian water extraction system and for the future of spaceflight. Our system was controlled by a [BeagleBone Black](https://beagleboard.org/black), and due to the rapid growth and continued development of the kernel at that time, no existing library supported all I/O operations. Thus, I developed BBBKit to be the low-level interface between our software and the BeagleBone. The development of BBBKit even led to some novel research into the relatively-undocumented kernel for [PWM configuration](https://stackoverflow.com/questions/50203064/pwm-on-beaglebone-black-v4-14) and the [PWM chip mappings](https://stackoverflow.com/questions/50204329/pwm-chip-to-pin-mapping-on-beaglebone-black-v4-14). The [tids-control](https://github.com/TartanIceDrillingSystem/tids-control) software, built on top of BBBKit, controlled the autonomous operation and communication with hardware component drivers.

I released this library as free and open-source software for future development of complex hardware systems using the BeagleBone Black. The library currently supports (kernel v4.14):

  - General purpose input/output (GPIO)
  - Pulse width modulation (PWM)
  - Analog-to-digital convertors (ADC)
  - I<sup>2</sup>C bus communication
  - DC motors
  - Servo motors
  - Stepper motors

## Usage

To install the library on a BeagleBone, in the `BBBKit/` directory run:

    make
    sudo make install
    sudo ldconfig
