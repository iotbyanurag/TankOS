
LIBRARY := false
project := $(get-basedir)
include $(project)/Dependencies.mk

include Defaults.mk
include Tank-Shared/DefaultTankSymbols.mk

USE_TWI := false
USE_SCHEDULER := true
USE_PROCESS_EXT := true

include Main.mk
