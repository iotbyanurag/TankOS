/*
 * hardware_reset.c
 *
 * Created: 28.04.2012 11:05:02
 *  Author: Anton
 */ 

#include "../anton_std.h"
#include "hardware_reset.h"
#include <avr/wdt.h>

void HARDWARE_RESET() {
	cli();
	// Clear this bit to enable WDE. Should already be 0, but make sure.
	MCUSR &= ~_BV(WDRF);
	wdt_enable(WDTO_15MS); // Shortest possilbe value.
	while(1) ;
}