#include "gravitiyWatermeter.hpp"

// Definition 
int _gravityWatermeter_BitMask = 0;

#ifdef ARDUINO_AVR_MEGA2560
volatile uint8_t _gravityWatermeter_LastPINB = 0;
volatile uint8_t _gravityWatermeter_LastPINJ = 0;
volatile uint8_t _gravityWatermeter_LastPINK = 0;
volatile uint32_t _gravityWatermeter_Count[26] = {0};
#endif

auto GravityWatermeter::checkPinValid(uint8_t pin) -> int
{
    auto retVal = 0;

#ifdef ARDUINO_AVR_MEGA2560
    switch (pin)
    {
        // External inputs valid pins
    case 2:
        retVal = (1 << INT0);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 3:
        retVal = (1 << INT1);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 18:
        retVal = (1 << INT5);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 19:
        retVal = (1 << INT4);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 20:
        retVal = (1 << INT3);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 21:
        retVal = (1 << INT2);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 24:
        retVal = (1 << INT6);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    case 25:
        retVal = (1 << INT7);
        mINTGroup = pinGroups::EXTERNAL_INPUTS;
        break;
    // ##############
    // PCINT 0
    case 10:
        retVal = (1 << PINB4);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 11:
        retVal = (1 << PINB5);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 12:
        retVal = (1 << PINB6);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 13:
        retVal = (1 << PINB7);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 50:
        retVal = (1 << PINB3);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 51:
        retVal = (1 << PINB2);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 52:
        retVal = (1 << PINB1);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    case 53:
        retVal = (1 << PINB0);
        mINTGroup = pinGroups::INTERNAL_GRP0;
        break;
    // ##############
    // PCINT 1
    case 14:
        retVal = (1 << PINJ1);
        mINTGroup = pinGroups::INTERNAL_GRP1;
        break;
    case 15:
        retVal = (1 << PINJ0);
        mINTGroup = pinGroups::INTERNAL_GRP1;
        break;
    // ##############
    // PCINT 2
    case A8:
        retVal = (1 << PINK0);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A9:
        retVal = (1 << PINK1);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A10:
        retVal = (1 << PINK2);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A11:
        retVal = (1 << PINK3);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A12:
        retVal = (1 << PINK4);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A13:
        retVal = (1 << PINK5);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A14:
        retVal = (1 << PINK6);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    case A15:
        retVal = (1 << PINK7);
        mINTGroup = pinGroups::INTERNAL_GRP2;
        break;
    default:
        return 0;
        break;
    }
#endif

    return retVal;

}

auto GravityWatermeter::begin(pinGroups group, uint8_t pin1) -> void
{
    if (mInitialized)
        return;

    _gravityWatermeter_BitMask = checkPinValid(pin1);

    enable();
    assignPins(pin1);


    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2) -> void
{
    if (mInitialized)
        return;

    _gravityWatermeter_BitMask = checkPinValid(pin1) | checkPinValid(pin2);

    enable();
    assignPins(pin1, pin2);
    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3) -> void
{
    if (mInitialized)
        return;

    _gravityWatermeter_BitMask = checkPinValid(pin1) | checkPinValid(pin2) | checkPinValid(pin3);

    enable();
    assignPins(pin1, pin2, pin3);
    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) -> void
{
    if (mInitialized)
        return;

    _gravityWatermeter_BitMask = checkPinValid(pin1) | checkPinValid(pin2) | checkPinValid(pin3) | checkPinValid(pin4);

    enable();
    assignPins(pin1, pin2, pin3, pin4);
    mInitialized = true;
}
/**
 * @brief Sets the interrupt registers
 * 
 */
