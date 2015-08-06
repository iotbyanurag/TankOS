#ifndef PORT_H_
#define PORT_H_

#include <tank_os_common.h>

DEFINE_HANDLE(Port)
DEFINE_HANDLE(Pin)

Port newPort(volatile uint8_t *port, volatile uint8_t *pin, volatile uint8_t *ddr);
Port destroyPort(Port port);
Pin newPin(Port port, uint8_t pinNumber);
Pin destroyPin(Pin pin);

void setPortOutput(Port port);
void setPortInput(Port port);

void setPinOutput(Pin pin);
void setPinInput(Pin pin);

void writePort(Port port, uint8_t value);
uint8_t readPort(Port port);

void writePin(Pin pin, BOOL value);
void setPinOne(Pin pin);
void setPinZero(Pin pin);

BOOL readPin(Pin pin);

// == Pin configuration
// Each pin can have a linked list of configuration data attached. Every entry
// describes a possible usage of the pin, like analog input, pwm output, etc.
// To prevent duplicate use of a pin, an occupation flag is set once a certain config data is extracted.
// External modules can allocate and register config data and later read and evaluate it.

typedef struct ConfigData {
    byte data[4];
} ConfigData;

typedef enum {
    PinNoOccupation = 0,
    PinGPIO = 1 // Available for every pin to be used as general IO pin.
} PinConfigTag;

BOOL registerPinConfig(Pin pin, PinConfigTag tag, ConfigData configData); // return FALSE if configuration with that tag is already present or if malloc failed.
BOOL occupyPin(Pin pin, PinConfigTag tag); // return FALSE if pin is already occupied or requested configuration is not available.
BOOL occupyPinDirectly(Pin pin, PinConfigTag tag, ConfigData configData); // Occupy without registering. return FALSE if pin already occupied.
PinConfigTag pinOccupation(Pin pin);
ConfigData pinConfigData(Pin pin, PinConfigTag tag); // return {0} if pin not occupied or is occupied by different configuration than the given tag.
BOOL deOccupyPin(Pin pin, PinConfigTag tag); // return FALSE, if pin was not occupied by tag.

#endif
