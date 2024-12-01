TARGET := aarch64-unknown-none

ifeq ($(V),1)
	Q :=
else
	Q := @
endif

tinyos:
	$(Q)cargo build --target=$(TARGET)

clean:
	$(Q)cargo clean
