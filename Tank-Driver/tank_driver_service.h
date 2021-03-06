#ifndef _TANK_DRIVER_SERVICE_TWI_
#define _TANK_DRIVER_SERVICE_TWI_

#include <twi/rpc/client_functions.h>
#include "tank_motor.h"

typedef enum {
    TANK_DRIVER_SET_MOTOR = 30,
    TANK_DRIVER_GET_MOTOR_SPEED,
    TANK_DRIVER_EMERGENCY_STOP,
    TANK_DRIVER_BATTERY_VOLTAGE,
    TANK_DRIVER_MOTOR_VOLTAGE
} PROTOCOL_TANK_DRIVER_SERVICE; // Start at 30

typedef enum {
	TANK_MOTOR_LEFT,
	TANK_MOTOR_RIGHT,
	TANK_MOTOR_MAX,
	TANK_MOTOR_INVALID
} TankMotorVoltageSelection;

typedef struct MotorStatus {
    uint16_t motorNum; // enum TankMotorNum
    uint16_t direction; // enum MotorDirection
    uint16_t speed;
} MotorStatus;

TWI_RPC_FUNCTION_VOID(tank_driver_set_motor, TANK_DRIVER_SET_MOTOR, MotorStatus)
TWI_RPC_FUNCTION(tank_driver_get_motor_speed, TANK_DRIVER_GET_MOTOR_SPEED, uint16_t /* enum TankMotorNum */, MotorStatus)
TWI_RPC_FUNCTION_NOTIFY(tank_driver_emergency_stop, TANK_DRIVER_EMERGENCY_STOP)
TWI_RPC_FUNCTION_NOARGS(tank_driver_get_battery_voltage, TANK_DRIVER_BATTERY_VOLTAGE, uint16_t)
TWI_RPC_FUNCTION(tank_driver_get_motor_voltage, TANK_DRIVER_MOTOR_VOLTAGE, uint16_t /* enum TankMotorVoltageSelection */, uint16_t)

#endif // _TANK_DRIVER_SERVICE_TWI_
