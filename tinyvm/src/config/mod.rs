// @Hustler
//
// Self-Education Only

//! MVM init configuration, including platform-specific configuration and VM configuration.

pub use self::config::*;

#[cfg(feature = "static-config")]
pub use self::vm_def::*;

#[cfg(all(feature = "qemu", target_arch = "aarch64"))]
pub use self::qemu_def::*;

#[cfg(all(feature = "qemu", target_arch = "riscv64"))]
pub use self::qemu_riscv64_def::*;

#[cfg(feature = "rk3588")]
pub use self::rk3588_def::*;

mod config;

#[cfg(all(feature = "qemu", target_arch = "aarch64"))]
mod qemu_def;

#[cfg(all(feature = "qemu", target_arch = "riscv64"))]
mod qemu_riscv64_def;

#[cfg(feature = "rk3588")]
mod rk3588_def;

#[cfg(feature = "static-config")]
mod vm_def;
