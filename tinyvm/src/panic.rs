// @Hustler
//
// Self-Education Only

//! Panic handler

use core::panic::PanicInfo;

use crate::kernel::current_cpu;

#[cfg_attr(target_os = "none", panic_handler)]
fn panic(info: &PanicInfo) -> ! {
    println!("\u{1B}[1;31m[Panic] core {}", current_cpu().id); // 1;31 BrightRed
    println!("{}\u{1B}[0m", info);
    loop {
        core::hint::spin_loop();
    }
}
