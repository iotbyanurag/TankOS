#ifndef twi_driver_H_
#define twi_driver_H_

#include <tank_os_common.h>
#include <devices/port.h>

// Since the twi-lines are pulled up, this byte is transmitted by default,
// even when the Slave switches to not-addressed-mode (stops sending/receiving actively).
#define TwiIllegalByte 0xff

// Only the seven MSB are valid address bits. The LSB will be replaced.
typedef struct TWIDevice {
	byte address;
} TWIDevice;

#define TWI_SLA_WRITE(dev) ((dev.address << 1) & ~_BV(0))
#define TWI_SLA_READ(dev) ((dev.address << 1) | _BV(0))

extern const TWIDevice TWIBroadcast;

typedef struct TWIBuffer {
	byte *data;
	uint16_t size;
} TWIBuffer;

#define EmptyBuffer ((TWIBuffer) { 0, 0 })
#define StaticEmptyBuffer { 0, 0 }

typedef enum TwiError {
	TWI_No_Error,

	TWI_No_Info_Interrupt, // TWI-Interrupt happened without any special condition.
	TWI_Bus_Error,
	TWI_Illegal_Status, // The TWI-Status-Register contained an unrecognized code.

	TWI_SlaveAddress_NoAck,
	TWI_Arbitration_Lost,

	TWI_Master_TooMuchDataTransmitted,
	// Cannot find out, if Master Transmitted not enough data, because master just terminates with a STOP signal.
	// Master Receive mode is ONLY left, when master issues a STOP, which happens after he received
	//		EXACTLY the amount of bytes specified in the prepared buffer.
	//		--> If slave already stopped transmitting earlier, we continue receiving 0xff.
	//		--> If slave wanted to transmit more, it has no chance of telling us.
	// Actually, with our own TWI Slave mode implementation, even the TWI_Master_TooMuchDataTransmitted
	// state cannot be detected, because the slave continues acknowledging the data even
	// if it is not really reading it anymore.

	TWI_Slave_NotEnoughDataTransmitted,
	TWI_Slave_TooMuchDataTransmitted,
	TWI_Slave_NotEnoughDataReceived,
	TWI_Slave_TooMuchDataReceived

} TwiError;

// The whole TWI-library is not reentrant! should be used by only one process
// or strictly atomic (disabled interrupts).
// Can be looped on until the completion of current operation.
extern volatile BOOL twi_running;

// Can be checked for success of the operation afterwards.
// Read-only.
extern TwiError twi_error;

// This is used for the communication between the TWI interrupt handler
// and the functions actually handling the interrupt (twi_handle, twi_handle_slave)
typedef struct TwiHandlerStatus {
	BOOL handlerFinished; // Causes twi_running to be reset.
	byte controlRegister;
} TwiHandlerStatus;

// Wait for the current TWI operation to finish.
// With concurrency, and with two operations quickly following each other,
// the end of the current operation might be skipped.
// Optionally, define non-zero msWaitEachIteration to add a delay between two
// completion checks.
void twiWaitForCompletion();

typedef uint16_t TwiStatus;

// Usage-tag for occupyPin
#define PinTwiIO 8
extern Pin twiDataPin, twiClockPin;
BOOL occupyTwiPins(Pin dataPin, Pin clockPin);

#endif
