#################################################################
##            Hustler's Tiny Embedded C Project                ##
#################################################################

CURDIR            := .
OUTPUT            := out
ARCH              ?= arm64
BIN               := $(OUTPUT)/tinyos
ELF               := $(OUTPUT)/tinyos.elf
HOST              ?= $(shell uname -m | sed -e s/aarch64/arm64/ \
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

CC                := $(CROSS_COMPILE)gcc
CXX               := $(CROSS_COMPILE)cpp
LD                := $(CROSS_COMPILE)ld
AS                := $(CROSS_COMPILE)as
OBJCOPY           := $(CROSS_COMPILE)objcopy

#################################################################

ARCH_DIR          := $(CURDIR)/arch/$(ARCH)
TINY_DIR          := $(CURDIR)/tiny

ARCH_OUT          := $(OUTPUT)/arch/$(ARCH)
TINY_OUT          := $(OUTPUT)/tiny

CFLAGS            += -I$(ARCH_DIR)/include
CFLAGS            += -I$(TINY_DIR)/include

AFLAGS            += -I$(ARCH_DIR)/include
AFLAGS            += -D__ASSEMBLY__

include $(ARCH_DIR)/arch.mk
include $(TINY_DIR)/tiny.mk

asm_obj           := $(addprefix $(ARCH_OUT)/,$(asm-y))
tiny_obj          := $(addprefix $(TINY_OUT)/,$(tiny-y))

LDS               := $(addprefix $(ARCH_OUT)/,$(lds-y))

#################################################################

cc_cc      = $(Q)mkdir -p `dirname $(1)`; \
             echo "CC       $(subst $(OUTPUT)/,,$(1))"; \
             $(CC) $(CFLAGS) -c $(2) -o $(1)

cc_as      = $(Q)mkdir -p `dirname $(1)`; \
             echo "AS       $(subst $(OUTPUT)/,,$(1))"; \
             $(CC) $(AFLAGS) -c $(2) -o $(1)

cc_ld      = $(Q)mkdir -p `dirname $(1)`; \
             echo "LD       $(subst $(OUTPUT)/,,$(1))"; \
             $(CC) $(3) $(LDFLAGS) -T$(2) -o $(1)

cc_lds     = $(Q)mkdir -p `dirname $(1)`; \
             echo "LD       $(patsubst %.lds,%.ld,$(subst $(OUTPUT)/,,$(1)))"; \
             $(CXX) -P $(AFLAGS) -DLINKER_SCRIPT -o $(1) $(2)

cc_objcopy = $(Q)mkdir -p `dirname $(1)`; \
             echo "END      $(subst $(OUTPUT)/,,$(1))"; \
             $(OBJCOPY) -O binary $(2) $(1)

#################################################################

.PHONY: all
all: $(BIN)

$(BIN): $(ELF)
	$(call cc_objcopy,$@,$<)

$(ELF): $(asm_obj) $(tiny_obj) $(LDS)
	$(call cc_ld,$@,$(LDS),$(filter-out $(LDS),$^))

$(LDS): $(ARCH_OUT)/%.lds: $(ARCH_DIR)/%.lds.S
	$(call cc_lds,$@,$<)

$(asm_obj): $(ARCH_OUT)/%.o: $(ARCH_DIR)/%.S
	$(call cc_as,$@,$<)

$(tiny_obj): $(TINY_OUT)/%.o: $(TINY_DIR)/%.c
	$(call cc_cc,$@,$<)

#################################################################

clean:
	$(Q)rm -rf $(OUTPUT)

#################################################################
