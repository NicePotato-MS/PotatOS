CROS_KERNEL_CFLAGS += \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m32 \
    -march=i686 \
    -mgeneral-regs-only \
    -mno-red-zone \
    -Ilimine \
	-Ikernel/x86/include

CROS_LDFLAGS += \
    -m elf_i386 \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -T arch/i686/linker.ld

KERNEL_MODULES += arch/$(ARCH)/kernel
KERNEL_MODULES += kernel/x86
KERNEL_MODULES += $(KRNLIB)/limine