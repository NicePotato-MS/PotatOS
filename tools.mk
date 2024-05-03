CROS_CC := tools/$(ARCH)/bin/$(ARCH)-elf-gcc
CROS_KERNEL_CFLAGS := -g -O2 -Ikernel/include

CROS_LD := tools/$(ARCH)/bin/$(ARCH)-elf-ld
CROS_LDFLAGS := 

HOST_CC := cc
HOST_CFLAGS := -g -O2 