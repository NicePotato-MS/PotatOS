# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2023

ARCH ?= x86_64
DEBUG ?= true

M ?= 512

MKBUILD = make -f build.mk iso DEBUG=$(DEBUG)

.PHONY: run
run: iso
	qemu-system-$(ARCH) -cdrom build/potatos-$(ARCH).iso -m $(M)M

.PHONY: iso
iso: clean
	$(MKBUILD) ARCH=$(ARCH)

.PHONY: all
all: clean
	$(MKBUILD) ARCH=x86_64
	$(MKBUILD) ARCH=i386

.PHONY: clean
	rm -rf build
	rm -rf bin
	rm -rf obj
	