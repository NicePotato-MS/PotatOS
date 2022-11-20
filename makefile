# Windows with Visual Studio Supported, however make clean only works with linux, other's may work, but idk

# requires
# -UnxUtils
# -qemu
# -MinGW gcc tools
# -nasm



# Automatically generate lists of sources using wildcards .
CFLAGS=-O2 -g -Wall -Wextra
C_SOURCES = $(wildcard src/kernel/*.c src/drivers/*.c)
HEADERS = $(wildcard src/kernel/*.h src/drivers/*.h)
# TODO : Make sources dep on all header files .
# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o}
# Default build target



qemu_env : create_qemu_dir
#Create Virtual HDD with 5GB of storage
	@qemu-img create qemu/HDD.img 5G

create_qemu_dir : wipe
	@mkdir qemu

wipe :
	@if exist qemu rmdir /s /q qemu

all : os-image
# Run bochs to simulate booting of our code .
run : all
#Open qemu emulator with 4GB of RAM
	@qemu-system-i386 -m 1M -drive file=qemu/HDD.img,index=0,media=disk,format=raw -drive file=images/os-image.img,index=0,if=floppy,format=raw -soundhw hda

# This is the actual disk image that the computer loads
# which is the combination of our compiled bootsector and kernel
os-image : src/boot/boot_sect.bin kernel.bin
	cat $^ >> images/os-image.img
# This builds the binary of our kernel from two object files :
# - the kernel_entry , which jumps to main () in our kernel
# - the compiled C kernel
kernel.bin : src/kernel/kernel_entry.o ${OBJ}
	ld -T NUL -o kernel.tmp -Ttext 0x1000 $^
	objcopy -O binary -j .text  kernel.tmp kernel.bin 
# Generic rule for compiling C code to an object file
# For simplicity , we C files depend on all header files .
%.o : %.c ${HEADERS}
	gcc $(CFLAGS) -ffreestanding -c $< -o $@
# Assemble the kernel_entry .
%.o : %.asm
	nasm $< -f elf -o $@
%.bin : %.asm
	nasm $< -f bin -I '../../16 bit /' -o $@
clean :
	rm -fr *.bin *.dis *.o os-image *.tmp && rm -fr src/kernel/*.o src/boot/*.bin src/drivers/*.o && rm -fr images/*

	