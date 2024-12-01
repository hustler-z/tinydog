############################################################
# @Hustler
#
# To compile this project, vm0 image (linux kernel image)
# is needed, for linking process.
#
# In this case, VM0_IMG_PATH is must set.
#
# Here rootfs is needed, when use qemu (TODO)
############################################################

ROOTFS              := ../../vm-prebuilt/rootfs.ext4
BUILD               := build
# Compile
ARCH                ?= aarch64
PROFILE             ?= release
BOARD               ?= qemu
# features, separate with comma `,`
FEATURES            ?=

############################################################

ifeq ($(V),1)
Q                   :=
else
Q                   := @
endif

############################################################

VM0_ALIAS           := vm0

ifeq ($(ARCH), aarch64)
CROSS_COMPILE       := aarch64-none-elf-
TEXT_START          := 0x40080000
# This is must set before compilation (TODO)
VM0_IMG_PATH        := ../../vm-prebuilt/Image
else ifeq ($(ARCH), riscv64)
CROSS_COMPILE       := riscv64-linux-gnu-
TEXT_START          := 0x80200000
# This is must set before compilation (TODO)
VM0_IMG_PATH        :=
else
$(error bad arch: $(ARCH))
endif

QEMU                := qemu-system-$(ARCH)

############################################################

OBJDUMP             := ${CROSS_COMPILE}objdump
OBJCOPY             := ${CROSS_COMPILE}objcopy
LD                  := ${CROSS_COMPILE}ld

############################################################

GIC_VERSION         ?= 2

ifeq ($(GIC_VERSION),3)
override FEATURES   += ,gicv3,
else ifneq ($(GIC_VERSION),2)
$(error Bad gic version)
endif

IRQ                 ?= plic
AIA_GUESTS          ?= 3

ifeq ($(IRQ), plic)
FEATURES            += plic
else ifeq ($(IRQ), aia)
FEATURES            += aia
endif

RELOCATE_IMAGE      := libtinyvm.a
IMAGE               := tinyvm

TARGET_DIR          := target/${ARCH}/${PROFILE}

############################################################

ifeq (${ARCH}, aarch64)
QEMU_COMMON_OPTIONS += -machine virt,virtualization=on,gic-version=$(GIC_VERSION) \
                       -m 8g -cpu cortex-a57 -smp 4 -display none \
                       -global virtio-mmio.force-legacy=false \
                       -kernel ${TARGET_DIR}/${IMAGE}.bin
QEMU_NET_OPTIONS    += -netdev user,id=n0,hostfwd=tcp::5555-:22 \
                       -device virtio-net-device,bus=virtio-mmio-bus.24,netdev=n0
QEMU_DISK_OPTIONS   += -drive file=${ROOTFS},if=none,format=raw,id=x0 \
                       -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.25
MKIMAGE_ARCH        := arm64
else ifeq (${ARCH}, riscv64)

ifeq ($(IRQ), plic)
QEMU_COMMON_OPTIONS += -machine virt
else ifeq ($(IRQ), aia)
QEMU_COMMON_OPTIONS += -machine virt,aia=aplic-imsic,aia-guests=$(AIA_GUESTS)
endif
QEMU_COMMON_OPTIONS += -m 8g -smp 4 -display none -bios default \
                       -kernel ${TARGET_DIR}/${IMAGE}.bin
QEMU_NET_OPTIONS    += -netdev user,id=n0,hostfwd=tcp::5555-:22 \
                       -device virtio-net-device,netdev=n0
QEMU_DISK_OPTIONS   += -drive file=${ROOTFS},if=none,format=raw,id=x0 \
                       -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0
MKIMAGE_ARCH        := riscv
else
$(error bad qemu arch: $(ARCH))
endif

# Use target_cfg depending on ARCH
TARGET_CFG          := $(CURDIR)/$(BUILD)/${ARCH}.json

