# Wherlpool
Whirlpool refrigerator Arduino ATmega8 circuit board and code project.

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
...
