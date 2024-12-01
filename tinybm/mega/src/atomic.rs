// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//! A collection of atomic "polyfill" routines, to use a term from JavaScript.
//!
//! ARMv6-M processors like the Cortex-M0 don't support the fancier atomic
//! operations available on most other ARM processors. In particular, they have
//! no atomic swap or read-modify-write instructions. This module provides
//! traits that use the native atomics on M3 and later, and fallback
//! implementations on M0.
//!
//! The M0 implementations rely on disabling interrupts. This means that:
//!
//! 1. They will hurt interrupt latency/jitter. However, the M0 already has
//!    pretty poor interrupt latency/jitter because of uninterruptible
//!    instructions and lack of BASEPRI. So, not a big loss.
//!
//! 2. They don't work in unprivileged mode. But, neither does most of `lilos`.
//!
//! This is exposed so that applications don't have to rewrite it for M0
//! support.

use core::sync::atomic::{AtomicBool, AtomicPtr, AtomicU32, AtomicUsize, Ordering};

mod private {
    /// ########################################################################
    /// @Hustler
    ///
    /// Trait
    ///
    /// Static Dispatch - Figure out which method to call at compile time.
    ///
    ///
    ///
    /// Trait Object - The combination of a type that implements a trait and its
    ///                vtable (Virtual Method Table).
    ///                                         |
    ///                                         V
    ///                                  Dynamic Dispatch
    ///
    ///                  [Figure out which method to call at runtime time]
    ///
    ///                  Trait                            +-------------+
    ///                    |        pointer               |             |
    ///                    +---------------------> vtable :  Interfaces :
    ///                                                   |             |
    ///                                                   +-------------+
    ///
    /// For instance, &dyn [Defined-Trait]
    ///
    /// To be object-safe, none of a trait's methods can be generic or use the
    /// Self type.
    /// ########################################################################
    pub trait Sealed {}
}

/// Basic atomic operations.
///
/// Note that this trait is _sealed,_ meaning it cannot be implemented for
/// types outside `lilos` itself.
///
/// @Hustler
///
/// As the explanation above
///
/// Sealed Trait is one that can be used only, but not implemented by other
/// crates.
pub trait AtomicExt: private::Sealed {
    /// Primitive type corresponding to this atomic type.
    ///
    /// @Hustler
    ///
    /// Associated Type
    type Value;

    /// Atomically exchange our current contents for `val`, returning the
    /// original contents.
    ///
    /// Replacement for `swap`.
    fn swap_polyfill(&self, val: Self::Value, ordering: Ordering) -> Self::Value;

    /// If `self`'s value is equal to `current`, atomically replace it with
    /// `new`, otherwise leave it untouched.
    ///
    /// Returns `Ok(current)` on success, `Err(actual_value)` on failure.
    ///
    /// Replacement for `compare_exchange`.
    fn compare_exchange_polyfill(
        &self,
        current: Self::Value,
        new: Self::Value,
        success: Ordering,
        failure: Ordering,
    ) -> Result<Self::Value, Self::Value>;

    /// Fetches the current value using `fetch_order`, applies `f` to it. If `f`
    /// produces `Some(new_value)`, attempts to swap the value that was read for
    /// `new_value`. If `f` produces `None`, stops.
    ///
    /// If the swap fails, the process repeats and `f` is called again with the
    /// new observed value.
    ///
    /// Replacement for `fetch_update`.
    ///
    /// ########################################################################
    /// @Hustler 2024/11/16
    ///
    /// Here I plan to dig on closure and funtion pointer, since I'm not fully
    /// understand the concept and also how to use them in real situation.
    ///
    /// Function Item
    ///
    /// Function Pointers
    ///
    /// fn pass_callback(callback: fn(x: u32) -> u32) {}
    ///
    ///
    /// Fn Traits
    ///
    /// [1] FnOnce - A closure that can be called once.
    /// [2] FnMut  - A closure doesn't move captured values out of its body, But
    ///              might mutate the values.
    /// [3] Fn     - A closure doesn't move captured values out of its body, also
    ///              does not mutate the values.
    ///
    /// Closure Anonymous Function that Capture Their Environment
    ///
    ///
    ///
    /// ########################################################################
    fn fetch_update_polyfill(
        &self,
        set_order: Ordering,
        fetch_order: Ordering,
        f: impl FnMut(Self::Value) -> Option<Self::Value>,
    ) -> Result<Self::Value, Self::Value>;
}

/// Atomic operations that apply to arithmetic types.
pub trait AtomicArithExt: AtomicExt {
    /// Atomically add `val` to our contents, returning the original value.
    fn fetch_add_polyfill(&self, val: Self::Value, ordering: Ordering) -> Self::Value;
    /// Atomically subtract `val` to our contents, returning the original value.
    fn fetch_sub_polyfill(&self, val: Self::Value, ordering: Ordering) -> Self::Value;
    /// Atomically OR `val` into our contents, returning the original value.
    fn fetch_or_polyfill(&self, val: Self::Value, ordering: Ordering) -> Self::Value;
}

