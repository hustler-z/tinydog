// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

fn main() {
    println!(r#"cargo:rustc-check-cfg=cfg(tinyos_has_basepri)"#);
    println!(r#"cargo:rustc-check-cfg=cfg(tinyos_has_native_rmw)"#);

    match std::env::var("TARGET").unwrap().as_str() {
        "aarch64-unknown-none" => {
            // TODO.
        }
        t => {
            panic!("unknown target {}, update build.rs", t);
        }
    }
}
