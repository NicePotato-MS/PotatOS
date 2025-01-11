CROS_KERNEL_CFLAGS += \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m64 \
    -march=x86-64 \
	-mgeneral-regs-only \
    -mno-red-zone \
    -Ilimine \
	-Ikernel/x86/include \
	-Iarch/$(ARCH)/include

CROS_LDFLAGS += \
    -m elf_x86_64 \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -T arch/x86_64/linker.ld

KERNEL_MODULES += arch/$(ARCH)/kernel
KERNEL_MODULES += kernel/x86