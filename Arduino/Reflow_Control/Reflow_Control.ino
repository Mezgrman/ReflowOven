#include <PID_v1.h>

#define LEAD_FREE 1

#define TEMP_SENSOR   A0
#define HEATER_BOTTOM A4
#define HEATER_TOP    A5
#define LED           13
#define MODE_SW_1     2
#define MODE_SW_2     3

double setTemp = 0.0;
double curTemp = 0.0;
double onTime = 0.0;

PID pid(&curTemp, &onTime, &setTemp, 5.0, 0.1, 0.0, DIRECT);

int windowSize = 200;
unsigned long windowStartTime = 0;

enum MODES {
  MODE_FULL,
  MODE_LOW,
  MODE_MED,
  MODE_REFLOW
};

int getTemperature() {
  int value = 0;
  for (int n = 0; n < 10; n++) {
    value += analogRead(TEMP_SENSOR);
    delay(1);
  }
  value /= 10;
  return map(value, 685, 355, 0, 250);
}

void setHeater(bool state) {
  digitalWrite(HEATER_BOTTOM, state);
  digitalWrite(HEATER_TOP, state);
  digitalWrite(LED, state);
}

int getMode() {
  if (digitalRead(MODE_SW_1) && digitalRead(MODE_SW_2)) {
    return MODE_FULL;
  } else if (!digitalRead(MODE_SW_1) && digitalRead(MODE_SW_2)) {
    return MODE_MED;
  } else if (digitalRead(MODE_SW_1) && !digitalRead(MODE_SW_2)) {
    return MODE_LOW;
  } else if (!digitalRead(MODE_SW_1) && !digitalRead(MODE_SW_2)) {
    return MODE_REFLOW;
  }
}

#if LEAD_FREE
float getReflowTemp(unsigned long time) {
  if (time < 150000) {
    return 25.0  + 1.00 * (time -      0) / 1000;
  } else if (time >= 150000 && time < 330000) {
    return 175.0 + 0.14 * (time - 150000) / 1000;
  } else if (time >= 330000 && time < 390000) {
    return 200.0 + 1.00 * (time - 330000) / 1000;
  } else if (time >= 390000 && time < 430000) {
    return 260.0;
  } else if (time >= 450000) {
    return 0.0;
  }
}
#endif

#if !LEAD_FREE
float getReflowTemp(unsigned long time) {
  if (time < 150000) {
    return 25.0  + 1.00 * (time -      0) / 1000;
  } else if (time >= 150000 && time < 330000) {
    return 100.0 + 0.14 * (time - 150000) / 1000;
  } else if (time >= 330000 && time < 390000) {
    return 140.0 + 1.00 * (time - 330000) / 1000;
  } else if (time >= 390000 && time < 430000) {
    return 190.0;
  } else if (time >= 450000) {
    return 0.0;
  }
}
#endif

void setup() {
  // Set the ADC reference voltage to the internal 1.1V source for greater accuracy
  analogReference(INTERNAL);

  pinMode(TEMP_SENSOR, INPUT);
  pinMode(HEATER_BOTTOM, OUTPUT);
  pinMode(HEATER_TOP, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(MODE_SW_1, INPUT_PULLUP);
  pinMode(MODE_SW_2, INPUT_PULLUP);

  setHeater(false);

  windowStartTime = millis();
  pid.SetOutputLimits(0, windowSize);
  pid.SetMode(AUTOMATIC);

  Serial.begin(9600);
  delay(2500);
}

int previousMode = MODE_LOW;
unsigned long reflowStartTime = 0;
void loop() {
  unsigned long now = millis();
  int mode = getMode();
  if (mode != previousMode) {
    if (mode == MODE_REFLOW) {
      reflowStartTime = now;
    }
    previousMode = mode;
  }

  switch (mode) {
    case MODE_FULL:
      setTemp = 350.0;
      break;

    case MODE_LOW:
      setTemp = 40.0;
      break;

    case MODE_MED:
      setTemp = 80.0;
      break;

    case MODE_REFLOW:
      setTemp = getReflowTemp(now - reflowStartTime);
      break;
  }

  curTemp = getTemperature();
  pid.Compute();

  if (now - windowStartTime > windowSize) {
    windowStartTime += windowSize;
  }

  setHeater(onTime > now - windowStartTime);

  Serial.print(now);
  Serial.print("\t");
  Serial.print(setTemp);
  Serial.print("\t");
  Serial.print(curTemp);
  Serial.print("\t");
  Serial.println(onTime);
}
