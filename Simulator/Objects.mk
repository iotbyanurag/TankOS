
include Tank-Shared/KernelObjects1.mk

# Simulate Tank IO
objects += \
    $(BUILD_Tank-IO)/tank_led.kernel.o \
    $(BUILD_Tank-IO)/tank_button.kernel.o \
    $(BUILD_TankOS)/platform/platform_Avr/avr_atmega1284p/pin_change.kernel.o \
    $(BUILD_Tank-IO)/tank_button_interrupts.kernel.o

ifeq ($(USE_TWI), true)
    objects += \
        $(BUILD_TankOS)/platform/platform_Avr/avr_atmega1284p/twi/slave.kernel.o \
        $(BUILD_TankOS)/twi/rpc/server_handler_functions.kernel.o \
        $(BUILD_TankOS)/twi/services/hardware.kernel.o \
        $(BUILD_TankOS)/twi/services/example.kernel.o \
        $(BUILD_Simulator)/simulator_twi.kernel.o

    ifeq ($(USE_BUFFER_STDOUT), true)
        objects += $(BUILD_TankOS)/twi/services/buffer_stdout.kernel.o
    endif
endif

include Tank-Shared/KernelObjects2.mk
