# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

include ./system-config.cfg

.PHONY: all
all: clean $(OSIDENTIF).iso

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v4.x-branch-binary --depth=1
	$(MAKE) -C limine

$(OSIDENTIF).iso: limine kernel
	rm -rf iso_root
	mkdir -p iso_root
# Copy these files into the root of the ISO
# - Kernel executable
# - Limine config
# - Limine files
	cp kernel/build/kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
# Make the ISO
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(OSIDENTIF).iso
# Inject the limine bootloader into the ISO
	limine/limine-deploy $(OSIDENTIF).iso
# I recommend leaving below option disabled for debugging
# Remove the iso_root as the ISO has been created
#rm -rf iso_root

.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)
# Clean libs
	rm -rf libs/build
	mkdir libs/build
	rm -rf iso_root

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-x64

#================================#
#            Emulator            #
#================================#

.PHONY: qemu_env
qemu_env : qemu_dir
# Create Virtual HDD with 5GB of storage
	qemu-img create qemu/HDD.img 5G

wipe-emu :
# Delete entire qemu directory
	rm -rf qemu

.PHONY: qemu_dir
qemu_dir : wipe-emu
# Create qemu directory
	mkdir qemu

.PHONY: run
run:
	qemu-system-x86_64 -name $(OSNAME) -M q35 -m 4G -cdrom $(OSIDENTIF).iso -boot d -drive file=qemu/HDD.img,index=0,media=disk,format=raw -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -device intel-hda -device hda-duplex

.PHONY: run-uefi
run-uefi: ovmf-x64
	qemu-system-x86_64 -name $(OSNAME) -M q35 -m 4G -bios ovmf-x64/OVMF.fd -cdrom $(OSIDENTIF).iso -boot d -drive file=qemu/HDD.img,index=0,media=disk,format=raw -device intel-hda -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -device hda-duplex

# UEFI bootloader
ovmf-x64:
	mkdir -p ovmf-x64
	cd ovmf-x64 && curl -o OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && 7z x OVMF-X64.zip

#================================#
#              Libs              #
#================================#

# Get library source files
LIB_SRC_C=$(shell find libs/src -name '*.c')
LIB_SRC_S=$(shell find libs/src -name '*.S')
LIB_SRC_ASM=$(shell find libs/src -name '*.asm')

# Compile them into one variable
SOURCES=\
$(LIB_SRC_C)\
$(LIB_SRC_S)\
$(LIB_SRC_ASM)

# Turn them into a list of object files
LIB_OBJS = $(patsubst libs/src/%,libs/build/%.o,$(SOURCES))

limine.h:
	rm -rf libs/src/include/$@
	curl https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h -o libs/src/include/$@

.PHONY: libs
libs: limine.h $(LIB_OBJS)

# Compilation rules for *.c files.
libs/build/%.c.o: libs/src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
libs/build/%.S.o: libs/src/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
libs/build/%.asm.o: libs/src/%.asm
	mkdir -p $(@D)
	nasm $(NASMFLAGS) $< -o $@

#================================#
#             Kernel             #
#================================#

KERNEL_OBJ=kernel/build/kernel.c.o

.PHONY: kernel
kernel: $(KERNEL)

# Default target.
.PHONY: all
all: 

# Link rules for the final kernel executable.
$(KERNEL): $(KERNEL_OBJ)
	mkdir -p $(@D)
	$(LD) $(KERNEL_OBJ) $(LDFLAGS) -o $@ $(LIB_OBJS)

# Include header dependencies.
-include $(HEADER_DEPS)

# Compilation rules for *.c files.
kernel/build/%.c.o: kernel/src/%.c libs
	mkdir -p $(@D)
	$(CC) -Ilibs/src/include $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
kernel/build/%/S.o: kernel/src/%.S libs
	mkdir -p $(@D)
	$(CC) -Ilibs/src/include $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
kernel/build/%.asm.o: kernel/src/%.asm libs
	mkdir -p $(@D)
	nasm $(NASMFLAGS) $< -o $@

