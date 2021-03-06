#define RPC_CLIENT_IMPLEMENTATION
#include "tank_arm_service.h"

int tank_arm_position_format(ClientResultPrinter print, void *results, uint16_t results_length) {
    if (results_length != sizeof(pos_t)) return 0;
    pos_t pos = * (pos_t*) results;
    return print("Motor position: %i", pos);
}

int tank_arm_get_max_format(ClientResultPrinter print, void *results, uint16_t results_length) {
    if (results_length != sizeof(speed_t)) return 0;
    speed_t freq = * (speed_t*) results;
    return print("Max motor frequency: %i", freq);
}

int tank_arm_set_max_format(ClientResultPrinter print, void *results, uint16_t results_length) {
    if (results_length != sizeof(uint16_t)) return 0;
    BOOL success = * (BOOL*) results;
    if (success)
        return print("Setting frequency successful");
    else
        return print("Frequency was too high!");
}

int tank_arm_state_format(ClientResultPrinter print, void *results, uint16_t results_length) {
    if (results_length != sizeof(TankArmState)) return 0;
    TankArmState *state = (TankArmState*) results;
    int res = print("Encoder %i, Motor %i", state->encoderPos, state->motorPos);
    if (state->backSensor || state->frontSensor) {
        res += print(" [");
        if (state->backSensor) res += print("BACK");
        if (state->frontSensor) res += print("FRONT");
        res += print("]");
    }
    print(", Moving to %i ", state->targetPos);
    res += print("(");
    switch ((TankArmCalibration) state->calibration) {
        case NotCalibrated:
            res += print("not calibrated");
            break;
        case Calibrating:
            res += print("calibrating");
            break;
        case Calibrated:
            res += print("calibrated");
            break;
        default:
            res += print("illegal calibration value: %i", state->calibration);
    }
    res += print(")");
    if (state->encoder_error.errors > 0 || state->encoder_error.errorMask != 0) {
        res += print(" %i encoder errors: 0x%.2x", state->encoder_error.errors, state->encoder_error.errorMask);
    }
    return res;
}