auto GravityWatermeter::enable() -> void
{
    #ifdef ARDUINO_AVR_MEGA2560

    switch (mINTGroup)
    {
    case pinGroups::EXTERNAL_INPUTS:
        EIMSK |= _gravityWatermeter_BitMask; // Enable external interrupts
        break;

    case pinGroups::INTERNAL_GRP0:
        DDRB &= ~_gravityWatermeter_BitMask; // Set as input
        PORTB |= _gravityWatermeter_BitMask; // Enable pull-up resistors
        PCICR |= (1 << PCIE0); // Enable PCINT0
        PCMSK0 |= _gravityWatermeter_BitMask;    // Set bitmask
        break;
    case pinGroups::INTERNAL_GRP1:
        DDRJ &= ~_gravityWatermeter_BitMask; // Set as input
        PORTJ |= _gravityWatermeter_BitMask; // Enable pull-up resistors
        PCICR |= (1 << PCIE1); // Enable PCINT1
        PCMSK1 |= _gravityWatermeter_BitMask;    // Set bitmask
        break;
    case pinGroups::INTERNAL_GRP2:
        DDRK &= ~_gravityWatermeter_BitMask; // Set as input
        PORTK |= _gravityWatermeter_BitMask; // Enable pull-up resistors
        PCICR |= (1 << PCIE2); // Enable PCINT2
        PCMSK2 |= _gravityWatermeter_BitMask;    // Set bitmask
        break;
    default:
        break;
    }

    #endif
};
/**
 * @brief Stops the configured interrupts
 * @note Use with care, as this stops all interrupts on the configured group!
 */
auto GravityWatermeter::startCriticalSection() -> void
{
    
    switch (mINTGroup)
    {
    case pinGroups::EXTERNAL_INPUTS:
        /* code */
        break;
    
    default:
        break;
    }

}
/**
 * @brief Restarts the configured interrupts
 * 
 */
auto GravityWatermeter::stopCriticalSection() -> void
{
    interrupts();
}

auto GravityWatermeter::getWaterflow1() -> float
{
    static auto timestamp = millis();

    mWaterflowAbsPin1 += _gravityWatermeter_Count[(uint8_t)mSelectedPins[0]] / 450.0; // 450 Ticks per Liter

    if(millis() - timestamp >= 1000)
    {
        timestamp = millis();

        if(mWaterflowAbsPin1 > 0.0f)
        {
            auto result = mWaterflowAbsPin1 * 3600.0f; // L/h
            _gravityWatermeter_Count[(uint8_t)mSelectedPins[0]] = 0.0f;
            return result; 
        }
    }

    return 0.0f;
}

auto GravityWatermeter::getWaterflow2() -> float
{
    static auto timestamp = millis();

    mWaterflowAbsPin2 += _gravityWatermeter_Count[(uint8_t)mSelectedPins[1]] / 450.0; // 450 Ticks per Liter

    if(millis() - timestamp >= 1000)
    {
        timestamp = millis();

        if(mWaterflowAbsPin2 > 0.0f)
        {
            auto result = mWaterflowAbsPin2 * 3600.0f; // L/h
            _gravityWatermeter_Count[(uint8_t)mSelectedPins[1]] = 0.0f;
            return result; 
        }
    }

    return 0.0f;
}

auto GravityWatermeter::getWaterflow3() -> float
{
    static auto timestamp = millis();

    mWaterflowAbsPin3 += _gravityWatermeter_Count[(uint8_t)mSelectedPins[2]] / 450.0; // 450 Ticks per Liter

    if(millis() - timestamp >= 1000)
    {
        timestamp = millis();

        if(mWaterflowAbsPin3 > 0.0f)
        {
            auto result = mWaterflowAbsPin3 * 3600.0f; // L/h
            _gravityWatermeter_Count[(uint8_t)mSelectedPins[2]] = 0.0f;
            return result; 
        }
    }

    return 0.0f;
}

auto GravityWatermeter::getWaterflow4() -> float
{
    static auto timestamp = millis();

    mWaterflowAbsPin4 += _gravityWatermeter_Count[(uint8_t)mSelectedPins[3]] / 450.0; // 450 Ticks per Liter

    if(millis() - timestamp >= 1000)
    {
        timestamp = millis();

        if(mWaterflowAbsPin4 > 0.0f)
        {
            auto result = mWaterflowAbsPin4 * 3600.0f; // L/h
            _gravityWatermeter_Count[(uint8_t)mSelectedPins[3]] = 0.0f;
            return result; 
        }
    }

    return 0.0f;
}

