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
```

### Examples for SensorBar16


* [Analog Read](https://github.com/MdelgadoL83/TapatioElectronics/tree/main/examples/SensorBar16_AnalogRead): control the shaft of a servo motor by turning a potentiometer
* [Digital Read](https://github.com/MdelgadoL83/TapatioElectronics/tree/main/examples/SensorBar16_DigitalRead): sweeps the shaft of a servo motor back and forth
