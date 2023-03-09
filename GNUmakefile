# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

# Filename of OS and ISO stuff
OSIDENTIF=potatos
# Actual name of OS (Supports spaces and stuff)
OSNAME="PotatOS"

.PHONY: all
all: $(OSIDENTIF).iso

.PHONY: all-hdd
all-hdd: $(OSIDENTIF).hdd

.PHONY: qemu_env
qemu_env : qemu_dir
#Create Virtual HDD with 5GB of storage
	qemu-img create qemu/HDD.img 5G

wipe-emu :
	rm -rf qemu

.PHONY: qemu_dir
qemu_dir : wipe-emu
	mkdir qemu

.PHONY: run
run: $(OSIDENTIF).iso
	qemu-system-x86_64 -name $(OSNAME) -M q35 -m 4G -cdrom $(OSIDENTIF).iso -boot d -drive file=qemu/HDD.img,index=0,media=disk,format=raw -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -device intel-hda -device hda-duplex

.PHONY: run-uefi
run-uefi: ovmf-x64 $(OSIDENTIF).iso
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
	cp kernel/kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(OSIDENTIF).iso
	limine/limine-deploy $(OSIDENTIF).iso
	rm -rf iso_root

$(OSIDENTIF).hdd: limine kernel
	rm -f $(OSIDENTIF).hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(OSIDENTIF).hdd
	parted -s $(OSIDENTIF).hdd mklabel gpt
	parted -s $(OSIDENTIF).hdd mkpart ESP fat32 2048s 100%
	parted -s $(OSIDENTIF).hdd set 1 esp on
	limine/limine-deploy $(OSIDENTIF).hdd
	sudo losetup -Pf --show $(OSIDENTIF).hdd >loopback_dev
	sudo mkfs.fat -F 32 `cat loopback_dev`p1
	mkdir -p img_mount
	sudo mount `cat loopback_dev`p1 img_mount
	sudo mkdir -p img_mount/EFI/BOOT
	sudo cp -v kernel/kernel.elf limine.cfg limine/limine.sys img_mount/
	sudo cp -v limine/BOOTX64.EFI img_mount/EFI/BOOT/
	sync
	sudo umount img_mount
	sudo losetup -d `cat loopback_dev`
	rm -rf loopback_dev img_mount

.PHONY: clean
clean:
	rm -rf iso_root $(OSIDENTIF).iso $(OSIDENTIF).hdd
	$(MAKE) -C kernel clean

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-x64
	$(MAKE) -C kernel distclean