auto GravityWatermeter::assignPins(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) -> void
{
    switch (pin1)
    {
        #ifdef ARDUINO_AVR_MEGA2560

            // External inputs valid pins
    case 2:
        mSelectedPins [0] = CountArrayPos::_PIN2;
        break;
    case 3:
        mSelectedPins [0] = CountArrayPos::_PIN3;
        break;
    case 18:
        mSelectedPins [0] = CountArrayPos::_PIN18;
        break;
    case 19:
        mSelectedPins [0] = CountArrayPos::_PIN19;
        break;
    case 20:
        mSelectedPins [0] = CountArrayPos::_PIN20;
        break;
    case 21:
        mSelectedPins [0] = CountArrayPos::_PIN21;
        break;
    case 24:
        mSelectedPins [0] = CountArrayPos::_PIN24;
        break;
    case 25:
        mSelectedPins [0] = CountArrayPos::_PIN25;
        break;
    // ##############
    // PCINT 0
    case 10:
        mSelectedPins [0] = CountArrayPos::_PIN10;
        break;
    case 11:
        mSelectedPins [0] = CountArrayPos::_PIN11;
        break;
    case 12:
        mSelectedPins [0] = CountArrayPos::_PIN12;
        break;
    case 13:
        mSelectedPins [0] = CountArrayPos::_PIN13;
        break;
    case 50:
        mSelectedPins [0] = CountArrayPos::_PIN50;
        break;
    case 51:
        mSelectedPins [0] = CountArrayPos::_PIN51;
        break;
    case 52:
        mSelectedPins [0] = CountArrayPos::_PIN52;
        break;
    case 53:
        mSelectedPins [0] = CountArrayPos::_PIN53;
        break;
    // ##############
    // PCINT 1
    case 14:
        mSelectedPins [0] = CountArrayPos::_PIN14;
        break;
    case 15:
        mSelectedPins [0] = CountArrayPos::_PIN15;
        break;
    // ##############
    // PCINT 2
    case A8:
        mSelectedPins [0] = CountArrayPos::_PINA8;
        break;
    case A9:
        mSelectedPins [0] = CountArrayPos::_PINA9;
        break;
    case A10:
        mSelectedPins [0] = CountArrayPos::_PINA10;
        break;
    case A11:
        mSelectedPins [0] = CountArrayPos::_PINA11;
        break;
    case A12:
        mSelectedPins [0] = CountArrayPos::_PINA12;
        break;
    case A13:
        mSelectedPins [0] = CountArrayPos::_PINA13;
        break;
    case A14:
        mSelectedPins [0] = CountArrayPos::_PINA14;
        break;
    case A15:
        mSelectedPins [0] = CountArrayPos::_PINA15;
        break;

        #endif
    default:
        break;
    }

    switch (pin2)
    {
        #ifdef ARDUINO_AVR_MEGA2560

            // External inputs valid pins
    case 2:
        mSelectedPins [1] = CountArrayPos::_PIN2;
        break;
    case 3:
        mSelectedPins [1] = CountArrayPos::_PIN3;
        break;
    case 18:
        mSelectedPins [1] = CountArrayPos::_PIN18;
        break;
    case 19:
        mSelectedPins [1] = CountArrayPos::_PIN19;
        break;
    case 20:
        mSelectedPins [1] = CountArrayPos::_PIN20;
        break;
    case 21:
        mSelectedPins [1] = CountArrayPos::_PIN21;
        break;
    case 24:
        mSelectedPins [1] = CountArrayPos::_PIN24;
        break;
    case 25:
        mSelectedPins [1] = CountArrayPos::_PIN25;
        break;
    // ##############
    // PCINT 0
    case 10:
        mSelectedPins [1] = CountArrayPos::_PIN10;
        break;
    case 11:
        mSelectedPins [1] = CountArrayPos::_PIN11;
        break;
    case 12:
        mSelectedPins [1] = CountArrayPos::_PIN12;
        break;
    case 13:
        mSelectedPins [1] = CountArrayPos::_PIN13;
        break;
    case 50:
        mSelectedPins [1] = CountArrayPos::_PIN50;
        break;
    case 51:
        mSelectedPins [1] = CountArrayPos::_PIN51;
        break;
    case 52:
        mSelectedPins [1] = CountArrayPos::_PIN52;
        break;
    case 53:
        mSelectedPins [1] = CountArrayPos::_PIN53;
        break;
    // ##############
    // PCINT 1
    case 14:
        mSelectedPins [1] = CountArrayPos::_PIN14;
        break;
    case 15:
        mSelectedPins [1] = CountArrayPos::_PIN15;
        break;
    // ##############
    // PCINT 2
    case A8:
        mSelectedPins [1] = CountArrayPos::_PINA8;
        break;
    case A9:
        mSelectedPins [1] = CountArrayPos::_PINA9;
        break;
    case A10:
        mSelectedPins [1] = CountArrayPos::_PINA10;
        break;
    case A11:
        mSelectedPins [1] = CountArrayPos::_PINA11;
        break;
    case A12:
        mSelectedPins [1] = CountArrayPos::_PINA12;
        break;
    case A13:
        mSelectedPins [1] = CountArrayPos::_PINA13;
        break;
    case A14:
        mSelectedPins [1] = CountArrayPos::_PINA14;
        break;
    case A15:
        mSelectedPins [1] = CountArrayPos::_PINA15;
        break;

        #endif
    default:
        break;
    }


}


