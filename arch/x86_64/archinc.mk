CROS_KERNEL_CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
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
	-Ikernel/x86/include

CROS_LDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T arch/x86_64/linker.ld

KERNEL_MODULES += arch/$(ARCH)/kernel
KERNEL_MODULES += $(KRNLIB)/x86
KERNEL_MODULES += $(KRNLIB)/limine