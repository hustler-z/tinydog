// @Hustler
//
// Self-Education Only

#[cfg(feature = "pi4")]
pub use self::gpio::*;

#[cfg(feature = "pi4")]
mod gpio;
