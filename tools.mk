CROS_CC := tools/bin/$(ARCH)-elf-gcc
CROS_CFLAGS := -O2 -Iinclude
ifeq ($(DEBUG),true)
	CROS_CFLAGS += -g
else
	CROS_CFLAGS += -s
endif

CROS_LD := tools/bin/$(ARCH)-elf-ld
CROS_LDFLAGS := 

HOST_CC := cc
HOST_CFLAGS := -g -O2 