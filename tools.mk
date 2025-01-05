TOOLS := ~/tools

CROS_CC := $(TOOLS)/cross-compiler/$(ARCH)/bin/$(ARCH)-elf-gcc
CROS_KERNEL_CFLAGS := \
	-Wall \
	-Wextra \
	-Wshadow \
	-Wvla \
	-g \
	-O2 \
	-Ikernel/include

CROS_LD := $(TOOLS)/cross-compiler/$(ARCH)/bin/$(ARCH)-elf-ld
CROS_LDFLAGS := \
    -nostdlib \
    -z max-page-size=0x1000 \
	-gc-sections

ifeq ($(DEBUG), true)
CROS_KERNEL_CFLAGS += -DDEBUG
endif