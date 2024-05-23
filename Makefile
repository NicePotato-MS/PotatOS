# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2023

ARCH ?= x86_64
DEBUG ?= true
OUT_ASM ?= false
LIMINE_CUSTOM ?= false
# LIMINE_CUSTOM will use a custom version of limine not included in the repo
# My custom-ish version has a modified source to skip a sanity check.

M ?= 32

MKBUILD = make -f build.mk iso DEBUG=$(DEBUG) ARCH=$(ARCH) OUT_ASM=$(OUT_ASM) LIMINE_CUSTOM=$(LIMINE_CUSTOM)

.PHONY: iso
iso: clean
	compiledb $(MKBUILD)

.PHONY: run
run: iso
# Note that boot order below is not just cd, it's c (hdd), d (cd)
	@echo
	qemu-system-x86_64 -debugcon stdio -boot order=cd -cdrom build/potatos-$(ARCH).iso -m $(M)M -D ./log.txt

.PHONY: run-nobuild
run-nobuild:
	@echo
	qemu-system-x86_64 -debugcon stdio -boot order=cd -cdrom build/potatos-$(ARCH).iso -m $(M)M -D ./log.txt
	

.PHONY: all
all: clean
	$(MKBUILD)

.PHONY: clean
	rm -rf build
	rm -rf bin
	rm -rf obj
	