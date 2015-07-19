
#include <kernel/kernel_init.h>
#include <kernel/twi/driver/driver.h>

const byte TwiSlaveAddress = TANK_IO_ADDRESS;

void init_rpc_server_buffer(); // Tank-Shared/twi.kernel.c
KERNEL_INIT(init_rpc_server_buffer)
