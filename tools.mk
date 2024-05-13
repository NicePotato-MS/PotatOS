CROS_CC := tools/$(ARCH)/bin/$(ARCH)-elf-gcc
CROS_KERNEL_CFLAGS := \
	-Wall \
	-Wextra \
	-Wshadow \
	-Wvla \
	-g \
	-O2 \
	-Ikernel/include

CROS_LD := tools/$(ARCH)/bin/$(ARCH)-elf-ld
CROS_LDFLAGS := \
    -nostdlib \
    -z max-page-size=0x1000 \
	-gc-sections

HOST_CC := cc
HOST_CFLAGS := -g -O2 