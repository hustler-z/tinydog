TARGET_ARCH := arm64
TARGET_CFGS :=
RUSTC_FLAGS += -Zbuild-std=core
RUSTC_FLAGS += -Zbuild-std-features=compiler-builtins-mem
RUSTC_FLAGS += --release

ifeq ($(V),1)
	Q :=
else
	Q := @
endif

ifeq ($(TARGET_ARCH), arm64)
	TARGET_CFGS = build/aarch64-unknown-none.json
endif

ifeq ($(TARGET_ARCH), riscv64)
	TARGET_CFGS = build/riscv64gcv-unknown-none-elf.json
endif

ifeq ($(TARGET_ARCH), x64)
	TARGET_CFGS = build/x86_64-unknown-none.json
endif

tinyfw:
	$(Q)cargo build --target $(TARGET_CFGS) $(RUSTC_FLAGS)

clean:
	$(Q)cargo clean
