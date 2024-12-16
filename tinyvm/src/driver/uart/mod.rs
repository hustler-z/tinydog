// @Hustler
//
// Self-Education Only

#[cfg(feature = "ns16550")]
mod ns16550;
#[cfg(feature = "pl011")]
#[allow(dead_code)]
mod pl011;

#[cfg(feature = "ns16550")]
use ns16550::Ns16550Mmio32 as Uart;
#[cfg(feature = "pl011")]
use pl011::Pl011Mmio as Uart;

/// A trait defining operations for a UART device.
trait UartOperation {
	/// Initializes the UART device.
	fn init(&self);
	/// Sends a byte through the UART device.
	fn send(&self, byte: u8);
}

use crate::{
	board::{PlatOperation, Platform},
	utils::device_ref::DeviceRef,
};

/// The base address for the UART device.
const UART_BASE: usize = Platform::HYPERVISOR_UART_BASE;

// SAFETY: The reference of unart is a MMIO address
const UART: DeviceRef<Uart> =
	unsafe { DeviceRef::new(UART_BASE as *const Uart) };

/// Sends a byte to the UART device, replacing '\n' with '\r\n'.
pub fn putc(byte: u8) {
	if byte == b'\n' {
		putc(b'\r');
	}
	UART.send(byte);
}

/// Initializes the UART device.
pub(super) fn init() {
	UART.init();
}
