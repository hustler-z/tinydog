#################################################################
##            Hustler's Tiny Embedded C Project                ##
#################################################################

CURDIR     := .
OUTPUT     := out
ARCH       ?= arm64
BIN        := $(OUTPUT)/tinyog
ELF        := $(OUTPUT)/tinyog.elf
HOST       ?= $(shell uname -m | sed -e s/aarch64/arm64/ \
                     -e s/x86_64/x86/)

ifeq ($(V),1)
	Q :=
else
	Q := @
endif

#################################################################

ifeq ($(HOST), x86)
CROSS_COMPILE ?= aarch64-none-elf-
else
CROSS_COMPILE ?=
endif

CC         := $(CROSS_COMPILE)gcc
CXX        := $(CROSS_COMPILE)cpp
LD         := $(CROSS_COMPILE)ld
AS         := $(CROSS_COMPILE)as
OBJCOPY    := $(CROSS_COMPILE)objcopy

#################################################################

ARCH_DIR   := $(CURDIR)/arch/$(ARCH)
TINY_DIR   := $(CURDIR)/tiny
BOARD_DIR  := $(CURDIR)/tiny/board
DRIVER_DIR := $(CURDIR)/tiny/driver

ARCH_OUT   := $(OUTPUT)/arch/$(ARCH)
TINY_OUT   := $(OUTPUT)/tiny
BOARD_OUT  := $(OUTPUT)/tiny/board
DRIVER_OUT := $(OUTPUT)/tiny/driver

CFLAGS     += -I$(ARCH_DIR)/include
CFLAGS     += -I$(TINY_DIR)/include
CFLAGS     += -I$(BOARD_DIR)/include
CFLAGS     += -nostdlib -nostdinc -g -Wall
CFLAGS     += -std=gnu11 -fno-builtin -fno-common
# objdump -x [.elf]
# CFLAGS     += -Wl,--no-warn-rwx-segments

AFLAGS     += -I$(ARCH_DIR)/include
AFLAGS     += -D__ASSEMBLY__ -D__TINY_START__=0x200000
AFLAGS     += -nostdlib -g -Wall

include $(ARCH_DIR)/arch.mk
include $(TINY_DIR)/tiny.mk
include $(BOARD_DIR)/board.mk
include $(DRIVER_DIR)/driver.mk

asm_obj    := $(addprefix $(ARCH_OUT)/,$(asm-y))
tiny_obj   := $(addprefix $(TINY_OUT)/,$(tiny-y))
arm_obj    := $(addprefix $(ARCH_OUT)/,$(arm-y))
board_obj  := $(addprefix $(BOARD_OUT)/,$(board-y))
driver_obj := $(addprefix $(DRIVER_OUT)/,$(driver-y))
LDS        := $(addprefix $(ARCH_OUT)/,$(lds-y))

#################################################################

OFFSET_H   := $(ARCH_DIR)/include/asm/offset.h
OFFSET_C   := $(ARCH_DIR)/offset.c
OFFSET_S   := offset.S

define sed-offsets
	's:^[[:space:]]*\.ascii[[:space:]]*"\(.*\)".*:\1:; \
	/^->/{s:->#\(.*\):/* \1 */:; \
	s:^->\([^ ]*\) [\$$#]*\([^ ]*\) \(.*\):#define \1 \2 /* \3 */:; \
	s:->::; p;}'
endef

define asm_offsets
	$(Q)echo "OF   $@"; \
	(set -e; \
	 echo "// @Hustler's Project"; \
	 echo ""; \
	 echo "#ifndef _ASM_OFFSET_"; \
	 echo "#define _ASM_OFFSET_"; \
	 echo ""; \
	 sed -ne $(sed-offsets) < $<; \
	 echo ""; \
	 echo "#endif /* _ASM_OFFSET_ */") > $@
endef

#################################################################

cc_cc      = $(Q)mkdir -p `dirname $(1)`; \
             echo "CC   $(subst $(OUTPUT)/,,$(1))"; \
             $(CC) $(CFLAGS) -c $(2) -o $(1)

cc_as      = $(Q)mkdir -p `dirname $(1)`; \
             echo "AS   $(subst $(OUTPUT)/,,$(1))"; \
             $(CC) $(AFLAGS) -c $(2) -o $(1)

cc_ld      = $(Q)mkdir -p `dirname $(1)`; \
             echo "LD   $(subst $(OUTPUT)/,,$(1))"; \
             $(CC) $(3) $(CFLAGS) -T$(2) -o $(1)

cc_lds     = $(Q)mkdir -p `dirname $(1)`; \
             echo "LS   $(patsubst %.lds,%.ld,$(subst $(OUTPUT)/arch/$(ARCH)/,,$(1)))"; \
             $(CXX) -P $(AFLAGS) -DLINKER_SCRIPT -o $(1) $(2)

cc_objcopy = $(Q)mkdir -p `dirname $(1)`; \
             echo "BN   $(subst $(OUTPUT)/,,$(1))"; \
             $(OBJCOPY) -O binary $(2) $(1)

#################################################################

.PHONY: all
all: $(BIN)

$(BIN): $(ELF)
	$(call cc_objcopy,$@,$<)

$(ELF): $(OFFSET_H) $(asm_obj) $(arm_obj) $(tiny_obj) $(board_obj) $(driver_obj) $(LDS)
	$(call cc_ld,$@,$(LDS),$(filter-out $(OFFSET_H),$(filter-out $(LDS),$^)))

$(OFFSET_H): $(OFFSET_S)
	$(call asm_offsets)
	$(Q)rm -f $<

$(OFFSET_S): $(OFFSET_C)
	$(Q)$(CC) $(CFLAGS) -S -g0 -o $@ $<

$(LDS): $(ARCH_OUT)/%.lds: $(ARCH_DIR)/%.lds.S
	$(call cc_lds,$@,$<)

$(asm_obj): $(ARCH_OUT)/%.o: $(ARCH_DIR)/%.S
	$(call cc_as,$@,$<)

$(arm_obj): $(ARCH_OUT)/%.o: $(ARCH_DIR)/%.c
	$(call cc_cc,$@,$<)

$(tiny_obj): $(TINY_OUT)/%.o: $(TINY_DIR)/%.c
	$(call cc_cc,$@,$<)

$(board_obj): $(BOARD_OUT)/%.o: $(BOARD_DIR)/%.c
	$(call cc_cc,$@,$<)

$(driver_obj): $(DRIVER_OUT)/%.o: $(DRIVER_DIR)/%.c
	$(call cc_cc,$@,$<)

#################################################################

clean:
	$(Q)rm -rf $(OUTPUT)

#################################################################
