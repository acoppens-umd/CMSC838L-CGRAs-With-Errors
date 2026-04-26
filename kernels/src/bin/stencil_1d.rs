#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const LENGTH: usize = 16;

#[no_mangle]
#[inline(never)]
pub extern "C" fn stencil_1d(a: &[i32; LENGTH], out: &mut [i32; LENGTH]) {
    for i in 0..LENGTH {
        unsafe { please_map_me() };
        let left   = if i > 0          { a[i - 1] } else { 0 };
        let center = a[i];
        let right  = if i + 1 < LENGTH { a[i + 1] } else { 0 };
        out[i] = left + 2 * center + right;
    }
}

#[no_mangle]
fn rust_entry() {
    let a: [i32; LENGTH] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16];
    let mut out: [i32; LENGTH] = [0; LENGTH];
    stencil_1d(&a, &mut out);
    println!("{:?}", out);
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
