// @Hustler
//
// Self-Education Only

use core::fmt::{Arguments, Write};
use spin::Mutex;

pub struct Writer;

static WRITER: Mutex<Writer> = Mutex::new(Writer);

impl Write for Writer {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        for b in s.bytes() {
            #[cfg(target_arch = "aarch64")]
            crate::driver::putc(b);
            #[cfg(target_arch = "riscv64")]
            sbi_rt::console_write_byte(b);
        }
        Ok(())
    }
}

/// Print a format string to the console.
pub fn _print(args: Arguments) {
    // use core::fmt::Write;
    let mut lock = WRITER.lock();
    lock.write_fmt(args).unwrap();
}
