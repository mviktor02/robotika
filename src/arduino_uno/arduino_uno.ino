#define A 10
#define B 11
#define C 12
#define D 13
#define PIN_CONFIG 0
#define PIN_STATE  1
#define LED_Num   12

#define STCP1 4
#define SHCP1 3
#define DS1   2
#define STCP2 7
#define SHCP2 6
#define DS2   5

#define HRS_POT_PIN   A0
#define MIN_POT_PIN   A1
#define MODE_BTN_PIN   8
#define ALARM_BTN_PIN  9
#define ALARM_PIN     A2

#define MODE_NORMAL       0
#define MODE_TIME_ADJUST  1
#define MODE_ALARM_ADJUST 2

int alarmTones[4] = { 500, 750, 1000, 750 };
int alarmToneIndex = 0;

int clockMatrix[LED_Num][2][4] = {
  //           PIN_CONFIG                  PIN_STATE
  //    A       B       C      D         A     B    C    D
  { { OUTPUT, OUTPUT, INPUT , INPUT  }, { HIGH, LOW , LOW , LOW  } }, 
  { { OUTPUT, OUTPUT, INPUT , INPUT  }, { LOW , HIGH, LOW , LOW  } }, 
  { { INPUT , OUTPUT, OUTPUT, INPUT  }, { LOW , HIGH, LOW , LOW  } },
  { { INPUT , OUTPUT, OUTPUT, INPUT  }, { LOW , LOW , HIGH, LOW  } }, 
  { { OUTPUT, INPUT , OUTPUT, INPUT  }, { HIGH, LOW , LOW , LOW  } },
  { { OUTPUT, INPUT , OUTPUT, INPUT  }, { LOW , LOW , HIGH, LOW  } }, 
  { { OUTPUT, INPUT , INPUT , OUTPUT }, { HIGH, LOW , LOW , LOW  } }, 
  { { OUTPUT, INPUT , INPUT , OUTPUT }, { LOW , LOW , LOW , HIGH } }, 
  { { INPUT , OUTPUT, INPUT , OUTPUT }, { LOW , HIGH, LOW , LOW  } }, 
  { { INPUT , OUTPUT, INPUT , OUTPUT }, { LOW , LOW , LOW , HIGH } }, 
  { { INPUT , INPUT , OUTPUT, OUTPUT }, { LOW , LOW , HIGH, LOW  } }, 
  { { INPUT , INPUT , OUTPUT, OUTPUT }, { LOW , LOW , LOW , HIGH } }  
};

//a,b,c,d,e,f,g,dp
byte segValue[10] = {
   0b00000010, //0
   0b10011110, //1
   0b00100100, //2
   0b00001100, //3
   0b10011000, //4
   0b01001000, //5
   0b01000000, //6
   0b00011110, //7
   0b00000000, //8
   0b00001000  //9  
};

int hours = 0;
int minutes = 0;
int seconds = 0;
int fakeHours = 0;
int fakeMinutes = 0;
int fakeHundredMillis = 0;

int alarmHours = -1;
int alarmMinutes = -1;
bool shouldSoundAlarm = true;
bool soundAlarm = false;
int alarmBtnPressed = 0;

bool readButtonInput = false;
int mode = 0;

void clockLightOn(int led) {
  pinMode(A, clockMatrix[led][PIN_CONFIG][0]);
  pinMode(B, clockMatrix[led][PIN_CONFIG][1]);
  pinMode(C, clockMatrix[led][PIN_CONFIG][2]);
  pinMode(D, clockMatrix[led][PIN_CONFIG][3]);
  digitalWrite(A, clockMatrix[led][PIN_STATE][0]);
  digitalWrite(B, clockMatrix[led][PIN_STATE][1]);
  digitalWrite(C, clockMatrix[led][PIN_STATE][2]);
  digitalWrite(D, clockMatrix[led][PIN_STATE][3]);
}
void displayNumOn7Segment(int num)
{
  int ones = num % 10;
  int tens = (num % 100) / 10;
  digitalWrite(STCP1, LOW);
  shiftOut(DS1, SHCP1, LSBFIRST, segValue[tens]);
  digitalWrite(STCP1, HIGH);
  digitalWrite(STCP2, LOW);
  shiftOut(DS2, SHCP2, LSBFIRST, segValue[ones]);
  digitalWrite(STCP2, HIGH);
}

