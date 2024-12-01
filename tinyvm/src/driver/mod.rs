// @Hustler
//
// Self-Education Only

//! Driver module, including device drivers and board drivers.

#[cfg(target_arch = "aarch64")]
pub use uart::putc;

#[cfg(feature = "pi4")]
pub use self::gpio::*;

mod gpio;
mod uart;
