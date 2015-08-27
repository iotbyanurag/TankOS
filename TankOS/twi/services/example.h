#ifndef TWI_SERVICE_TEST_H_
#define TWI_SERVICE_TEST_H_

#include <twi/rpc/client_functions.h>

typedef enum {
    TEST_WRITE_VALUE_OPERATION = 240,
    TEST_READ_VALUE_OPERATION,
    TEST_INC_VALUE_OPERATION,
    TEST_RESET_VALUE_OPERATION
} PROTOCOL_EXAMPLE_SERVICE; // Start at 240

TWI_RPC_FUNCTION_VOID(test_write_value, TEST_WRITE_VALUE_OPERATION, int16_t)
TWI_RPC_FUNCTION_NOARGS(test_read_value, TEST_READ_VALUE_OPERATION, int16_t)
TWI_RPC_FUNCTION_NOTIFY(test_increment_value, TEST_INC_VALUE_OPERATION)
TWI_RPC_FUNCTION_NOTIFY(test_reset_value, TEST_RESET_VALUE_OPERATION)

#endif // TWI_SERVICE_TEST_H_