void doSoundAlarm() {
  int numTones = sizeof(alarmTones)/sizeof(alarmTones[0]);
  alarmToneIndex += 1;
  if (alarmToneIndex == numTones) {
    alarmToneIndex = 0;
  }
  tone(ALARM_PIN, alarmTones[alarmToneIndex], 1000);
}

void trySoundAlarm() {
  if (alarmHours == -1) return;
  if (!shouldSoundAlarm && (alarmHours != hours || alarmMinutes != minutes)) {
    shouldSoundAlarm = true;
  }
  if (soundAlarm) {
    doSoundAlarm();
    if (digitalRead(ALARM_BTN_PIN)) {
      soundAlarm = false;
    }
  } else if (alarmHours == hours && alarmMinutes == minutes && shouldSoundAlarm) {
    soundAlarm = true;
    shouldSoundAlarm = false;
  }
}

void tryClearAlarm() {
  if (alarmBtnPressed == 5) {
    alarmBtnPressed = 0;
    alarmHours = -1;
    alarmMinutes = -1;
    return;
  }
  if (digitalRead(ALARM_BTN_PIN)) {
    alarmBtnPressed += 1;
  } else {
    alarmBtnPressed = 0;
  }
}

void advanceTime() {
  seconds += 1;
  if (seconds == 60) {
    seconds = 0;
    minutes += 1;
    if (minutes == 60) {
      minutes = 0;
      hours += 1;
      if (hours == 12) {
        hours = 0;
      }
    }
  }
}

void advanceFakeTime() {
  fakeHundredMillis += 1;
  if (fakeHundredMillis == 600) {
    fakeHundredMillis = 0;
    fakeMinutes += 1;
    if (fakeMinutes == 60) {
      fakeMinutes = 0;
      fakeHours += 1;
      if (fakeHours == 12) {
        fakeHours = 0;
      }
    }
  }
}

void updateMode() {
  if (mode < 2) {
    mode += 1;
    if (mode == MODE_ALARM_ADJUST) {
      fakeHundredMillis = seconds * 10;
      fakeMinutes = minutes;
      fakeHours = hours;
    }
  } else {
    mode = 0;
    // last mode is setting alarm, so fix time to actual and set alarm
    alarmHours = hours;
    alarmMinutes = minutes;
    hours = fakeHours;
    minutes = fakeMinutes;
    seconds = fakeHundredMillis / 10;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(STCP1, OUTPUT);
  pinMode(SHCP1, OUTPUT);
  pinMode(DS1  , OUTPUT);
  pinMode(STCP2, OUTPUT);
  pinMode(SHCP2, OUTPUT);
  pinMode(DS2  , OUTPUT);
  pinMode(HRS_POT_PIN, INPUT);
  pinMode(MIN_POT_PIN, INPUT);
  pinMode(ALARM_BTN_PIN, INPUT);
  pinMode(MODE_BTN_PIN, INPUT);
  pinMode(ALARM_PIN, OUTPUT);
}

void loop() {
  clockLightOn(hours);
  displayNumOn7Segment(minutes);
  bool shouldHandleBtn = !readButtonInput && digitalRead(MODE_BTN_PIN);
  if (shouldHandleBtn) {
    readButtonInput = true;
    updateMode();
  } else if (!digitalRead(MODE_BTN_PIN)) {
    readButtonInput = false;
  }
  if (mode == MODE_TIME_ADJUST) {
    hours   = map(analogRead(HRS_POT_PIN), 0, 1023, 0, 11);
    minutes = map(analogRead(MIN_POT_PIN), 0, 1023, 0, 59);
  	seconds = 0;
    delay(100);
  }
  else if (mode == MODE_ALARM_ADJUST) {
    hours   = map(analogRead(HRS_POT_PIN), 0, 1023, 0, 11);
    minutes = map(analogRead(MIN_POT_PIN), 0, 1023, 0, 59);
    advanceFakeTime();
    delay(100);
  }
  else {
    advanceTime();
    tryClearAlarm();
    trySoundAlarm();
    delay(1000);
  }
}