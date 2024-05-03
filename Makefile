# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2023

ARCH ?= x86_64
DEBUG ?= true
OUT_ASM ?= false

M ?= 512

MKBUILD = make -f build.mk iso DEBUG=$(DEBUG) ARCH=$(ARCH) OUT_ASM=$(OUT_ASM)

.PHONY: run
run: iso
	qemu-system-$(ARCH) -cdrom build/potatos-$(ARCH).iso -m $(M)M -D ./log.txt

.PHONY: iso
iso: clean
	$(MKBUILD)

.PHONY: all
all: clean
	$(MKBUILD)
#	$(MKBUILD) ARCH=i386

.PHONY: clean
	rm -rf build
	rm -rf bin
	rm -rf obj
	