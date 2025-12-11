#pragma once

#include <Arduino.h>

class GravityWatermeter
{
private:
    bool mInitialized = false;

#ifdef ARDUINO_AVR_MEGA2560
private:
    enum class pinGroups
    {
        INTERNAL_GRP0,
        INTERNAL_GRP1,
        INTERNAL_GRP2,
        EXTERNAL_INPUTS,
    } mINTGroup;

public:
    enum class CountArrayPos : uint8_t
    {
        _PIN2,
        _PIN3,
        _PIN10,
        _PIN11,
        _PIN12,
        _PIN13,
        _PIN14,
        _PIN15,
        _PIN18,
        _PIN19,
        _PIN20,
        _PIN21,
        _PIN24,
        _PIN25,
        _PINA8,
        _PINA9,
        _PINA10,
        _PINA11,
        _PINA12,
        _PINA13,
        _PINA14,
        _PINA15,
        _PIN50,
        _PIN51,
        _PIN52,
        _PIN53
    };
private:
    CountArrayPos mSelectedPins[4];

#endif
public:
    auto begin(pinGroups group, uint8_t pin1) -> void;
    auto begin(pinGroups group, uint8_t pin1, uint8_t pin2) -> void;
    auto begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3) -> void;
    auto begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) -> void;

    auto startCriticalSection() -> void;
    auto stopCriticalSection() -> void;

    auto getWaterflow1() -> float;
    auto getWaterflow2() -> float;
    auto getWaterflow3() -> float;
    auto getWaterflow4() -> float;

private:
    auto checkPinValid(uint8_t pin) -> int;
    auto enable() -> void;

    auto assignPins(uint8_t pin1, uint8_t pin2 = 255, uint8_t pin3 = 255, uint8_t pin4 = 255) -> void;

    volatile float mWaterflowAbsPin1 = 0.0f;
    volatile float mWaterflowAbsPin2 = 0.0f;
    volatile float mWaterflowAbsPin3 = 0.0f;
    volatile float mWaterflowAbsPin4 = 0.0f;
};

extern int _gravityWatermeter_BitMask;

#ifdef ARDUINO_AVR_MEGA2560

extern volatile uint8_t _gravityWatermeter_LastPINB;
extern volatile uint8_t _gravityWatermeter_LastPINJ;
extern volatile uint8_t _gravityWatermeter_LastPINK;

extern volatile uint32_t _gravityWatermeter_Count[26];
#endif

