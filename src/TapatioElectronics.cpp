#include "TapatioElectronics.h"
#include "Arduino.h"

/*************************************************************************************************
****************************************** BARRA DE SENSORES **************************************
**************************************************************************************************/
/*
-------------------------------------------------------------------------------------------------
-------------------------------------------CONSTRUCTOR-------------------------------------------
-------------------------------------------------------------------------------------------------
*/
SensorBar16::SensorBar16(int s0, int s1, int s2, int s3, int out) {
  this->s0 = s0;
  this->s1 = s1;
  this->s2 = s2;
  this->s3 = s3;
  this->out = out;
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
}
/*
-------------------------------------------------------------------------------------------------
-------------------------------------------PRIVATES----------------------------------------------
-------------------------------------------------------------------------------------------------
*/
int SensorBar16::read(int x) {
  x = 15 - x;
  digitalWrite(s0, bitRead(x, 0));
  digitalWrite(s1, bitRead(x, 1));
  digitalWrite(s2, bitRead(x, 2));
  digitalWrite(s3, bitRead(x, 3));
  return analogRead(out);
}
/*
-------------------------------------------------------------------------------------------------
-------------------------------------------PUBLICS-----------------------------------------------
-------------------------------------------------------------------------------------------------
*/
int SensorBar16::singleSensorRead(int sensor) {
  if (sensor >= 0 && sensor < numSensors) {
    return read(sensor);
  } else {
    return 0;
  }
}

int SensorBar16::getPosition(int range) {
  int p = getPosition();
  return map(p, 100, 1600, range * -1, range);
}

int SensorBar16::getPosition() {
  int suma = 0;
  int contador = 0;
  for (int x = 0; x < numSensors; x++) {
    data[x] = read(x);
    if (data[x] > umbral[x]) {
      suma += (x + 1) * 100;
      contador++;
    }
  }
  lastPosition = contador > 0 ? suma / contador : lastPosition;
  return lastPosition;
}

int *SensorBar16::analogValues() {
  for (int x = 0; x < numSensors; x++) {
    data[x] = read(x);
  }
  return data;
}

int *SensorBar16::digitalValues() {
  for (int x = 0; x < numSensors; x++) {
    if (read(x) > umbral[x]) {
      data[x] = (x + 1) * 100;
    } else {
      data[x] = 0;
    }
  }
  return data;
}

int *SensorBar16::getUmbral() { return umbral; }

int *SensorBar16::getValues() { return data; }

void SensorBar16::calibrate(int times) {
  int min[] = {5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
               5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};
  int max[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int y = 0; y < times; y++) {
    for (int x = 0; x < numSensors; x++) {
      int v = read(x);
      if (min[x] > v) {
        min[x] = v;
      }
      if (max[x] < v) {
        max[x] = v;
      }
    }
    delay(8);
  }
  for (int x = 0; x < numSensors; x++) {
    umbral[x] = ((min[x] + max[x]) / 2);
  }
}

/*************************************************************************************************
****************************************** DRIVER TB6612 ******************************************
**************************************************************************************************/
/*
-------------------------------------------------------------------------------------------------
-------------------------------------------CONSTRUCTOR-------------------------------------------
-------------------------------------------------------------------------------------------------
*/
DriverTB6612::DriverTB6612(int pwma, int ain1, int ain2, int pwmb, int bin1,
                           int bin2) {
  this->pwma = pwma;
  this->ain1 = ain1;
  this->ain2 = ain2;
  this->pwmb = pwmb;
  this->bin1 = bin1;
  this->bin2 = bin2;
  pinMode(pwma, OUTPUT);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
}
/**
-------------------------------------------------------------------------------------------------
-------------------------------------------PUBLICS-----------------------------------------------
-------------------------------------------------------------------------------------------------
*/
void DriverTB6612::setVelocidad(int motIzq, int motDer) {
  // Limitar velocidad al rango -255 a 255
  if (motIzq > 255)
    motIzq = 255;
  else if (motIzq < -255)
    motIzq = -255;

  if (motDer > 255)
    motDer = 255;
  else if (motDer < -255)
    motDer = -255;

  // Motor izquierdo
  if (motIzq >= 0) {
    digitalWrite(ain2, HIGH);
    digitalWrite(ain1, LOW);
    analogWrite(pwma, motIzq);
  } else {
    digitalWrite(ain2, LOW);
    digitalWrite(ain1, HIGH);
    analogWrite(pwma, -motIzq);
  }

  // Motor derecho
  if (motDer >= 0) {
    digitalWrite(bin2, HIGH);
    digitalWrite(bin1, LOW);
    analogWrite(pwmb, motDer);
  } else {
    digitalWrite(bin2, LOW);
    digitalWrite(bin1, HIGH);
    analogWrite(pwmb, -motDer);
  }
}

