// FridgeDefinitions.h
// Wherlpool

#include <pins_arduino.h>

#ifndef _FRIDGE_DEFINITIONS_
#define _FRIDGE_DEFINITIONS_

#define pp_Fan (PIN_PB1)
#define pp_Motor (PIN_PB0)
#define pp_Heater (PIN_PD5)

// defrost sensor, analog input
#define pp_TDF (PIN_A6)
// thermo sensor, analog input
#define pp_Thermo (PIN_A7)
#define pp_Door (PIN_PD2)

// reset button
#define pp_Reset (PIN_PD3)
// +/- (up/down) button
#define pp_UpDn (PIN_PD4)
// 3 temperature mode leds
#define pp_Led1 (PIN_PC3)
#define pp_Led2 (PIN_PC4)
#define pp_Led3 (PIN_PC5)

// heat time in minutes
#define HeatTime (15)

#endif
