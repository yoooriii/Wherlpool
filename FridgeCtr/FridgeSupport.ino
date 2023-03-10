#include <stdint.h>
#include <Arduino.h>
#include "FridgeSupport.h"

const unsigned long motor_delay_value = 60000L;
unsigned long motor_delay = 0;

void turnMotor(uint8_t on) {
    if (!on) {
      if (!isMotorOn()) {
        // the motor is already off, skip the rest
        return;
      }

      motor_delay = 0;
      digitalWrite(pp_Motor, LOW);

      if (1) {
        Serial.print("Motor off, temp:");
        Serial.print(getAvgTemp());
        Serial.println(";");
      }
      return;
    }

    if (isMotorOn()) {
      // the motor is already on, skip the rest
      return;
    }

    const unsigned long dt = (0L == motor_delay) ? 0L : millis() - motor_delay;

    if (dt > motor_delay_value) {
      // turn motor on and stop delay timer
      digitalWrite(pp_Motor, HIGH);
      motor_delay = 0;

      if (1) {
        Serial.print("Motor on, temp:");
        Serial.print(getAvgTemp());
        Serial.println(";");
      }
      return;
    }

    if (0 == motor_delay) {
      // start motor delay timer
      motor_delay = millis();
    }
}

uint8_t isMotorOn() {
  return digitalRead(pp_Motor);
}


const unsigned long fan_delay_value = 1000L;
unsigned long fan_delay = 0;

void turnFan(uint8_t on) {
    if (!on) {
      // stop fan delay timer
      fan_delay = 0;
      digitalWrite(pp_Fan, LOW);
      return;
    }

    if (isFanOn()) {
      // the fan is already on, skip the rest
      return;
    }

    const unsigned long dt = (0L == fan_delay) ? 0L : millis() - fan_delay;

    if (dt > fan_delay_value) {
      // turn fan on and stop delay timer
      digitalWrite(pp_Fan, HIGH);
      fan_delay = 0;
      return;
    }

    if (0 == fan_delay) {
      // start fan delay timer
      fan_delay = millis();
    }
}

uint8_t isFanOn() {
  return digitalRead(pp_Fan);
}


void turnHeater(uint8_t on) {
    digitalWrite(pp_Heater, on);
}

uint8_t isDoorOpen() {
  return !digitalRead(pp_Door);
}

int getTemp() {
  return analogRead(pp_Thermo);
}

int getTempDf() {
  return analogRead(pp_TDF);
}

uint8_t isResetButtonOn() {
  return !digitalRead(pp_Reset);
}

void turnTempLed(uint8_t state) {
  switch(state) {
    default:
    case 0:
      digitalWrite(pp_Led1, LOW);
      digitalWrite(pp_Led2, LOW);
      digitalWrite(pp_Led3, LOW);
      break;
    case 1:
      digitalWrite(pp_Led1, HIGH);
      digitalWrite(pp_Led2, LOW);
      digitalWrite(pp_Led3, LOW);
      break;
    case 2:
      digitalWrite(pp_Led1, LOW);
      digitalWrite(pp_Led2, HIGH);
      digitalWrite(pp_Led3, LOW);
      break;
    case 3:
      digitalWrite(pp_Led1, LOW);
      digitalWrite(pp_Led2, LOW);
      digitalWrite(pp_Led3, HIGH);
      break;
    case 4:
    case 5:
      digitalWrite(pp_Led1, HIGH);
      digitalWrite(pp_Led2, HIGH);
      digitalWrite(pp_Led3, HIGH);
      break;
  }
}
