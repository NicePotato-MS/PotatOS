# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2025

# this needs to be redone

.SILENT:

override MAKEFLAGS += -rR

ARCH ?= x86_64
DEBUG ?= true
OUT_ASM ?= false

KRNLIB = kernel/lib

KERNEL_SRC_C := 
KERNEL_MODULES :=

include tools.mk
-include modules.mk
-include arch/$(ARCH)/archinc.mk

KERNEL_SRC_C += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.c'))
KERNEL_SRC_CPP += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.cpp'))
KERNEL_SRC_S += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.S'))

KERNEL_OBJ := $(addprefix build/kobj/,$(KERNEL_SRC_C:.c=.c.o) $(KERNEL_SRC_S:.S=.S.o) $(KERNEL_SRC_CPP:.cpp=.cpp.o))
KERNEL_DEP := $(addprefix build/kobj/,$(KERNEL_SRC_C:.c=.c.d) $(KERNEL_SRC_S:.S=.S.d) $(KERNEL_SRC_CPP:.cpp=.cpp.o))


VERSION_FILE := version.json
VERSION := $(shell python3 version.py $(VERSION_FILE))

ifneq ($(word 1, $(VERSION)), ver)
$(error Version error thingie the dev is too lazy to handle this rn sorry !!!)
endif

VERSION_MAJOR = $(word 2, $(VERSION))
VERSION_MINOR = $(word 3, $(VERSION))
VERSION_PATCH = $(word 4, $(VERSION))
VERSION_BUILD = $(word 5, $(VERSION))

VERSION_DESCRIPTION := $(shell printf "%08u" $(VERSION_BUILD))$(VERSION_DESCRIPTION)
ifeq ($(DEBUG), true)
VERSION_DESCRIPTION := $(VERSION_DESCRIPTION)-debug
endif

VERSION_NAME := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)-$(VERSION_DESCRIPTION)

$(info Version $(VERSION_NAME))

CROS_KERNEL_CFLAGS += -DVERSION_MAJOR=$(VERSION_MAJOR) -DVERSION_MINOR=$(VERSION_MINOR) \
	-DVERSION_PATCH=$(VERSION_PATCH) -DVERSION_BUILD=$(VERSION_BUILD) \
	-DVERSION_DESCRIPTION="\"$(VERSION_DESCRIPTION)\""

ISO_NAME := potatos-$(ARCH)-$(VERSION_NAME).iso

.PHONY: iso
iso: errorcheck fullclean build/kbin/kernel.elf
	rm -rf build/isoroot
	rm -rf build/*.iso
	mkdir build/isoroot
	cp build/kbin/kernel.elf limine.conf limine/limine-bios.sys \
    	limine/limine-bios-cd.bin limine/limine-uefi-cd.bin build/isoroot/
	mkdir -p build/isoroot/EFI/BOOT

	
	if [ "$(ARCH)" = "x86_64" ] || [ "$(ARCH)" = "i686" ]; then \
        cp limine/BOOTX64.EFI build/isoroot/EFI/BOOT/; \
		cp limine/BOOTIA32.EFI build/isoroot/EFI/BOOT/; \
    elif [ "$(ARCH)" = "aarch64" ]; then \
        cp limine/BOOTAA64.EFI build/isoroot/EFI/BOOT/; \
	elif [ "$(ARCH)" = "riscv64" ]; then \
        cp limine/BOOTRISCV64.EFI build/isoroot/EFI/BOOT/; \
    fi

	mkdir -p build
	xorriso -as mkisofs -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        build/isoroot -o build/$(ISO_NAME)

	limine bios-install build/$(ISO_NAME)
	
errorcheck:
	if [ ! -d "arch/$(ARCH)" ]; then \
        echo "Fatal: Architecture '$(ARCH)' is not available"; \
        exit 1; \
    fi

.PHONY: fullclean
fullclean:
	rm -rf build/kbin
	rm -rf build/kobj

build/kbin/kernel.elf: $(KERNEL_OBJ)
	mkdir -p "$$(dirname $@)"
	$(CROS_LD) $(KERNEL_OBJ) $(CROS_LDFLAGS) -o $@
	if [ "$(DEBUG)" = "false" ]; then \
        strip build/kbin/kernel.elf; \
    fi

build/kobj/%.c.o: %.c
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@
	if [ "$(OUT_ASM)" = "true" ]; then \
		$(CROS_CC) -S -fverbose-asm $(CROS_KERNEL_CFLAGS) -c $< -o $@.S; \
	fi

build/kobj/%.cpp.o: %.cpp
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@
	if [ "$(OUT_ASM)" = "true" ]; then \
		$(CROS_CC) -S -fverbose-asm $(CROS_KERNEL_CFLAGS) -c $< -o $@.S; \
	fi

build/kobj/%.S.o: %.S
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@