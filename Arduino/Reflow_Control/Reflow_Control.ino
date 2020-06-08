#include <PID_v1.h>

#define LOW_TEMP

#ifdef LEAD_FREE
#define TIME_0 0
#define TEMP_0 25

#define TIME_1 150
#define TEMP_1 175

#define TIME_2 330
#define TEMP_2 200

#define TIME_3 390
#define TEMP_3 260

#define TIME_4 430
#define TEMP_4 260

#define TIME_5 431
#define TEMP_5 0
#endif

#ifdef LEADED
#define TIME_0 0
#define TEMP_0 25

#define TIME_1 150
#define TEMP_1 100

#define TIME_2 330
#define TEMP_2 140

#define TIME_3 390
#define TEMP_3 190

#define TIME_4 430
#define TEMP_4 190

#define TIME_5 431
#define TEMP_5 0
#endif

#ifdef LOW_TEMP
#define TIME_0 0
#define TEMP_0 25

#define TIME_1 120
#define TEMP_1 100

#define TIME_2 270
#define TEMP_2 140

#define TIME_3 300
#define TEMP_3 170

#define TIME_4 330
#define TEMP_4 170

#define TIME_5 331
#define TEMP_5 0
#endif

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

double getReflowTemp(unsigned long time) {
  time /= 1000;
  if (time <= TIME_0) {
    return TEMP_0;
  }

  if (time <= TIME_1) {
    double m = (double)(TEMP_1 - TEMP_0) / (double)(TIME_1 - TIME_0);
    double b = (double)TEMP_1 - m * TIME_1;
    return m * time + b;
  }

  if (time <= TIME_2) {
    double m = (double)(TEMP_2 - TEMP_1) / (double)(TIME_2 - TIME_1);
    double b = (double)TEMP_2 - m * TIME_2;
    return m * time + b;
  }

  if (time <= TIME_3) {
    double m = (double)(TEMP_3 - TEMP_2) / (double)(TIME_3 - TIME_2);
    double b = (double)TEMP_3 - m * TIME_3;
    return m * time + b;
  }

  if (time <= TIME_4) {
    double m = (double)(TEMP_4 - TEMP_3) / (double)(TIME_4 - TIME_3);
    double b = (double)TEMP_4 - m * TIME_4;
    return m * time + b;
  }

  if (time <= TIME_5) {
    double m = (double)(TEMP_5 - TEMP_4) / (double)(TIME_5 - TIME_4);
    double b = (double)TEMP_5 - m * TIME_5;
    return m * time + b;
  }

  if (time > TIME_5) {
    return TEMP_5;
  }
}

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
