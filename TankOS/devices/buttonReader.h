#ifndef READ_BUTTONS_LOOP_
#define READ_BUTTONS_LOOP_

#include <tank_os_common.h>
#include <devices/button.h>

DEFINE_HANDLE(ButtonReader);

typedef void (*ButtonCallbackFunction)(Button button);

ButtonReader newButtonReader(
		Button *buttonArray, uint8_t buttonCount,
		ButtonCallbackFunction button_pressed, ButtonCallbackFunction button_released);

ButtonReader destroyButtonReader(ButtonReader reader);

// Enter an eternal loop, reading the status of the given buttons
// and invoking the weak function button_pressed() whenever a button
// is pressed. Holding buttons are ignored.
void startButtonReader(ButtonReader reader);
void stopButtonReader(ButtonReader reader);

#endif