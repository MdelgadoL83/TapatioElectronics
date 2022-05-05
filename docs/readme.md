# TapatioElectronics library


This library contains all the necessary elements to make all the sensor boards designed and developed by Tejuino Electronics work, the operation of each of them is shown below.

Remember to use this library:

```
#include <TapatioElecronics.h>
```

## SensorBar16

This card has 16 infrared sensors dedicated to distinguishing a line with high contrast (black/white), usually used in line following robots.

### How use SensorBar16
1.- Import the library in your arduino project
```
#include <TapatioElecronics.h>
```
2.- Create an object to use
```
SensorBar16 sb(2,3,4,5,A0);
```
3.- Call the methods you need
```
sb.calibrate();
int pos = sb.getPosition();
...
```

### summary of constructors ands methods for SensorBar16
CONSTRUCTOR
  SensorBar16(int s0,int s1,int s2, int s3, int out);

METHODS
  int getPosition();
  int getPosition(int range);
  void calibrate(int times=800);
  int* analogValues();
  int* digitalValues();
  int singleSensorRead(int sensor);
  int* getUmbral();
  int* getValues();


### Examples for SensorBar16

* [Analog Read](https://github.com/MdelgadoL83/TapatioElectronics/blob/main/examples/SensorBar16_AnalogRead/SensorBar16_AnalogRead.ino): Example showing the use of the sensor bar in an analog mode
* [Digital Read](https://github.com/MdelgadoL83/TapatioElectronics/blob/main/examples/SensorBar16_DigitalRead/SensorBar16_DigitalRead.ino): Example showing the use of the sensor bar in an digital mode
* [Single Sensor](https://github.com/MdelgadoL83/TapatioElectronics/blob/main/examples/SensorBar16_SingleVaue/SensorBar16_SingleVaue.ino): shows how to access each sensor separately
* [get Position](https://github.com/MdelgadoL83/TapatioElectronics/blob/main/examples/SensorBar16_getPosition/SensorBar16_getPosition.ino): Example showing the operation for a line following robot
