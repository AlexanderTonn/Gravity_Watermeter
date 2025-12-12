#pragma once

#include <Arduino.h>

class GravityWatermeter
{
private:
    bool mInitialized = false;
    uint8_t mSREG ;
    uint32_t mLastMillis;
    uint32_t mComputeIntervalMs = 1000; // Default compute interval: 1000 ms
    float mflow_lph1 = 0.0f;
    float mflow_lph2 = 0.0f;
    float mflow_lph3 = 0.0f;
    float mflow_lph4 = 0.0f;

#ifdef ARDUINO_AVR_MEGA2560
public:

    enum class pinGroups
    {
        INTERNAL_GRP0,
        INTERNAL_GRP1,
        INTERNAL_GRP2,
        EXTERNAL_INPUTS,
    } mINTGroup;

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
    CountArrayPos mPinPos[4];

#endif
public:
    auto begin(pinGroups group, uint8_t pin1) -> void;
    auto begin(pinGroups group, uint8_t pin1, uint8_t pin2) -> void;
    auto begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3) -> void;
    auto begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) -> void;

    auto run() -> void;

    auto startCriticalSection() -> void;
    auto stopCriticalSection() -> void;

    auto getWaterflow1() -> float { return mflow_lph1; };
    auto getWaterflow2() -> float { return mflow_lph2; };
    auto getWaterflow3() -> float { return mflow_lph3; };
    auto getWaterflow4() -> float { return mflow_lph4; };

    auto setComputeInterval(uint32_t intervalMs) -> void { mComputeIntervalMs = intervalMs; };

private:
    auto checkPinValid(uint8_t pin) -> int;
    auto enable(uint8_t pin1, uint8_t pin2 = 0, uint8_t pin3 = 0, uint8_t pin4 = 0) -> void;
    auto compute() -> void;
    auto setArrayPos(uint8_t pin1, uint8_t pin2 = 0, uint8_t pin3 = 0, uint8_t pin4 = 0) -> void;
    auto mapPin(uint8_t pin, uint8_t idx) -> void; 
};

extern volatile uint8_t _gravityWatermeter_BitMaskB;
extern volatile uint8_t _gravityWatermeter_BitMaskJ;
extern volatile uint8_t _gravityWatermeter_BitMaskK;

#ifdef ARDUINO_AVR_MEGA2560

extern volatile uint8_t _gravityWatermeter_LastPINB;
extern volatile uint8_t _gravityWatermeter_LastPINJ;
extern volatile uint8_t _gravityWatermeter_LastPINK;

extern volatile uint16_t _gravityWatermeter_Count[26];

/**
 * @brief Interrupt Service Routine for PCINT0 (pins 50-53, 10-13)
 */
inline void __attribute__((always_inline)) PCINT0_ISR_Handler()
{
    auto now = PINB;
    auto changed = (now ^ _gravityWatermeter_LastPINB) & _gravityWatermeter_BitMaskB;

    auto rose = changed & now;
    _gravityWatermeter_LastPINB = now;

    if(rose & (1 << PINB4)) // Pin 10
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN10]++;

    if(rose & (1 << PINB5)) // Pin 11
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN11]++;
    
    if(rose & (1 << PINB6)) // Pin 12
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN12]++; 
    
    if(rose & (1 << PINB7)) // Pin 13
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN13]++;
    
    if(rose & (1 << PINB3)) // Pin 50
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN50]++;
    
    if(rose & (1 << PINB2)) // Pin 51
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN51]++;
    
    if(rose & (1 << PINB1)) // Pin 52
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN52]++;
    
    if(rose & (1 << PINB0)) // Pin 53
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN53]++;
}

inline void __attribute__((always_inline)) PCINT1_ISR_Handler()
{
    auto now = PINJ;
    auto changed = (now ^ _gravityWatermeter_LastPINJ) &  _gravityWatermeter_BitMaskJ;

    auto rose = changed & now;
    _gravityWatermeter_LastPINJ = now;

    if(rose & (1 << PINJ1)) // Pin 15
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA15]++;

    if(rose & (1 << PINJ0)) // Pin 14
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA14]++;
}

inline void __attribute__((always_inline)) PCINT2_ISR_Handler()
{
    auto now = PINK;
    auto changed = (now ^ _gravityWatermeter_LastPINK) & _gravityWatermeter_BitMaskK;

    auto rose = changed & now;
    _gravityWatermeter_LastPINK = now;

    if(rose & (1 << PINK0)) // Pin A8
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA8]++;

    if(rose & (1 << PINK1)) // Pin A9
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA9]++;
    
    if(rose & (1 << PINK2)) // Pin A10
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA10]++; 
    
    if(rose & (1 << PINK3)) // Pin A11
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA11]++;
    
    if(rose & (1 << PINK4)) // Pin A12
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA12]++;
    
    if(rose & (1 << PINK5)) // Pin A13
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA13]++;
    
    if(rose & (1 << PINK6)) // Pin A14
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA14]++;
    
    if(rose & (1 << PINK7)) // Pin A15
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA15]++;
}

// ISR wrapper macros - define these in your main.cpp
#define DEFINE_PCINT0_ISR() ISR(PCINT0_vect) { PCINT0_ISR_Handler(); }
#define DEFINE_PCINT1_ISR() ISR(PCINT1_vect) { PCINT1_ISR_Handler(); }
#define DEFINE_PCINT2_ISR() ISR(PCINT2_vect) { PCINT2_ISR_Handler(); }

#endif

