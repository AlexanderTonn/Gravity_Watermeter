#include "gravitiyWatermeter.hpp"

// Definition
volatile uint8_t _gravityWatermeter_BitMaskB = 0;
volatile uint8_t _gravityWatermeter_BitMaskJ = 0;
volatile uint8_t _gravityWatermeter_BitMaskK = 0;

#ifdef ARDUINO_AVR_MEGA2560
volatile uint8_t _gravityWatermeter_LastPINB = 0;
volatile uint8_t _gravityWatermeter_LastPINJ = 0;
volatile uint8_t _gravityWatermeter_LastPINK = 0;
volatile uint16_t _gravityWatermeter_Count[26] = {0};
#endif

auto GravityWatermeter::checkPinValid(uint8_t pin) -> int
{
    auto retVal = 0;

#ifdef ARDUINO_AVR_MEGA2560
    switch (pin)
    {
        // External inputs valid pins
    case 2:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT0) : 0;
        break;
    case 3:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT1) : 0;
        break;
    case 18:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT5) : 0;
        break;
    case 19:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT4) : 0;
        break;
    case 20:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT3) : 0;
        break;
    case 21:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT2) : 0;
        break;
    case 24:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT6) : 0;
        break;
    case 25:
        retVal = mINTGroup == pinGroups::EXTERNAL_INPUTS ? (1 << INT7) : 0;
        break;
    // ##############
    // PCINT 0
    case 10:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB4) : 0;
        break;
    case 11:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB5) : 0;
        break;
    case 12:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB6) : 0;
        break;
    case 13:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB7) : 0;
        break;
    case 50:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB3) : 0;
        break;
    case 51:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB2) : 0;
        break;
    case 52:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB1) : 0;
        break;
    case 53:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP0 ? (1 << PINB0) : 0;
        break;
    // ##############
    // PCINT 1
    case 14:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP1 ? (1 << PINJ1) : 0;
        break;
    case 15:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP1 ? (1 << PINJ0) : 0;
        break;
    // ##############
    // PCINT 2
    case A8:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK0) : 0;
        break;
    case A9:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK1) : 0;
        break;
    case A10:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK2) : 0;
        break;
    case A11:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK3) : 0;
        break;
    case A12:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK4) : 0;
        break;
    case A13:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK5) : 0;
        break;
    case A14:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK6) : 0;
        break;
    case A15:
        retVal = mINTGroup == pinGroups::INTERNAL_GRP2 ? (1 << PINK7) : 0;
        break;
    default:
        return 0;
        break;
    }
#endif

    return retVal;
}
/**
 * @brief
 * @note INTERNAL_GRP0 == PINB (10-13,50-53)
 * @note INTERNAL_GRP1 == PINJ (14-15)
 * @note INTERNAL_GRP2 == PINK (A8-A15)
 * @note EXTERNAL_INPUTS == INT0-INT7 (2,3,18-21,24,25)
 * @param group
 * @param pin1
 */
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1) -> void
{
    if (mInitialized)
        return;
    mINTGroup = group;
    enable(pin1);

    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2) -> void
{
    if (mInitialized)
        return;

    enable(pin1, pin2);

    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3) -> void
{
    if (mInitialized)
        return;
    enable(pin1, pin2, pin3);

    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) -> void
{
    if (mInitialized)
        return;
    enable(pin1, pin2, pin3, pin4);

    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5) -> void
{
    if (mInitialized)
        return;
    enable(pin1, pin2, pin3, pin4, pin5);

    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6) -> void
{
    if (mInitialized)
        return;
    enable(pin1, pin2, pin3, pin4, pin5, pin6);


    mInitialized = true;
}
auto GravityWatermeter::begin(pinGroups group, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7) -> void
{
    if (mInitialized)
        return;
    enable(pin1, pin2, pin3, pin4, pin5, pin6, pin7);

    mInitialized = true;
}
/**
 * @brief Sets the interrupt registers
 *
 */
auto GravityWatermeter::enable(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8) -> void
{
#ifdef ARDUINO_AVR_MEGA2560
    switch (mINTGroup)
    {

    case pinGroups::INTERNAL_GRP0:
        _gravityWatermeter_BitMaskB |= checkPinValid(pin1) | checkPinValid(pin2) | checkPinValid(pin3) | checkPinValid(pin4) | checkPinValid(pin5) | checkPinValid(pin6) | checkPinValid(pin7) | checkPinValid(pin8);
        setArrayPos(pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8);
        DDRB &= ~(_gravityWatermeter_BitMaskB); // Set as input
        // PORTB |= (_gravityWatermeter_BitMaskB);  // Enable pull-up resistors
        PCMSK0 |= (_gravityWatermeter_BitMaskB); // Set bitmask
        PCICR |= (1 << PCIE0);                   // Enable PCINT0
        _gravityWatermeter_LastPINB = PINB;
        break;
    case pinGroups::INTERNAL_GRP1:
        _gravityWatermeter_BitMaskJ |= checkPinValid(pin1) | checkPinValid(pin2) | checkPinValid(pin3) | checkPinValid(pin4) | checkPinValid(pin5) | checkPinValid(pin6) | checkPinValid(pin7) | checkPinValid(pin8);
        setArrayPos(pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8);
        DDRJ &= ~(_gravityWatermeter_BitMaskJ); // Set as input
        // PORTJ |= (_gravityWatermeter_BitMaskJ);  // Enable pull-up resistors
        PCMSK1 |= (_gravityWatermeter_BitMaskJ); // Set bitmask
        PCICR |= (1 << PCIE1);                   // Enable PCINT1
        _gravityWatermeter_LastPINJ = PINJ;

        break;
    case pinGroups::INTERNAL_GRP2:
        _gravityWatermeter_BitMaskK |= checkPinValid(pin1) | checkPinValid(pin2) | checkPinValid(pin3) | checkPinValid(pin4) | checkPinValid(pin5) | checkPinValid(pin6) | checkPinValid(pin7) | checkPinValid(pin8);
        setArrayPos(pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8);
        DDRK &= ~(_gravityWatermeter_BitMaskK); // Set as input
        // PORTK |= (_gravityWatermeter_BitMaskK);  // Enable pull-up resistors
        PCMSK2 |= (_gravityWatermeter_BitMaskK); // Set bitmask
        PCICR |= (1 << PCIE2);                   // Enable PCINT2
        _gravityWatermeter_LastPINK = PINK;

        break;
    default:
        break;
    }

#endif
};
/**
 * @brief
 *
 */
