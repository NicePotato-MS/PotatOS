# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2023

.SILENT:

override MAKEFLAGS += -rR

ARCH ?= x86_64
DEBUG ?= true
OUT_ASM ?= false
LIMINE_CUSTOM ?= false

KRNLIB = kernel/lib

KERNEL_SRC_C := 
KERNEL_MODULES :=

include tools.mk
-include modules.mk
-include arch/$(ARCH)/archinc.mk

KERNEL_SRC_C += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.c'))
KERNEL_SRC_CPP += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.cpp'))
KERNEL_SRC_S += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.S'))

KERNEL_OBJ := $(addprefix kobj/,$(KERNEL_SRC_C:.c=.c.o) $(KERNEL_SRC_S:.S=.S.o) $(KERNEL_SRC_CPP:.cpp=.cpp.o))
KERNEL_DEP := $(addprefix kobj/,$(KERNEL_SRC_C:.c=.c.d) $(KERNEL_SRC_S:.S=.S.d) $(KERNEL_SRC_CPP:.cpp=.cpp.o))

ifeq ($(LIMINE_CUSTOM),"false")
LIMINE := limine
else
LIMINE := limine-test/limine/bin
endif

.PHONY: iso
iso: errorcheck fullclean kbin/kernel.elf
	rm -rf isoroot
	mkdir isoroot
	cp kbin/kernel.elf limine.cfg $(LIMINE)/limine-bios.sys \
    	$(LIMINE)/limine-bios-cd.bin $(LIMINE)/limine-uefi-cd.bin isoroot/
	mkdir -p isoroot/EFI/BOOT
	if [ "$(DEBUG)" = "false" ]; then \
        strip kbin/kernel.elf; \
    fi

	
	if [ "$(ARCH)" = "x86_64" ] || [ "$(ARCH)" = "i686" ]; then \
        cp $(LIMINE)/BOOTX64.EFI isoroot/EFI/BOOT/; \
		cp $(LIMINE)/BOOTIA32.EFI isoroot/EFI/BOOT/; \
    elif [ "$(ARCH)" = "aarch64" ]; then \
        cp $(LIMINE)/BOOTAA64.EFI isoroot/EFI/BOOT/; \
	elif [ "$(ARCH)" = "riscv64" ]; then \
        cp $(LIMINE)/BOOTRISCV64.EFI isoroot/EFI/BOOT/; \
    fi

	mkdir -p build
	xorriso -as mkisofs -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isoroot -o build/potatos-$(ARCH).iso

	./$(LIMINE)/limine bios-install build/potatos-$(ARCH).iso
	
errorcheck:
	if [ ! -d "arch/$(ARCH)" ]; then \
        echo "Fatal: Architecture '$(ARCH)' is not available"; \
        exit 1; \
    fi

.PHONY: fullclean
fullclean:
	rm -rf bin
	rm -rf obj
	rm -rf kbin
	rm -rf kobj

# Link rules for the final kernel executable.
kbin/kernel.elf: $(KERNEL_OBJ)
	mkdir -p "$$(dirname $@)"
	$(CROS_LD) $(KERNEL_OBJ) $(CROS_LDFLAGS) -o $@
	if [ "$(DEBUG)" = "false" ]; then \
        strip bin/kernel.elf; \
    fi

kobj/%.c.o: %.c
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@
	if [ "$(OUT_ASM)" = "true" ]; then \
		$(CROS_CC) -S -fverbose-asm $(CROS_KERNEL_CFLAGS) -c $< -o $@.S; \
	fi

kobj/%.cpp.o: %.cpp
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@
	if [ "$(OUT_ASM)" = "true" ]; then \
		$(CROS_CC) -S -fverbose-asm $(CROS_KERNEL_CFLAGS) -c $< -o $@.S; \
	fi

kobj/%.S.o: %.S
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@