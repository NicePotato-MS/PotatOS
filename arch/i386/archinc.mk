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
    -march=i386 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -Ilimine

CROS_LDFLAGS += \
    -m elf_i386 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T arch/i386/linker.ld

KERNEL_SRC_C += $(shell find arch/$(ARCH)/kernel -type f -name '*.c')
