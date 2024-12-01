// @Hustler
//
// Self-Education Only

//! Board-specific configuration

pub use self::platform_common::*;

#[cfg(all(feature = "qemu", target_arch = "aarch64"))]
pub use self::qemu::{QemuPlatform as Platform, PLAT_DESC};

#[cfg(all(feature = "qemu", target_arch = "riscv64"))]
pub use self::qemu_riscv64::{QemuPlatform as Platform, PLAT_DESC};

#[cfg(feature = "rk3588")]
pub use self::rk3588::{Rk3588Platform as Platform, PLAT_DESC};

mod platform_common;

#[cfg(all(feature = "qemu", target_arch = "aarch64"))]
mod qemu;

#[cfg(all(feature = "qemu", target_arch = "riscv64"))]
mod qemu_riscv64;

#[cfg(feature = "rk3588")]
mod rk3588;
