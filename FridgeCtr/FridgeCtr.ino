/*

*/

#include <EEPROM.h>
#include <DS1302.h>
#include "FridgeSupport.h"


void printMetrics();
void loopMotorControl();


// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
const int kCePin   = PIN_PC2;  // Chip Enable
const int kIoPin   = PIN_PC1;  // Input/Output
const int kSclkPin = PIN_PC0;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

void printTime() {
  // Get the current time and date from the chip.
  TimeD2 t;
  rtc.readTime(&t);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
}


void setTemperatureMode(uint8_t tempMode);
void readTemperatureMode();

uint8_t lastButtonState = HIGH;  // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
uint8_t temperatureMode = 0;

const int getTemperatureOff() {
  switch (temperatureMode)
  {
  case 0:
    return 400; // -9-10C
  case 1:
    return 460; // -13C
  case 2:
    return 530; // -17C
  case 3:
    return 575; // -20C
  default:
    return 1024; // max
  }
}

void switchTemperature() {
  ++temperatureMode;
  if (temperatureMode > 3) {
    temperatureMode = 1;
  }
  setTemperatureMode(temperatureMode);
}

const uint8_t TempModeAddr = 0; 

void setTemperatureMode(uint8_t tempMode) {
  rtc.writeRam(TempModeAddr, tempMode);
  turnTempLed(tempMode);
}

void readTemperatureMode() {
  const uint8_t tempMode = rtc.readRam(TempModeAddr);
  Serial.print("Temp mode read:");
  Serial.println(tempMode);

  if (tempMode > 5) {
    temperatureMode = 0;
    rtc.writeRam(TempModeAddr, 0);
  } else {
    temperatureMode = tempMode;
  }
  turnTempLed(temperatureMode);
}

void readButtons() {
  // read the state of the switch into a local variable:
  const uint8_t reading = digitalRead(pp_UpDn);
  if (reading == lastButtonState) {
    const unsigned long dt = (0L == lastDebounceTime) ? 0L : millis() - lastDebounceTime;
    if (dt > debounceDelay) {
      lastDebounceTime = 0; // stop debouncing timer
      if (reading == HIGH) {
        // button released, switch temperature      
        switchTemperature();
      }
    }

    return;
  }
  
  // the button state has changed --> restart the debouncing timer
  lastDebounceTime = millis();
  lastButtonState = reading;
}

void setupRTC() {
    // Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);

  TimeD2 t;
  rtc.readTime(&t);
  if (t.yr < 5) {
    t.yr = 6;
    t.mon = 1;
    t.date = 1;
    t.hr = 0;
    t.min = 0;
    t.sec = 0;
    // Set the time and date on the chip.
    rtc.writeTime(&t);
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(pp_Fan, OUTPUT);
  pinMode(pp_Motor, OUTPUT);
  pinMode(pp_Heater, OUTPUT);
  pinMode(pp_Led1, OUTPUT);
  pinMode(pp_Led2, OUTPUT);
  pinMode(pp_Led3, OUTPUT);

  pinMode(pp_TDF, INPUT);
  pinMode(pp_Thermo, INPUT);
  pinMode(pp_Door, INPUT);
  pinMode(pp_Reset, INPUT);
  pinMode(pp_UpDn, INPUT);

  measureReset();

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  setupRTC();
  readTemperatureMode();

  Serial.println("setup-end");
  // dumpTemp();
}

uint8_t skipLoop = 0;

// the loop function runs over and over again forever
void loop() {
  readButtons();

  const uint8_t door_open = isDoorOpen();
  // const uint8_t isReset = isResetButtonOn();

  turnFan(!door_open);

  /// if not heating then motor control
  loopMotorControl();


  ++skipLoop;
  if (skipLoop > 20) {
    skipLoop = 0;
    // dumpTemp();
    printMetrics();
  }
  
  delay(100);
}

void loopMotorControl() {
  measureTemp();
  const int avgTemp = getAvgTemp();
  const int tempOff = getTemperatureOff();
  if (0) {
    Serial.print("MCtr, avg:");
    Serial.print(avgTemp);
    Serial.print("; toff:");
    Serial.print(tempOff);
    Serial.print(";");
  }

  if (isMotorOn()) {
    // motor is running, check if the temperature is higher than requested
    if (avgTemp > tempOff) {
      turnMotor(LOW);
    }
  } else {
    // motor is not running, check if the temperature is back minus histeresis
    if (avgTemp < (tempOff - 20)) {
      turnMotor(HIGH);
    }
  }
}

void printMetrics() {
  Serial.print("Temp.mode:");
  Serial.print(temperatureMode);
  Serial.print("; Temp.off:");
  Serial.print(getTemperatureOff());
  Serial.println(";");

  // read the temp sensor value
  int sensorValue1 = getAvgTemp();
  Serial.print("Temp.avg.Fr:");
  Serial.print(sensorValue1);
  Serial.print(";  ");
  
// read the defrost sensor value
  int sensorValue2 = getTempDf();
  Serial.print("Temp.Df:");
  Serial.print(sensorValue2);
  Serial.print(";  ");

  Serial.print("Motor:");
  Serial.print(isMotorOn());
  Serial.println(";");

  printTime();
}
