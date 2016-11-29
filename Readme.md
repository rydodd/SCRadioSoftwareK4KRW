# SCRadioSoftwareK4KRW

This software is for a Software Controlled Radio of the type introduced in the 
March 2016 **QST** magazine in an article titled:

**A Modular 40 Meter CW Transceiver with VFO**

The original project was the work of:

* Dr. Jack Purdum - W8TEE
* Farukh Zia - k2ZIA
* Dennis Kidder - W6DQ

The radio is a modified 49er QRP rig.  It is controlled by an Arduino Nano.  
The main oscillator is an AD9850 DDS.  

There is a yahoo group named **SoftwareControlledHamRadio** where project 
enthusiasts share their knowledge.

**SCRadioSoftwareK4KRW** was an effort to utilize my programming background in
a project that was solely for the fun of it.  The initial release was the result
of almost two months of work.  However, it was two months of 'when I could 
find time' work.  But, it did include at least three solid weekends of work.
So it was a significant amount of effort.  In the end I ended up with 
software that I wrote that could control a radio I assembled.  Of course, the
real thanks goes to Jack, Farukh and Dennis who got this whole thing started.
Enthusiasts in the Yahoo group have also been fantastic.

This software adds the following functionality to the rig.
* A menu system for controlling rig settings
* A rit on/off menu option
* A menu option for reversing the receive offset direction
* A backlight on/off option
* A rit offset adjustment.
* Multispeed tuning knob
* Startup check for a shorted key

The software makes the following assumptions:
* The microprocessor is an Arduino Nano with an AtMega 328 processor.
* The display is two lines of 16 charaters.  It may be that larger will work.
* You do not need a built in CW keyer.  Some earlier software provides one.
* Your rig has the Receive Offset hardware modifications

## Feedback

If you find a bug or if you would like a specific feature, please report it at:

https://github.com/rydodd/SCRadioSoftwareK4KRW/issues

## Credits

* The original software was created by Jack Purdum - W8TEE and Dennis Kidder - W6DQ
* The original logic for controlling the DDS was written by Richard Visokey - AD7C
* Thanks to Igor Mikolic-Torreira for his arduino-EventManager project which allowed
me to persue an event driven approach to the application.

## License

This software is free software; you can redistribute it and/or modify it under
the terms of the MIT License.

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the MIT license for more details.

A copy of the license is included in the **SCRadioSoftwareK4KRW** package.


## Copyright

Copyright (c) 2016 Richard Young Dodd

