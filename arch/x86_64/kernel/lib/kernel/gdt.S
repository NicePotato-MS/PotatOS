
.global asm_gdt_reload_segment

.text
asm_gdt_reload_segment:
    # Inputs
    # rdi - Code Segment
    # rsi - Data Segment
    push %rdi # Code Segment to use
    lea .reload_CS(%rip), %rdi # Load .reload_CS address
    push %rdi # Push address to return to
    retfq # Far Return to pushed address
.reload_CS:
    mov %rsi, %ds # Data Segment
    mov %rsi, %es # Extra Segment
    mov %rsi, %fs
    mov %rsi, %gs
    mov %rsi, %ss # Stack Segment
    ret # Actual return from function
