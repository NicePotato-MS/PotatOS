CROS_KERNEL_CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m32 \
    -march=i686 \
    -mgeneral-regs-only \
    -mno-red-zone \
	-lgcc \
    -Ilimine

CROS_LDFLAGS += \
    -m elf_i386 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T arch/i686/linker.ld

KERNEL_MODULES += arch/$(ARCH)/kernel
KERNEL_MODULES += $(KRNLIB)/x86
KERNEL_MODULES += $(KRNLIB)/limine