/*************************************************************************************************
****************************************** DRIVER TAPATIO ******************************************
**************************************************************************************************/
/*
-------------------------------------------------------------------------------------------------
-------------------------------------------CONSTRUCTOR-------------------------------------------
-------------------------------------------------------------------------------------------------
*/
DriverTapatio::DriverTapatio(int pwma1, int pwma2, int pwmb1, int pwmb2) {
  this->pwma1 = pwma1;
  this->pwma2 = pwma2;
  this->pwmb1 = pwmb1;
  this->pwmb2 = pwmb2;
  pinMode(pwma1, OUTPUT);
  pinMode(pwma2, OUTPUT);
  pinMode(pwmb1, OUTPUT);
  pinMode(pwmb2, OUTPUT);
}
/*
-------------------------------------------------------------------------------------------------
-------------------------------------------PUBLICS-----------------------------------------------
-------------------------------------------------------------------------------------------------
*/
void DriverTapatio::setVelocidad(int motIzq, int motDer) {
  if (motIzq > 0) {
    analogWrite(pwma1, motIzq);
    analogWrite(pwma2, 0);
  } else {
    analogWrite(pwma1, 5);
    analogWrite(pwma2, -motIzq);
  }
  if (motDer > 0) {
    analogWrite(pwmb1, motDer);
    analogWrite(pwmb2, 0);
  } else {
    analogWrite(pwmb1, 5);
    analogWrite(pwmb2, -motDer);
  }
}

/*************************************************************************************************
****************************************** SCREEN LOGO *******************************************
**************************************************************************************************/
// 'Logo TAPATIO'
const unsigned char tapatio_logo_128x32[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1e, 0x7f, 0x80, 0x7f,
    0x9f, 0x00, 0x0c, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
    0x1e, 0x3f, 0x80, 0x1f, 0x0f, 0x00, 0x08, 0xf0, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0x9f, 0x0f, 0x0f, 0xff, 0xf8, 0xe1,
    0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xfc, 0x1f, 0x9f, 0xce,
    0x07, 0xfb, 0xf8, 0xc7, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0xf8, 0x9f, 0x9f, 0xce, 0x67, 0xf1, 0xf8, 0xcf, 0xfc, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf3, 0xf9, 0x8f, 0x9f, 0xce, 0x63, 0xf1, 0xf8, 0xcf,
    0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xf1, 0xcf, 0xff, 0x8c,
    0x73, 0xf1, 0xf8, 0x8f, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0xf3, 0xc7, 0xff, 0x0c, 0xf3, 0xf1, 0xf8, 0x8f, 0xfc, 0x7f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf3, 0xf3, 0xe7, 0x80, 0x18, 0xf1, 0xf1, 0xf8, 0x8f,
    0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe3, 0xff, 0x80, 0xf9,
    0xff, 0xf1, 0xf8, 0x8f, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0xe0, 0x03, 0x9f, 0xf8, 0x00, 0xf1, 0xf8, 0xcf, 0xfc, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf3, 0xc0, 0x03, 0x9f, 0xf0, 0x00, 0xf1, 0xf8, 0xc7,
    0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xcf, 0xf1, 0x9f, 0xf3,
    0xfc, 0x71, 0xf8, 0xe3, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0x8f, 0xf9, 0x9f, 0xe3, 0xfc, 0x71, 0xf8, 0xe0, 0xc1, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf3, 0x8f, 0xf8, 0x9f, 0xe7, 0xfe, 0x71, 0xf8, 0xf8,
    0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x85, 0xe0, 0xc4, 0x00, 0x61, 0x9f, 0x63, 0x8f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xbd, 0xef, 0xbb, 0xb7, 0x6c, 0x9f, 0x5d, 0x37,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbd, 0xef, 0x3f, 0xb7,
    0x5e, 0x8f, 0x5f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x85, 0xe0, 0x7f, 0xb0, 0x5e, 0xaf, 0x5f, 0xcf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xbd, 0xef, 0x3b, 0xb7, 0x5e, 0xb3, 0x5c, 0x77,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbd, 0xef, 0xbb, 0xb7,
    0x6c, 0xb3, 0x5d, 0x37, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x84, 0x00, 0xc7, 0xb7, 0x61, 0xbb, 0x63, 0x87, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void SplashScreen(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.drawBitmap(0, 0, tapatio_logo_128x32, 128, 32, SSD1306_WHITE);
  display.display();
}
