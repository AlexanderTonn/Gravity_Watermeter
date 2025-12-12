# Table of Contents
[Introduction](#introduction)
* [Use Case](#use-case)

[Example](#example)
* [Wokwi Scheme](#wokwi-scheme)
* [diagram.json](#diagramjson)
* [code](#code)

[Functions](#functions)
* [begin()](#begin)
* [Available Pins on MEGA2560 R3](#available-pins-on-mega2560-r3)
* [run()](#run)
* [getWaterflow1()...getWaterflow4()](#getwaterflow1--getwaterflow4)
* [setComputeInterval()](#setComputeInterval)

# Introduction
This class was developed for the Gravity Waterflow Sensor but without using the external interrupts by using the `attachInterrupt` function as it's described in the offical example. 
The disadvantage is the limited amount of available pins which you can use. With my implementation it's possible to configure any internal interrupt pins (if it's connected to an accessable pin). 

## Use Case
* the available pins for external interrupts are already used in your control task
* you need more than the available external interrupts

# Example
The example is using the Arduino MEGA2560 R3 and simple buttons for creating the "fake impulses". If you push the buttons fast enough you can see the calculated water consumption per hour.

>[!Note]
> Please consider that the MEGA2560 R3 is the only board that is supported at this point. If you need more please open a issue

## Wokwi Scheme
<img width="1034" height="540" alt="image" src="https://github.com/user-attachments/assets/693e7228-f5ba-4b2f-98ea-a6659d5ddf4a" />

###  diagram.json
```json
{
  "version": 1,
  "author": "Alexander Tonn",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-arduino-mega", "id": "mega", "top": -47.4, "left": 6, "attrs": {} },
    {
      "type": "wokwi-pushbutton",
      "id": "btn1",
      "top": -80.2,
      "left": 518.4,
      "attrs": { "color": "green", "xray": "1", "bounce": "0", "label": "Pseudo Sensor1" }
    },
    {
      "type": "wokwi-pushbutton",
      "id": "btn2",
      "top": -3.4,
      "left": 518.4,
      "attrs": { "color": "green", "xray": "1", "bounce": "0","label": "Pseudo Sensor2" }
    },
    { "type": "wokwi-junction", "id": "j1", "top": 4.8, "left": 628.8, "attrs": {} },
    {
      "type": "wokwi-pushbutton",
      "id": "btn3",
      "top": 73.4,
      "left": 518.4,
      "attrs": { "color": "green", "xray": "1", "bounce": "0", "label": "Pseudo Sensor3" }
    },
    {
      "type": "wokwi-pushbutton",
      "id": "btn4",
      "top": 150.2,
      "left": 518.4,
      "attrs": { "color": "green", "xray": "1", "bounce": "0", "label": "Pseudo Sensor4" }
    },
    { "type": "wokwi-junction", "id": "j2", "top": 81.6, "left": 628.8, "attrs": {} },
    { "type": "wokwi-junction", "id": "j3", "top": 158.4, "left": 628.8, "attrs": {} }
  ],
  "connections": [
    [ "btn1:1.r", "mega:5V", "green", [ "v0", "h48.2", "v288", "h-432" ] ],
    [ "btn1:2.l", "mega:50", "green", [ "v0.2", "h-172.8", "v152.6" ] ],
    [ "btn2:2.l", "mega:51", "green", [ "h-105.6", "v76" ] ],
    [ "j1:J", "btn2:1.r", "green", [ "v0" ] ],
    [ "btn3:2.l", "mega:52", "green", [ "h-28.8", "v57.8", "h-124.8" ] ],
    [ "btn4:2.l", "mega:53", "green", [ "h0" ] ],
    [ "btn4:1.r", "j3:J", "green", [ "v0" ] ],
    [ "btn3:1.r", "j2:J", "green", [ "v0" ] ]
  ],
  "dependencies": {}
}
```

## code

Very important to consider is the positioning of the defines for the ISR Handler

```cpp
#include <Arduino.h>
#include <gravitiyWatermeter.hpp>

GravityWatermeter _GravityWatermeter;

#ifdef ARDUINO_AVR_MEGA2560
// Define ISRs using inline handlers from header
DEFINE_PCINT0_ISR()
DEFINE_PCINT1_ISR()
DEFINE_PCINT2_ISR()
#endif

void setup() {
 Serial.begin(9600);
delay(2000); 

_GravityWatermeter.begin(GravityWatermeter::pinGroups::INTERNAL_GRP0, 50,51,52, 53);

delay(2000); // Wait for serial monitor to open


}

void loop() {
  
  _GravityWatermeter.run();
  Serial.print(String("Waterflow: ") + _GravityWatermeter.getWaterflow1());
  Serial.print(" L/h");
  Serial.print(" | " + String(_GravityWatermeter.getWaterflow2()) + " L/h");
  Serial.print(" | " + String(_GravityWatermeter.getWaterflow3()) + " L/h");
  Serial.println(" | " + String(_GravityWatermeter.getWaterflow4()) + " L/h");


}
```


# Functions
The following chapter describes the public functions which you can use to run the class

## begin()
This function has been overloaded three times. You can ether only pass one pin if you only want to connect one watermeter or you can pass more pins up to 4 pins. 

```cpp
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
```

| Argument | Description |
| ---- | ---- |
| group | Interrupt group which you want to use |
| pin1 | corresponding pin of the selected group |
| pin2 | corresponding pin of the selected group|
| pin3 | corresponding pin of the selected group |
| pin4 | corresponding pin of the selected group|

> [!NOTE]  
> Please pass only pins of the same group. Mixing pins will not work (e.g. passsing PIN "15" of group 1 and passing PIN "A8" of group 2.


### Available Pins on MEGA2560 R3

**Group 0**
| PCINT  | AVR-Pin | Arduino-Pin | Description        |
| ------ | ------- | ----------- | ---------------- |
| PCINT0 | PB0     | D53 (SS)    | SPI-SS           |
| PCINT1 | PB1     | D52 (SCK)   | SPI-SCK          |
| PCINT2 | PB2     | D51 (MOSI)  | SPI-MOSI         |
| PCINT3 | PB3     | D50 (MISO)  | SPI-MISO         |
| PCINT4 | PB4     | D10         | PWM         |
| PCINT5 | PB5     | D11         | PWM         |
| PCINT6 | PB6     | D12         | –                |
| PCINT7 | PB7     | D13         | LED-Builtin |

**Group 1**
| PCINT   | AVR-Pin | Arduino-Pin | Description           |
| ------- | ------- | ----------- | ------------------- |
| PCINT8  | PJ0     | D15 (RX3)   | UART3 RX            |
| PCINT9  | PJ1     | D14 (TX3)   | UART3 TX            |
| PCINT10 | PJ2     | –           | not connected |
| PCINT11 | PJ3     | –           | not connected |
| PCINT12 | PJ4     | –           | not connected |
| PCINT13 | PJ5     | –           | not connected |
| PCINT14 | PJ6     | –           | not connected|
| PCINT15 | PJ7     | –           | not connected |

**Group 2**
| PCINT   | AVR-Pin | Arduino-Pin | Description  |
| ------- | ------- | ----------- | ---------- |
| PCINT16 | PK0     | A8          | Analog-Pin |
| PCINT17 | PK1     | A9          | Analog-Pin |
| PCINT18 | PK2     | A10         | Analog-Pin |
| PCINT19 | PK3     | A11         | Analog-Pin |
| PCINT20 | PK4     | A12         | Analog-Pin |
| PCINT21 | PK5     | A13         | Analog-Pin |
| PCINT22 | PK6     | A14         | Analog-Pin |
| PCINT23 | PK7     | A15         | Analog-Pin |


## Run()
The mainfunction for the calculation of the liter per hour result. Within this function the interrupts will be stopped for a very short time to guarantee atomic access to the counter variables.

```cpp
auto run() -> void;
```

## getWaterflow1() ... getWaterflow4()
Returns the calculated waterflow

```cpp
auto getWaterflow1() -> float
```

```cpp
auto getWaterflow2() -> float
```

```cpp
auto getWaterflow3() -> float
```

```cpp
auto getWaterflow4() -> float
```

## setComputeInterval()

By default the computation of the water flow has be done every 1000ms. If you need to change the interval you can do this with the help of this function.

```cpp
auto setComputeInterval(uint32_t intervalMs) -> void 
```

>[!Note]
>If you reduce the interval time to much it may reduce the accuracy of the result
