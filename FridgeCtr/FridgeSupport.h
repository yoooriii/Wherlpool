//
// Wherlpool

#include "FridgeDefinitions.h"

#ifndef _FRIDGE_SUPPORT_
#define _FRIDGE_SUPPORT_

// turn motor on/off: off - immediately, on - with a delay
void turnMotor(uint8_t on);
// returns: 1 - if motor is on; 0 - if motor is off;
uint8_t isMotorOn();
// turn fan on/off: off - immediately, on - with a delay
void turnFan(uint8_t on);
// is fan on? returns: 1 - if fan is on; 0 - if fan is off;
uint8_t isFanOn();
// turn heater on/off: off - immediately, on - with a delay, it turns off after the HeatTime delay
void turnHeater(uint8_t on);
// is door open? 0 - closed; 1 - open;
uint8_t isDoorOpen();
// get temperature (freezer)
int getTemp();
// get temperature (defrost)
int getTempDf();

//
uint8_t isResetButtonOn();

// turn led, state: 0 - off, [1...4] - turn on N mode
void turnTempLed(uint8_t state);


#endif
