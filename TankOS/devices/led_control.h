#ifndef _LED_CONTROL_
#define _LED_CONTROL_

#include "led.h"

#define BLINKING_TICKS 300
#define BLINKING_FAST_TICKS 150
#define FLASH_TICKS 1000
#define LONG_FLASH_TICKS 2000
#define GROUP_RUN_TICKS 35

DEFINE_HANDLE(ControlledLeds);

typedef enum {
    LedsEnabled,
    LedsDisabled,
    LedsBlinking,
    LedsBlinkingFast,
    LedsFlash,
    LedsLongFlash,

    // For Led groups
    LedsGroupRun,
    LedsGroupRunning,

    LedsInvalid
} LedState;

// If one Led is part of multiple ControlledLeds, the LedState
// of the last created ControlledLeds takes preference.

ControlledLeds newControlledLed(Led led);
ControlledLeds newControlledLedGroup(LedGroup group);
ControlledLeds destroyControlledLeds(ControlledLeds leds);
BOOL controlledLedsValid(ControlledLeds leds);

void controlLeds(ControlledLeds leds, LedState state); // Uses default durations given above
void controlLedsDuration(ControlledLeds leds, LedState state, uint16_t effect_duration);
LedState getControlledLedsState(ControlledLeds leds);

// Setting bits in mask high will make certain Leds ignore the effect. These leds will stay disabled.
// The MSB corresponds to the first led in the group, and so on.
void controlLedsMask(ControlledLeds leds, uint16_t mask);

// Calculate the led mask based on val within min..max
void controlLedsMaskValue(ControlledLeds leds, uint16_t val, uint16_t min, uint16_t max);

// Should be invoked every millisecond
// e.g. by binding __vector_LED_CONTROL_INTERRUPT to a timer-interrupt
void led_control_tick();

#endif // _LED_CONTROL_
