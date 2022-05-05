# TapatioElectronics library


This library contains all the necessary elements to make all the sensor boards designed and developed by Tejuino Electronics work, the operation of each of them is shown below.

Remember to use this library:

```
#include <TapatioElecronics.h>
```

## SensorBar16

This card has 16 infrared sensors dedicated to distinguishing a line with high contrast (black/white), usually used in line following robots.
╔════════════════════════════════════════════════════════════════════════════════════════════╗
║°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°TOP VIEW°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°║
║°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°███°°███°°███°°███°°███°°███°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°║
°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°███°°°°°5°°°°6°°°°7°°°°8°°°°9°°°°10°°°°°███°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°║
║                          ███     4                                                11    ███                           ║
║                   ███     3                                                              12    ███                    ║
║            ███     2                                                                            13    ███             ║
║     ███     1                              S2 ───────┐       ┌─────── VCC                              14    ███      ║
║      0                                     S3 ─────┐ │       │ ┌───── GND                                     15      ║
╚═══════════════════════════════════════════════════ █ █ █ █ █ █ █ ═════════════════════════════════════════════════════╝
S1 ─────────┘  │ └───────── OUT
S0

### Examples for SensorBar16

* [Knob](https://www.arduino.cc/en/Tutorial/Knob): control the shaft of a servo motor by turning a potentiometer
* [Sweep](https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep): sweeps the shaft of a servo motor back and forth
