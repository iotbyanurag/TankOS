#ifndef TANK_IO_PROTOCOL_H_
#define TANK_IO_PROTOCOL_H_

#include <tank_os_common.h>
#include <tank_io_address.h>
#include "tank_button.h"

// A random value, that is returned by TANK_IO_isInitialized to mean
// "true". "Random" value to assert correct communication.
#define TANK_IO_INITIALIZED 214

typedef enum {
	LEDS_ALL,
	LEDS_LEFT,
	LEDS_RIGHT,
	LEDS_MIDDLE,
	LEDS_RED,
	LEDS_YELLOW,
	LEDS_WHITE,
	LEDS_GREEN
} TankIoLeds;

typedef struct MaskedLeds {
	TankIoLeds leds;
	uint16_t mask;
} MaskedLeds;

typedef struct SingleLed {
	TankIoLeds leds;
	uint8_t index;
} SingleLed;

typedef struct SetLedParam {
	SingleLed led;
	BOOL value;
} SetLedParam;

typedef struct BlinkedLedParam {
	SingleLed led;
	uint8_t times;
} BlinkLedParam;

typedef struct BlinkLedsParam {
	MaskedLeds leds;
	uint8_t times;
} BlinkLedsParam;

typedef struct BlinkAllLedParam {
	TankIoLeds leds;
	uint8_t times;
} BlinkAllLedsParam;

typedef struct FlashLedParam {
	SingleLed led;
	uint16_t milliseconds;
} FlashLedParam;

typedef struct FlashLedsParam {
	MaskedLeds leds;
	uint16_t milliseconds;
} FlashLedsParam;

typedef struct FlashAllLedsParam {
	TankIoLeds leds;
	uint16_t milliseconds;
} FlashAllLedsParam;

typedef struct BlinkByteParam {
	TankIoLeds display;
	TankIoLeds notifier;
	byte data;
} BlinkByteParam;

enum {
	// == System
	// Triggers a hardware-reset
	TANK_IO_reset = 0xA0, // () => ()
	// Returns FALSE, while the IO-controller is not yet operational
	TANK_IO_isInitialized = 0xA1, // () => uint8_t

	// == Input
	TANK_IO_buttonStatus = 0xA2, // () => uint8_t
	// Buttons, that have been pressed since the last call to this.
	TANK_IO_pressedButtons = 0xA3, // () => uint8_t

	// == Output. Mirrors API of devices/led.h
	TANK_IO_enableLed = 0xA4, // SingleLed => ()
	TANK_IO_disableLed = 0xA5, // SingleLed => ()
	TANK_IO_setLed = 0xA6, // SetLedParam => ()

	TANK_IO_setLeds = 0xA7, // MaskedLeds => ()
	TANK_IO_enableLeds = 0xA8, // TankIoLeds => ()
	TANK_IO_disableLeds = 0xA9, // TankIoLeds => ()

	TANK_IO_blinkLed = 0xAA, // BlinkLedParam => ()
	TANK_IO_blinkLeds = 0xAB, // BlinkLedsParam => ()
	TANK_IO_blinkAllLeds = 0xAC, // BlinkAllLedsParam => ()

	TANK_IO_flashLed = 0xAD, // FlashLedParam => ()
	TANK_IO_flashLeds = 0xAE, // FlashLedsParam => ()
	TANK_IO_flashAllLeds = 0xAF, // FlashAllLedsParam => ()
};

#endif