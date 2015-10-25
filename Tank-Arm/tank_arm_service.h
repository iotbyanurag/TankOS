#ifndef _TANK_ARM_SERVICE_TWI_
#define _TANK_ARM_SERVICE_TWI_

#include <twi/rpc/client_functions.h>
#include "tank_arm_motor.h"

typedef enum {
    TANK_ARM_ROTATE = 40,
    TANK_ARM_STEP = 41,
    TANK_ARM_STOP = 42,
    TANK_ARM_POSITION = 43,
    TANK_ARM_ENABLE = 44,
    TANK_ARM_DISABLE = 45,
    TANK_ARM_SET_MAX = 46,
    TANK_ARM_GET_MAX = 47,
    TANK_ARM_SET_TIMER = 48,
    TANK_ARM_SET_DELAY = 49
} PROTOCOL_TANK_ARM_SERVICE; // Start at 40

TWI_RPC_FUNCTION_VOID(tank_arm_rotate, TANK_ARM_ROTATE, uint16_t) // enum MotorDirection
TWI_RPC_FUNCTION_VOID(tank_arm_step, TANK_ARM_STEP, pos_t)
TWI_RPC_FUNCTION_NOTIFY(tank_arm_stop, TANK_ARM_STOP)
TWI_RPC_FUNCTION_NOARGS(tank_arm_position, TANK_ARM_POSITION, pos_t)
TWI_RPC_FUNCTION_NOTIFY(tank_arm_enable, TANK_ARM_ENABLE)
TWI_RPC_FUNCTION_NOTIFY(tank_arm_disable, TANK_ARM_DISABLE)
TWI_RPC_FUNCTION(tank_arm_set_max, TANK_ARM_SET_MAX, freq_t, uint16_t) // enum BOOL
TWI_RPC_FUNCTION_NOARGS(tank_arm_get_max, TANK_ARM_GET_MAX, freq_t)
TWI_RPC_FUNCTION_VOID(tank_arm_set_timer, TANK_ARM_SET_TIMER, uint16_t)
TWI_RPC_FUNCTION_VOID(tank_arm_set_delay, TANK_ARM_SET_DELAY, uint16_t) // enum StepMotorStepDelay

#endif // _TANK_ARM_SERVICE_TWI_
