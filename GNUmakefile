# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

include ./system-config.cfg

.PHONY: all
all: $(OSIDENTIF).iso

.PHONY: all-hdd
all-hdd: $(OSIDENTIF).hdd

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

ovmf-x64:
	mkdir -p ovmf-x64
	cd ovmf-x64 && curl -o OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && 7z x OVMF-X64.zip

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v4.x-branch-binary --depth=1
	$(MAKE) -C limine

.PHONY: kernel
kernel:
	$(MAKE) -C kernel

$(OSIDENTIF).iso: limine kernel
	rm -rf iso_root
	mkdir -p iso_root
# Copy these files into the root of the ISO
# - Kernel executable
# - Limine config
# - Limine files
	cp kernel/kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
# Make the ISO
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(OSIDENTIF).iso
# Inject the limine bootloader into the ISO
	limine/limine-deploy $(OSIDENTIF).iso
# Clean up library compilations
	$(MAKE) -C libs clean
# Remove the iso_root as the ISO has been created
#rm -rf iso_root

.PHONY: clean
clean:
	rm -rf iso_root $(OSIDENTIF).iso $(OSIDENTIF).hdd
	$(MAKE) -C kernel clean

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-x64
	$(MAKE) -C kernel distclean