# Combine board(qemu, ...) with previous features as cargo's features
CARGO_FLAGS         ?= -Z build-std=core,alloc \
                       -Zbuild-std-features=compiler-builtins-mem \
                       --target ${TARGET_CFG} --no-default-features \
                       --features "${BOARD},${FEATURES}"
ifeq (${PROFILE}, release)
CARGO_FLAGS         += --release
endif

# Make 'cc' crate in dependencies cross compiles properly.
CFLAGS              += -fno-stack-protector

ifeq ($(ARCH), aarch64)
CFLAGS              += -mgeneral-regs-only
else ifeq ($(ARCH), riscv64)
export CRATE_CC_NO_DEFAULTS := true
CFLAGS              += -ffunction-sections -fdata-sections \
                       -fPIC -fno-omit-frame-pointer -mabi=lp64 \
                       -mcmodel=medany -march=rv64ima \
                       -ffreestanding
endif

############################################################

export BOARD CROSS_COMPILE CFLAGS AIA_GUESTS = $(AIA_GUESTS)

CARGO_ACTION        ?= build

UBOOT_IMAGE         ?= Image$(USER)_$(ARCH)_$(BOARD)

.PHONY: build doc cargo mkimg qemu rk3588 gdb clean clippy

############################################################
# @Hustler
#
# Note that in *.ld files:
#
# INPUT is a 'in-linker script' replacement for adding object
# files to the command line.
############################################################
build: tinyvm
	$(Q)cp $(VM0_IMG_PATH) $(VM0_ALIAS)
	$(Q)$(LD) ${TARGET_DIR}/${RELOCATE_IMAGE} -T $(BUILD)/${ARCH}.ld \
		--defsym TEXT_START=${TEXT_START} -o ${TARGET_DIR}/${IMAGE}
	$(Q)${OBJDUMP} --demangle -d ${TARGET_DIR}/${IMAGE} > ${TARGET_DIR}/t.txt
	$(Q)${OBJCOPY} ${TARGET_DIR}/${IMAGE} -O binary ${TARGET_DIR}/${IMAGE}.bin
	$(Q)rm $(VM0_ALIAS)

tinyvm:
	$(Q)cargo ${CARGO_ACTION} ${CARGO_FLAGS}

mkimg: build
	$(Q)mkimage -n ${IMAGE} -A ${MKIMAGE_ARCH} -O linux \
		-T kernel -C none -a $(TEXT_START) \
		-e $(TEXT_START) -d ${TARGET_DIR}/${IMAGE}.bin \
		${TARGET_DIR}/${UBOOT_IMAGE}

qemu:
	$(Q)$(MAKE) build BOARD=qemu TEXT_START=${TEXT_START} \
		VM0_IMG_PATH=${VM0_IMG_PATH}
	$(Q)${OBJCOPY} ${TARGET_DIR}/${IMAGE} -O binary ${TARGET_DIR}/${IMAGE}.bin

# VM0_IMG_PATH (TODO)
rk3588:
	$(Q)$(MAKE) mkimg BOARD=rk3588 TEXT_START=0x00480000 \
		VM0_IMG_PATH=""

QEMU_SERIAL_OPTIONS = -serial mon:stdio

run: qemu
	$(Q)${QEMU} ${QEMU_COMMON_OPTIONS} ${QEMU_SERIAL_OPTIONS} \
		${QEMU_NET_OPTIONS} ${QEMU_DISK_OPTIONS}

debug: qemu
	$(Q)${QEMU} ${QEMU_COMMON_OPTIONS} ${QEMU_SERIAL_OPTIONS} \
		${QEMU_NET_OPTIONS} ${QEMU_DISK_OPTIONS} \
		-s -S
clean:
	$(Q)cargo clean

clippy: CARGO_ACTION = clippy
clippy: cargo

doc:
	$(Q)$(MAKE) cargo CARGO_ACTION=doc FEATURES="doc,$(FEATURES)"
