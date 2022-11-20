; A boot sector that enters 32 - bit protected mode.
[org 0x7c00 ]
KERNEL_OFFSET equ 0x1000   ; This is the memory offset to which we will load our kernel
mov [BOOT_DRIVE], dl       ; BIOS stores our boot drive in DL , so it ’s
                            ; best to remember this for later.

mov bp, 0x9000              ; Set the stack.
mov sp, bp

call load_kernel
call switch_to_pm           ; Note that we never return from here.
jmp $

%include "src/boot/disk_load.asm"
%include "src/boot/print_string.asm"
%include "src/boot/gdt.asm"
%include "src/boot/switch_to_pm.asm"

[bits 16]
; load_kernel
load_kernel:
mov si, LOAD_KERNEL_MSG
call print_string
mov bx, KERNEL_OFFSET ; Set -up parameters for our disk_load routine , so
mov dh, 15 ; that we load the first 15 sectors ( excluding
mov dl, [BOOT_DRIVE] ; the boot sector ) from the boot disk ( i.e. our
call disk_load ; kernel code ) to address KERNEL_OFFSET
ret

[bits 32]
                            ; This is where we arrive after switching to and initialising protected mode.
BEGIN_PM:

call KERNEL_OFFSET          ; Jump to kernel
jmp $                       ; Hang.

; Global variables
BOOT_DRIVE db 0
LOAD_KERNEL_MSG db "Loading Kernel..." , 0

; Bootsector padding
times 510 -($-$$) db 0
dw 0xaa55