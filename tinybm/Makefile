############################################################
# Here the code is derived from lilos project
#
# [https://github.com/cbiffle/lilos]
#
# Self Education Only
############################################################

TAR                 := thumbv7em-none-eabihf

############################################################

ifeq ($(V),1)
Q                   :=
else
Q                   := @
endif

############################################################

ifeq ($(R), 1)
EXTRA               := --release
else
EXTRA               :=
endif

############################################################

tinybm:
	$(Q)cargo build --no-default-features \
			--features mutex,spsc,systick \
			--target $(TAR) $(EXTRA)

clean:
	$(Q)cargo clean
