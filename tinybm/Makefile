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

tinybm:
	$(Q)cargo build --no-default-features \
			--features mutex,spsc,systick \
			--target $(TAR)

clean:
	$(Q)cargo clean
