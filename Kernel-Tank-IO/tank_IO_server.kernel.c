/*
 * tank_IO_server.kernel
 *
 * Created: 28.04.2012 11:02:16
 *  Author: Anton
 */

#include "tank_led.h"
#include "tank_button.h"
#include <misc/hardware_reset.h>
#include <kernel/kernel_init.h>
#include <kernel/TWI/twi_rpc_hash_server.h>

#include <tank_IO_protocol.h>

byte initialized = 0;

// This is invoked right before main(), see kernel_init.kernel.c
void kernel_initialized() {
	initialized = TANK_IO_INITIALIZED;
}

void configure_tank_twi() {
	TwiSlaveAddress = TANK_IO_ADDRESS;
	
	// TODO define these values from the linker??
	
	// The TWI bit rate: Maximal TWI frequency (400 KHz).
	// 100 = Medium frequency, 250 = Minimal frequency
	TwiBitRateValue = 17;
	TwiPrescalerMask = 0;
}
KERNEL_INIT(configure_tank_twi)

// ==
// System
// ==

void tankIO_server_reset() {
	initialized = FALSE; // In case the master queries this.
	HARDWARE_RESET();
}
// This is declared ASYNC to cleanly terminate the current TWI-connection.
TWI_RPC_SERVER_FUNCTION_ASYNC_NOTIFY	(tankIO_server_reset, TANK_IO_reset)

void TANKIO_server_isInitialized(TWIBuffer *result) {
	FILL_RESULT(result, initialized, BOOL)
}
TWI_RPC_SERVER_FUNCTION_NOARGS	(TANKIO_server_isInitialized, TANK_IO_isInitialized, BOOL)

// ==
// Input
// ==

void tankIO_server_buttonStatus(TWIBuffer *result) {
	uint8_t buttons = buttonStatusMask();
	FILL_RESULT(result, buttons, uint8_t)
}
TWI_RPC_SERVER_FUNCTION_NOARGS	(tankIO_server_buttonStatus, TANK_IO_buttonStatus, uint8_t)

void tankIO_server_pressedButtons(TWIBuffer *result) {
	uint8_t buttons = pressedButtons();
	FILL_RESULT(result, buttons, uint8_t)
}
TWI_RPC_SERVER_FUNCTION_NOARGS	(tankIO_server_pressedButtons, TANK_IO_pressedButtons, uint8_t)

// ==
// Local converting functions
// ==

PLedGroup toLedGroup(TankIoLeds leds) {
	switch (leds) {
		case LEDS_ALL:
			return AllLeds;
		case LEDS_LEFT:
			return LeftLeds;
		case LEDS_RIGHT:
			return RightLeds;
		case LEDS_MIDDLE:
			return MiddleLeds;
		case LEDS_RED:
			return RedLeds;
		case LEDS_YELLOW:
			return YellowLeds;
		case LEDS_WHITE:
			return WhiteLeds;
		case LEDS_GREEN:
			return GreenLeds;
		default:
			return NULL;
	}
}

PLed toLed(SingleLed led) {
	return toLedGroup(led.leds)->leds[led.index];
}

// ==
// **Led
// ==

void tankIO_server_enableLed(SingleLed *led, uint16_t size) {
	enableLed(toLed(*led));
}
TWI_RPC_SERVER_FUNCTION_VOID	(tankIO_server_enableLed, TANK_IO_enableLed, SingleLed)

void tankIO_server_disableLed(SingleLed *led, uint16_t size) {
	disableLed(toLed(*led));
}
TWI_RPC_SERVER_FUNCTION_VOID	(tankIO_server_disableLed, TANK_IO_disableLed, SingleLed)

void tankIO_server_setLed(SetLedParam *param, uint16_t size) {
	setLed(toLed(param->led), param->value);
}
TWI_RPC_SERVER_FUNCTION_VOID	(tankIO_server_setLed, TANK_IO_setLed, SetLedParam)

// ==
// **Leds
// ==

void tankIO_server_setLeds(MaskedLeds *leds, uint16_t size) {
	setLeds(toLedGroup(leds->leds), leds->mask);
}
TWI_RPC_SERVER_FUNCTION_VOID	(tankIO_server_setLeds, TANK_IO_setLeds, MaskedLeds)

void tankIO_server_enableLeds(TankIoLeds *leds, uint16_t size) {
	enableLeds(toLedGroup(*leds));
}
TWI_RPC_SERVER_FUNCTION_VOID	(tankIO_server_enableLeds, TANK_IO_enableLeds, TankIoLeds)

void tankIO_server_disableLeds(TankIoLeds *leds, uint16_t size) {
	disableLeds(toLedGroup(*leds));
}
TWI_RPC_SERVER_FUNCTION_VOID	(tankIO_server_disableLeds, TANK_IO_disableLeds, TankIoLeds)

// ==
// blink**
// ==

void tankIO_server_blinkLed(BlinkLedParam *param, uint16_t size) {
	blinkLed(toLed(param->led), param->times);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_blinkLed, TANK_IO_blinkLed, BlinkLedParam)

void tankIO_server_blinkLeds(BlinkLedsParam *param, uint16_t size) {
	blinkLeds(toLedGroup(param->leds.leds), param->leds.mask, param->times);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_blinkLeds, TANK_IO_blinkLeds, BlinkLedsParam)

void tankIO_server_blinkAllLeds(BlinkAllLedsParam *param, uint16_t size) {
	blinkAllLeds(toLedGroup(param->leds), param->times);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_blinkAllLeds, TANK_IO_blinkAllLeds, BlinkAllLedsParam)

// ==
// flash**
// ==

void tankIO_server_flashLed(FlashLedParam *param, uint16_t size) {
	flashLed(toLed(param->led), param->milliseconds);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_flashLed, TANK_IO_flashLed, FlashLedParam)

void tankIO_server_flashLeds(FlashLedsParam *param, uint16_t size) {
	flashLeds(toLedGroup(param->leds.leds), param->leds.mask, param->milliseconds);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_flashLeds, TANK_IO_flashLeds, FlashLedsParam)

void tankIO_server_flashAllLeds(FlashAllLedsParam *param, uint16_t size) {
	flashAllLeds(toLedGroup(param->leds), param->milliseconds);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_flashAllLeds, TANK_IO_flashAllLeds, FlashAllLedsParam)

// ==
// Other
// ==

void tankIO_server_blinkByte(BlinkByteParam *param, uint16_t size) {
	blinkByte(toLedGroup(param->display), toLedGroup(param->notifier), param->data);
}
TWI_RPC_SERVER_FUNCTION_ASYNC_VOID(tankIO_server_blinkByte, TANK_IO_blinkByte, BlinkByteParam)
