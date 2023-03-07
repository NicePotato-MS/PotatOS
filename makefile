qemu_env : create_qemu_dir
#Create Virtual HDD with 5GB of storage
	@qemu-img create qemu/HDD.img 5G

create_qemu_dir : wipe
	@mkdir qemu

wipe :
	@rm -rf qemu

run :
#Open qemu emulator with 4GB of RAM
	@qemu-system-i386 -name PotatOS -m 4096 -cdrom potatos.iso -drive file=qemu/HDD.img,index=0,media=disk,format=raw -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -device intel-hda -device hda-duplex
	