#ifdef ARDUINO_AVR_MEGA2560

/**
 * @brief Construct a new ISR object
 * @note Interrupt Service Routine for PCINT0 (pins 50-53, 10-13)
 */
ISR(PCINT0_vect)
{
    auto now = PINB;
    auto changed = (now ^ _gravityWatermeter_LastPINB) & _gravityWatermeter_BitMask;

    auto rised = changed & now;
    _gravityWatermeter_LastPINB = now;

    if(rised & (1 << PINB4)) // Pin 10
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN10]++;

    if(rised & (1 << PINB5)) // Pin 11
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN11]++;
    
    if(rised & (1 << PINB6)) // Pin 12
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN12]++; 
    
    if(rised & (1 << PINB7)) // Pin 13
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN13]++;
    
    if(rised & (1 << PINB3)) // Pin 50
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN50]++;
    
    if(rised & (1 << PINB2)) // Pin 51
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN51]++;
    
    if(rised & (1 << PINB1)) // Pin 52
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN52]++;
    
    if(rised & (1 << PINB0)) // Pin 53
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PIN53]++;

}

ISR(PCINT1_vect)
{
    auto now = PINJ;
    auto changed = (now ^ _gravityWatermeter_LastPINJ) & _gravityWatermeter_BitMask;

    auto rised = changed & now;
    _gravityWatermeter_LastPINJ = now;

    if(rised & (1 << PINJ1)) // Pin 15
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA15]++;

    if(rised & (1 << PINJ0)) // Pin 14
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA14]++;
}

ISR(PCINT2_vect)
{
    auto now = PINK;
    auto changed = (now ^ _gravityWatermeter_LastPINK) & _gravityWatermeter_BitMask;

    auto rised = changed & now;
    _gravityWatermeter_LastPINK = now;

    if(rised & (1 << PINK0)) // Pin A8
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA8]++;

    if(rised & (1 << PINK1)) // Pin A9
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA9]++;
    
    if(rised & (1 << PINK2)) // Pin A10
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA10]++; 
    
    if(rised & (1 << PINK3)) // Pin A11
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA11]++;
    
    if(rised & (1 << PINK4)) // Pin A12
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA12]++;
    
    if(rised & (1 << PINK5)) // Pin A13
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA13]++;
    
    if(rised & (1 << PINK6)) // Pin A14
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA14]++;
    
    if(rised & (1 << PINK7)) // Pin A15
        _gravityWatermeter_Count[(uint8_t)GravityWatermeter::CountArrayPos::_PINA15]++;

}

#endif