auto GravityWatermeter::run() -> void
{
    if (!mInitialized)
        return;

    if (millis() - mLastMillis >= mComputeIntervalMs)
    {
        startCriticalSection();
        compute();
        stopCriticalSection();
        mLastMillis = millis();
    }
}
/**
 * @brief
 *
 */
auto GravityWatermeter::compute() -> void
{
    auto count1 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[0])];
    auto count2 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[1])];
    auto count3 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[2])];
    auto count4 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[3])];
    auto count5 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[4])];
    auto count6 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[5])];
    auto count7 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[6])];
    auto count8 = _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[7])];

    for(int i = 0; i < 8; i++)
    {
        _gravityWatermeter_Count[static_cast<uint8_t>(mPinPos[i])] = 0;
    }

    mflow_lph1 = (count1 * 3600.0f) / 450.0f; // L/h
    mflow_lph2 = (count2 * 3600.0f) / 450.0f; // L/h
    mflow_lph3 = (count3 * 3600.0f) / 450.0f; // L/h
    mflow_lph4 = (count4 * 3600.0f) / 450.0f; // L/h
    mflow_lph5 = (count5 * 3600.0f) / 450.0f; // L/h
    mflow_lph6 = (count6 * 3600.0f) / 450.0f; // L/h
    mflow_lph7 = (count7 * 3600.0f) / 450.0f; // L/h
    mflow_lph8 = (count8 * 3600.0f) / 450.0f; // L/h
}

/**
 * @brief Stops the configured interrupts
 * @note Use with care, as this stops all interrupts on the configured group!
 */
auto GravityWatermeter::startCriticalSection() -> void
{
    mSREG = SREG;
    cli();
}
/**
 * @brief Restarts the configured interrupts
 *
 */
auto GravityWatermeter::stopCriticalSection() -> void
{
    SREG = mSREG;
}
/**
 * @brief 
 * 
 * @param pin 
 * @param idx 
 */
auto GravityWatermeter::mapPin(uint8_t pin, uint8_t idx) -> void
{

#ifdef ARDUINO_AVR_MEGA2560

    switch (pin)
    {
        // External inputs valid pins
    case 2: mPinPos[idx] = CountArrayPos::_PIN2; break;
    case 3: mPinPos[idx] = CountArrayPos::_PIN3; break;
    case 18: mPinPos[idx] = CountArrayPos::_PIN18; break;
    case 19: mPinPos[idx] = CountArrayPos::_PIN19; break;
    case 20: mPinPos[idx] = CountArrayPos::_PIN20; break;
    case 21: mPinPos[idx] = CountArrayPos::_PIN21; break;
    case 24: mPinPos[idx] = CountArrayPos::_PIN24; break;
    case 25: mPinPos[idx] = CountArrayPos::_PIN25; break;
    // ##############
    // PCINT 0
    case 10:mPinPos[idx] = CountArrayPos::_PIN10; break;
    case 11:mPinPos[idx] = CountArrayPos::_PIN11; break;
    case 12:mPinPos[idx] = CountArrayPos::_PIN12;break;
    case 13:mPinPos[idx] = CountArrayPos::_PIN13;break;
    case 50:mPinPos[idx] = CountArrayPos::_PIN50;break;
    case 51:mPinPos[idx] = CountArrayPos::_PIN51;break;
    case 52:mPinPos[idx] = CountArrayPos::_PIN52;break;
    case 53:mPinPos[idx] = CountArrayPos::_PIN53;break;
    // ##############
    // PCINT 1
    case 14: mPinPos[idx] = CountArrayPos::_PIN14;break;
    case 15: mPinPos[idx] = CountArrayPos::_PIN15;break;
    // ##############
    // PCINT 2
    case A8:mPinPos[idx] = CountArrayPos::_PINA8;break;
    case A9:mPinPos[idx] = CountArrayPos::_PINA9;break;
    case A10:mPinPos[idx] = CountArrayPos::_PINA10;break;
    case A11:mPinPos[idx] = CountArrayPos::_PINA11;break;
    case A12:mPinPos[idx] = CountArrayPos::_PINA12;break;
    case A13:mPinPos[idx] = CountArrayPos::_PINA13;break;
    case A14:mPinPos[idx] = CountArrayPos::_PINA14;break;
    case A15:mPinPos[idx] = CountArrayPos::_PINA15;break;
    default:
        break;
    }

#endif
}

auto GravityWatermeter::setArrayPos(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8) -> void
{
    mapPin(pin1, 0);
    mapPin(pin2, 1);
    mapPin(pin3, 2);
    mapPin(pin4, 3);
    mapPin(pin5, 4);
    mapPin(pin6, 5);
    mapPin(pin7, 6);
    mapPin(pin8, 7);
}