/// ############################################################################
/// @Hustler 2024/11/17
///
///
/// To understand Rust 'macro_rules' a little better, here I try to walk through
/// the code and see if I can understand why and how.
///
/// In Rust, there are two types of macros, declarative macros as below, also
/// procedural macros.
///
/// ($matcher) => {$expansion}
///
///          Capture
///             |
///             V
/// $identifier:fragment-specifier
///                     |
///                    [0]  block
///                    [1]  expr
///                    [2]  ident
///                    [3]  item
///                    [4]  lifetime
///                    [5]  literal
///                    [6]  meta
///                    [7]  pat      a pattern
///                    [8]  path
///                    [9]  stmt     a statement
///                    [10] tt       a single token tree
///                    [11] ty       a type
///                    [12] vis
///
/// Matcher can contains repetitions:
///
/// $( ... ) separator-token repeat-operator
///                |                |
///                V                V
///            [, and ;]       [0] ? Indicate at most one repetition
///                            [1] * Indicate zero or more repetitions
///                            [2] + Indicate one or more repetitions
///
/// ############################################################################
macro_rules! impl_atomic_polyfills {
    ($t:ty, $v:ty $(, $param:ident)?) => {
        impl<$($param)?> private::Sealed for $t {}

        // Native version
        #[cfg(lilos_has_native_rmw)]
        impl<$($param)?> AtomicExt for $t {
            type Value = $v;

            fn swap_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                self.swap(val, ordering)
            }

            fn compare_exchange_polyfill(
                &self,
                current: Self::Value,
                new: Self::Value,
                success: Ordering,
                failure: Ordering,
            ) -> Result<Self::Value, Self::Value> {
                self.compare_exchange(current, new, success, failure)
            }

            fn fetch_update_polyfill(
                &self,
                set_order: Ordering,
                fetch_order: Ordering,
                f: impl FnMut(Self::Value) -> Option<Self::Value>,
            ) -> Result<Self::Value, Self::Value> {
                self.fetch_update(set_order, fetch_order, f)
            }
        }

        // Non-native version
        #[cfg(not(lilos_has_native_rmw))]
        impl<$($param)?> AtomicExt for $t {
            type Value = $v;

            #[inline(always)]
            fn swap_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                let (lo, so) = rmw_ordering(ordering);
                cortex_m::interrupt::free(|_| {
                    let x = self.load(lo);
                    self.store(val, so);
                    x
                })
            }

            fn compare_exchange_polyfill(
                &self,
                current: Self::Value,
                new: Self::Value,
                success: Ordering,
                failure: Ordering,
            ) -> Result<Self::Value, Self::Value> {
                let (lo, so) = rmw_ordering(success);
                cortex_m::interrupt::free(|_| {
                    let x = self.load(lo);
                    if x == current {
                        self.store(new, so);
                        Ok(x)
                    } else {
                        self.store(x, failure);
                        Err(x)
                    }
                })
            }

            fn fetch_update_polyfill(
                &self,
                set_order: Ordering,
                fetch_order: Ordering,
                mut f: impl FnMut(Self::Value) -> Option<Self::Value>
            ) -> Result<Self::Value, Self::Value> {
                let mut prev = self.load(fetch_order);
                while let Some(next) = f(prev) {
                    let cx_result = self.compare_exchange_polyfill(
                        prev,
                        next,
                        set_order,
                        fetch_order,
                    );
                    match cx_result {
                        Ok(x) => return Ok(x),
                        Err(change) => prev = change,
                    }
                }
                Err(prev)
            }
        }
    };
}

impl_atomic_polyfills!(AtomicU32, u32);
impl_atomic_polyfills!(AtomicUsize, usize);
impl_atomic_polyfills!(core::sync::atomic::AtomicIsize, isize);
impl_atomic_polyfills!(AtomicBool, bool);
impl_atomic_polyfills!(AtomicPtr<T>, *mut T, T);

macro_rules! impl_atomic_arith_polyfills {
    ($t:ty $(, $param:ident)?) => {
        // Native version
        #[cfg(lilos_has_native_rmw)]
        impl<$($param)?> AtomicArithExt for $t {
            fn fetch_add_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                self.fetch_add(val, ordering)
            }

            fn fetch_sub_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                self.fetch_sub(val, ordering)
            }

            fn fetch_or_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                self.fetch_or(val, ordering)
            }
        }

        // Non-native version
        #[cfg(not(lilos_has_native_rmw))]
        impl<$($param)?> AtomicArithExt for $t {
            fn fetch_add_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                let (lo, so) = rmw_ordering(ordering);
                cortex_m::interrupt::free(|_| {
                    let x = self.load(lo);
                    self.store(x.wrapping_add(val), so);
                    x
                })
            }

            fn fetch_sub_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                let (lo, so) = rmw_ordering(ordering);
                cortex_m::interrupt::free(|_| {
                    let x = self.load(lo);
                    self.store(x.wrapping_sub(val), so);
                    x
                })
            }

            fn fetch_or_polyfill(
                &self,
                val: Self::Value,
                ordering: Ordering,
            ) -> Self::Value {
                let (lo, so) = rmw_ordering(ordering);
                cortex_m::interrupt::free(|_| {
                    let x = self.load(lo);
                    self.store(x | val, so);
                    x
                })
            }
        }
    };
}

impl_atomic_arith_polyfills!(core::sync::atomic::AtomicIsize);
impl_atomic_arith_polyfills!(AtomicUsize);
impl_atomic_arith_polyfills!(AtomicU32);

#[cfg(not(lilos_has_native_rmw))]
#[inline(always)]
fn rmw_ordering(o: Ordering) -> (Ordering, Ordering) {
    match o {
        Ordering::AcqRel => (Ordering::Acquire, Ordering::Release),
        Ordering::Relaxed => (o, o),
        Ordering::SeqCst => (o, o),
        Ordering::Acquire => (Ordering::Acquire, Ordering::Relaxed),
        Ordering::Release => (Ordering::Relaxed, Ordering::Release),
        _ => panic!(),
    }
}
