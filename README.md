# Wherlpool
Whirlpool refrigerator Arduino ATmega8 circuit board and code project.

It features DS1302 timekeeping chip.
Arduino Library: Copyright (c) 2009, Matt Sparks (2-clause BSD license).
[DS1302 RTC library for Arduino](https://github.com/msparks/arduino-ds1302)


About this project:
It contains all documents to assemble and program a circuit board for
a Whirlpool refrigerator, or any other refrigerator with a few (or none) modifications.
It consists of 2 projects:
+ Eagle CAD circuit board
+ Arduino based firmware (ATmega8)


### PCB signal connections:

```
[Signal name]           : [pin] : [direction]
CP = control panel      : ----------
CP Reset button         : PD3   : in
CP +/- (up/dn) button   : PD4   : in
CP Led1                 : PC3   : out
CP Led2                 : PC4   : out
CP Led3                 : PC5   : out
MB = main board         : ----------
Defrost sensor          : ADC6  : in
Freezer sensor          : ADC7  : in
Door open sw            : PD2   : in
Fan 12V                 : PB1   : out
Motor                   : PB0   : out
Defrost heater          : PD5   : out
```

Other things:

In the freezer compartment I am using 2x 12V fans taken from used ATX power supply units.
One blows in the freezer and another blows from the freezer to the main compartment (+5C).
They work quite well, the only thing to consider is, as soon as fans work at low temperature 
I had to use full synthetic low temperature oil (Nissan 5w30 motor oil in my case). Before that they worked well
up to -5 -10 Celsius, but at -20 ... -30 Celsius they became too noisy and I had to oil them.

TODO:
defrost timer.
It is not implemented yet.
Logic: once each 1-2 days the fridge must be defrozen.
1. turn the compressor off and 
2. turn the heater on for 15-20 (30) minutes.
Additionally: save the last successful heating time. Note: electricity may go off so I should keep settings in DS1302 RAM.
(For some reason ATmega8 eerprom gets corrupted in my case, looks like sporadic code executed when power goes down)
