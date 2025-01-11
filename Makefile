# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2025

ARCH ?= x86_64
DEBUG ?= true
OUT_ASM ?= false
LIMINE_CUSTOM ?= false
# LIMINE_CUSTOM will use a custom version of limine not included in the repo
# My custom-ish version has a modified source to skip a sanity check.

M ?= 32

MKBUILD = -f build.mk iso DEBUG=$(DEBUG) ARCH=$(ARCH) OUT_ASM=$(OUT_ASM) LIMINE_CUSTOM=$(LIMINE_CUSTOM)

ifneq ($(shell command -v compiledb),)
	MAKETOOL = compiledb make
$(info Using compiledb to build)
else
	MAKETOOL = make
$(info Using make to build (no compiledb))
endif

.PHONY: iso
iso:
	make -f download-tools.mk ARCH=$(ARCH)
	$(MAKETOOL) $(MKBUILD)

.PHONY: download-tools
download-tools:
	make -f download-tools.mk ARCH=$(ARCH) FORCE=true

.PHONY: run
run: iso run-nobuild

.PHONY: run-nobuild
run-nobuild:
	@echo
	qemu-system-x86_64 -debugcon stdio -boot order=cd -m $(M)M -D ./log.txt \
	-cdrom $(shell find ./build/ -type f -name "potatos-x86_64-*.iso" | head -n 1)
	

.PHONY: all
all: clean
	$(MKBUILD)