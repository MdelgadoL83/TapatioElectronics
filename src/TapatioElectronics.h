#include <Adafruit_SSD1306.h>

class SensorBar16 {
private:
  int numSensors = 16;
  int umbral[16];
  int data[16];
  int s0, s1, s2, s3;
  int out;
  int read(int x);
  int lastPosition;

public:
  SensorBar16(int s0, int s1, int s2, int s3, int out);
  int getPosition();
  int getPosition(int range);
  void calibrate(int times = 800);
  int *analogValues();
  int *digitalValues();
  int singleSensorRead(int sensor);
  int *getUmbral();
  int *getValues();
};

class DriverTB6612 {
private:
  int pwma, ain1, ain2;
  int pwmb, bin1, bin2;

public:
  DriverTB6612(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2);
  void setVelocidad(int motIzq, int motDer);
  void stop();
};

class DriverTapatio {
private:
  int pwma1, pwma2;
  int pwmb1, pwmb2;

public:
  DriverTapatio(int pwma1, int pwma2, int pwmb1, int pwmb2);
  void setVelocidad(int velMotorA, int velMotorB);
  void stop();
};

void SplashScreen(Adafruit_SSD1306 &display);