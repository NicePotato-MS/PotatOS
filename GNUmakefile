# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2021-2023

.SILENT:

override MAKEFLAGS += -rR

ARCH ?= x86_64
DEBUG ?= true

# This rule should come first
.PHONY: iso
iso: errorcheck fullclean bin/kernel.elf
	rm -rf isoroot
	mkdir isoroot
	cp bin/kernel.elf limine.cfg limine/limine-bios.sys \
    	limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isoroot/
	mkdir -p isoroot/EFI/BOOT
	cp limine/BOOTX64.EFI isoroot/EFI/BOOT/
	cp limine/BOOTIA32.EFI isoroot/EFI/BOOT/

	rm -rf build
	mkdir build
	xorriso -as mkisofs -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isoroot -o build/potatos.iso

	./limine/limine bios-install build/potatos.iso
	
.PHONY: run
run: iso
	qemu-system-x86_64 -cdrom build/potatos.iso -m 512M

KERNEL_SRC_C := $(shell find kernel -type f -name '*.c')
KERNEL_SRC_C += $(shell find lib -type f -name '*.c')
KERNEL_SRC_S := $(shell find kernel -type f -name '*.S')
KERNEL_SRC_S += $(shell find lib -type f -name '*.S')

include tools.mk
-include arch/$(ARCH)/archinc.mk

KERNEL_OBJ := $(addprefix obj/,$(KERNEL_SRC_C:.c=.c.o) $(KERNEL_SRC_S:.S=.S.o))
KERNEL_DEP := $(addprefix obj/,$(KERNEL_SRC_C:.c=.c.d) $(KERNEL_SRC_S:.S=.S.d))

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
	$(CROS_CC) $(CROS_CFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: %.S
	mkdir -p "$$(dirname $@)"
	$(CROS_CC) $(CROS_CFLAGS) -c $< -o $@