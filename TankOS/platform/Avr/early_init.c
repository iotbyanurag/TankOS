/*
 * reset_condition.c
 *
 * Created: 28.04.2012 11:05:02
 *  Author: Anton
 */

#include "early_init.h"

uint16_t EEMEM eeprom_reset_counter;

// This counter will not be overwritten by software. Upon a hardware reset,
// the memory section will be cleared to zero, but software resets (jumping to address 0) will keep this intact.
uint16_t software_reset_counter __attribute__ ((section (".noinit")));

// To confirm completed initialization, this will be writton to __initialization_complete_mask when main is entered.
#define INIT_MASK_INITIALIZED 0x5a

byte __initialization_complete_mask = 0;

void boot_completed() {
    __initialization_complete_mask = INIT_MASK_INITIALIZED;
}

uint8_t current_reset_status __attribute__ ((section (".noinit")));
BOOL unclean_reset __attribute__ ((section (".noinit")));

ResetCondition getResetCondition() {
	uint16_t mask = 0;
	uint8_t status = current_reset_status;
	if (status & _BV(PORF)) {
		mask |= PowerOnReset;
	}
	if (status & _BV(WDRF)) {
		mask |= WatchDogReset;
	}
	if (status & _BV(BORF)) {
		mask |= BrownOutReset;
	}
    if (status & _BV(JTRF)) {
		mask |= JtagReset;
	}
    if (status & _BV(EXTRF)) {
		mask |= ExternalReset;
	}
    if (unclean_reset) {
        mask |= UncleanReset;
    }
	if (!mask) mask = OtherReset;
	return mask;
}

void increment_software_reset_counter() {
    software_reset_counter++;
}

void increment_eeprom_reset_counter() {
    uint16_t val = eeprom_read_word(&eeprom_reset_counter);
    val++;
    eeprom_update_word(&eeprom_reset_counter, val);
}

uint16_t getEepromResets() {
    return eeprom_read_word(&eeprom_reset_counter);
}

void init_reset_condition() {
    uint8_t mcu_mirror = MCUSR;
	MCUSR = 0;
    if (mcu_mirror == 0) {
        unclean_reset = TRUE;
    } else {
        unclean_reset = FALSE;
        current_reset_status = mcu_mirror;
    }
}

uint8_t rawResetConditionByte() {
	return current_reset_status;
}

InitStatus getInitStatus() {
    InitStatus status;
    status.initialized = __initialization_complete_mask == INIT_MASK_INITIALIZED;
    status.software_resets = software_reset_counter;
    return status;
}
