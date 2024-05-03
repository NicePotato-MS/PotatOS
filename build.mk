# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2023

.SILENT:

override MAKEFLAGS += -rR

ARCH ?= x86_64
DEBUG ?= true

KRNLIB = kernel/lib

KERNEL_SRC_C := 
KERNEL_MODULES :=

include tools.mk
-include modules.mk
-include arch/$(ARCH)/archinc.mk

KERNEL_SRC_C += $(foreach module,$(KERNEL_MODULES),$(shell find $(module) -type f -name '*.c'))

KERNEL_OBJ := $(addprefix kobj/,$(KERNEL_SRC_C:.c=.c.o) $(KERNEL_SRC_S:.S=.S.o))
KERNEL_DEP := $(addprefix kobj/,$(KERNEL_SRC_C:.c=.c.d) $(KERNEL_SRC_S:.S=.S.d))

.PHONY: iso
iso: errorcheck fullclean kbin/kernel.elf
	rm -rf isoroot
	mkdir isoroot
	cp kbin/kernel.elf limine.cfg limine/limine-bios.sys \
    	limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isoroot/
	mkdir -p isoroot/EFI/BOOT
	if [ "$(DEBUG)" = "false" ]; then \
        strip kbin/kernel.elf; \
    fi

	
	if [ "$(ARCH)" = "x86_64" ] || [ "$(ARCH)" = "i386" ]; then \
        cp limine/BOOTX64.EFI isoroot/EFI/BOOT/; \
		cp limine/BOOTIA32.EFI isoroot/EFI/BOOT/; \
    elif [ "$(ARCH)" = "aarch64" ]; then \
        cp limine/BOOTAA64.EFI isoroot/EFI/BOOT/; \
	elif [ "$(ARCH)" = "riscv64" ]; then \
        cp limine/BOOTRISCV64.EFI isoroot/EFI/BOOT/; \
    fi

	mkdir -p build
	xorriso -as mkisofs -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isoroot -o build/potatos-$(ARCH).iso

	./limine/limine bios-install build/potatos-$(ARCH).iso
	
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
	if ["$(DEBUG)" = "false"]; then \
        strip bin/kernel.elf; \
    fi

# Compilation rules for *.c files.
kobj/%.c.o: %.c
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@
	if [ "$(OUT_ASM)" = "true" ]; then \
		$(CROS_CC) -S -fverbose-asm $(CROS_KERNEL_CFLAGS) -c $< -o $@.S; \
	fi

# Compilation rules for *.S files.
kobj/%.S.o: %.S
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@