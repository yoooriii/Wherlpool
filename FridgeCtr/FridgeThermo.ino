//

#include "FridgeDefinitions.h"
#include "FridgeSupport.h"

#define ArrSize 8
#define MeasurePeriod 1000L
uint8_t tempArray[ArrSize];
uint8_t iCurrent = 0;
unsigned long lastMeasureTime = 0;

void measureTemp() {
    if (0 == lastMeasureTime) {
        lastMeasureTime = millis();
        return;
    }
    const unsigned long dt = millis() - lastMeasureTime;
    if (dt > MeasurePeriod) {
        lastMeasureTime = millis();
        doMeasureTemp();
    }
}

// accumulate temperature (fill array)
void doMeasureTemp() {
    uint8_t temp = 0xFF & (getTemp() >> 2);
    if (0 == temp) {
      // set min value
      temp = 1;
    }
    tempArray[iCurrent] = temp;
    ++iCurrent;
    if (iCurrent >= ArrSize) {
        iCurrent = 0;
    }
}

int getAvgTemp() {
    int temp = 0;
    uint8_t i = 0;
    for (i = 0; i < ArrSize; ++i) {
        const uint8_t val = tempArray[i];
        if (0 == val) {
            break;
        }
        temp += val;
    }
    temp = temp << 2;
    if (i) {
        temp /= i;
    }
    return temp;
}

void measureReset() {
    iCurrent = 0;
    lastMeasureTime = 0;
    for (uint8_t i = 0; i < ArrSize; ++i) {
        tempArray[i] = 0;
    }
}

void dumpTemp() {
  Serial.print("temp-dump:");
  uint8_t i;
  for (i = 0; i < ArrSize; ++i) {
    const uint8_t val = tempArray[i];
    if (0 == val) {
      break;
    }
    int xx = val;
    xx = xx << 2;
    Serial.print(val);
    Serial.print(", ");
  }
  Serial.print("> ");
  Serial.print(i);
  Serial.println(";");
}

#undef ArrSize
