# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

include ./../system-config.cfg

LIB_OBJS=$(patsubst %.c,%.o,$(shell find . -name '*.c'))

.PHONY: all
all: $(LIB_OBJS)

.PHONY: clean
clean:
	rm -rf *.o */*.o */*/*.o
	rm -rf *.d */*.d */*/*.d

# Compilation rules for *.c files.
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
%.o: %.S
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
%.o: %.asm
	nasm $(NASMFLAGS) $< -o $@