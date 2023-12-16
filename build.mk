# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2023

.SILENT:

override MAKEFLAGS += -rR

ARCH ?= x86_64
DEBUG ?= true

KERNEL_SRC_C := $(shell find kernel -type f -name '*.c')

include tools.mk
-include arch/$(ARCH)/archinc.mk

KERNEL_OBJ := $(addprefix obj/,$(KERNEL_SRC_C:.c=.c.o) $(KERNEL_SRC_S:.S=.S.o))
KERNEL_DEP := $(addprefix obj/,$(KERNEL_SRC_C:.c=.c.d) $(KERNEL_SRC_S:.S=.S.d))

.PHONY: iso
iso: errorcheck fullclean bin/kernel.elf
	rm -rf isoroot
	mkdir isoroot
	cp bin/kernel.elf limine.cfg limine/limine-bios.sys \
    	limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isoroot/
	mkdir -p isoroot/EFI/BOOT
	if [ "$(DEBUG)" = "false" ]; then \
        strip bin/kernel.elf; \
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

# Link rules for the final kernel executable.
bin/kernel.elf: $(KERNEL_OBJ)
	mkdir -p "$$(dirname $@)"
	$(CROS_LD) $(KERNEL_OBJ) $(CROS_LDFLAGS) -o $@
	if [ "$(DEBUG)" = "false" ]; then \
        strip bin/kernel.elf; \
    fi

# Compilation rules for *.c files.
obj/%.c.o: %.c
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: %.S
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_KERNEL_CFLAGS) -c $< -o $@