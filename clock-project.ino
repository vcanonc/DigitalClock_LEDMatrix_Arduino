#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

#include <Wire.h>
#include <RTClib.h>

#include "Font.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 4
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10

MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);  // HARDWARE SPI

RTC_DS3231 rtc;  // Objeto para el control del módulo de reloj

const uint8_t pinBuzzer = 4;

const uint8_t pinLightSensor = 8;

bool nightMode = false;  // Bandera para el manejo de la disminución de intensidad en caso de oscuridad

bool toggleColon = false;  // Para hacer el reloj parpadeante (por cada segundo)
uint8_t lastSecond = 255;
uint8_t lastMinute = 61;

// Banderas para mostrar solo una vez los valores por cada minuto
bool shownDate = true;
// bool shownTemperature = true;
bool showingHour = true;

const char* daysOfWeekES[] = {
  "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"
};

const char* monthsES[] = {
  "ENE", "FEB", "MAR", "ABR", "MAY", "JUN",
  "JUL", "AGO", "SEP", "OCT", "NOV", "DIC"
};

void setup() {
  Serial.begin(57600);

  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLightSensor, INPUT);

  display.begin();
  display.displayClear();
  display.setFont(myFont);


  if (!rtc.begin()) {
    Serial.println(F("Modulo RTC no encontrado!"));  // muestra mensaje de error
    display.displayText("RTC NF", PA_CENTER, 0, 1000, PA_PRINT, PA_NO_EFFECT);
    while (1) {
      while (!display.displayAnimate())
        ;
    }
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime dateTimeNow = rtc.now();

  updateLightIntensity();
  soundAlarm(dateTimeNow);

  // Restablecimiento de las banderas para mostrar fecha y temperatura
  if (dateTimeNow.minute() != lastMinute) {
    lastMinute = dateTimeNow.minute();
    shownDate = !shownDate;
    // shownTemperature = !shownTemperature;
  }

  // Evita repetir operaciones si no ha pasado un segundo
  if (dateTimeNow.second() == lastSecond) {
    return;
  }

  lastSecond = dateTimeNow.second();

  display.displayClear();

  /*
  Si ya se mostró alguna información, entonces, no se
  vuelve a mostrar de nuevo hasta el siguiente minuto
  */
  if (!shownDate && lastSecond >= 25) {
    showDate(dateTimeNow);
    shownDate = !shownDate;
    showingHour = false;
    // } else if (!shownTemperature && lastSecond >= 40) {
    //   showTemperature();
    //   shownTemperature = !shownTemperature;
    //   showingHour = false;
  } else {
    showHour(dateTimeNow);
    showingHour = true;
  }
}

void showHour(DateTime dateTime) {
  char buffer[6];  // 5 chars + null terminator
  toggleColon = !toggleColon;

  // Implementa la bandera toggleColon para mostrar o no los dos puntos por cada segundo
  sprintf(buffer, "%02d%c%02d", dateTime.hour(), toggleColon ? ':' : ' ', dateTime.minute());

  /*
  Si se está volviendo a mostrar la hora luego de mostrar 
  otra cosa, entonces tendrá una animación de entrada diferente
  */
  if (!showingHour) {
    display.displayText(buffer, PA_CENTER, 30, 970, PA_GROW_UP, PA_NO_EFFECT);
  } else {
    display.displayText(buffer, PA_CENTER, 0, 1000, PA_PRINT, PA_NO_EFFECT);
  }
  while (!display.displayAnimate())
    ;
}

void showDate(DateTime dateTime) {
  char date[40];

  sprintf(
    date,
    "%s, %02d %s %04d",
    daysOfWeekES[dateTime.dayOfTheWeek()],
    dateTime.day(),
    monthsES[dateTime.month() - 1],
    dateTime.year());
  display.displayText(date, PA_CENTER, 50, 500, PA_SCROLL_LEFT, PA_BLINDS);
  while (!display.displayAnimate())
    ;
}

// void showTemperature() {
//   float temperature = rtc.getTemperature();
//   char tempStr[20];
//   char numStr[10];

//   dtostrf(temperature, 4, 1, numStr);  // dtostrf convierte float a char[]
//   sprintf(tempStr, "%s", numStr);
//   strcat(tempStr, "\xB0");  // 176 en hexadecimal (símbolo de grados)
//   strcat(tempStr, "C");
//   display.displayText(tempStr, PA_CENTER, 60, 3000, PA_OPENING_CURSOR, PA_OPENING_CURSOR);
//   while (!display.displayAnimate())
//     ;
// }

void soundAlarm(DateTime now) {
  if ((now.hour() == 6 || now.hour() == 12 || now.hour() == 19) && now.minute() == 0 && now.second() <= 20) {
    bipBip();
  }
}

void bipBip() {
  digitalWrite(pinBuzzer, HIGH);
  delay(90);
  digitalWrite(pinBuzzer, LOW);
  delay(90);
  digitalWrite(pinBuzzer, HIGH);
  delay(90);
  digitalWrite(pinBuzzer, LOW);
}

void updateLightIntensity() {
  // Lee el estado del comparador
  bool isDark = digitalRead(pinLightSensor) == HIGH;

  if (isDark != nightMode) {
    nightMode = isDark;
    display.setIntensity(nightMode ? 1 : 8);
  }
}
