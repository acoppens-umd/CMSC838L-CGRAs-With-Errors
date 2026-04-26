#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const N: usize = 4;

#[inline(always)]
fn rotl(x: u32, n: u32) -> u32 {
    (x << n) | (x >> (32 - n))
}

#[no_mangle]
#[inline(never)]
pub extern "C" fn chacha20_quarter_round(
    a: &mut [u32; N],
    b: &mut [u32; N],
    c: &mut [u32; N],
    d: &mut [u32; N],
) {
    for i in 0..N {
        unsafe { please_map_me() };

        let mut av = a[i];
        let mut bv = b[i];
        let mut cv = c[i];
        let mut dv = d[i];

        av = av.wrapping_add(bv); dv ^= av; dv = rotl(dv, 16);
        cv = cv.wrapping_add(dv); bv ^= cv; bv = rotl(bv, 12);
        av = av.wrapping_add(bv); dv ^= av; dv = rotl(dv,  8);
        cv = cv.wrapping_add(dv); bv ^= cv; bv = rotl(bv,  7);

        a[i] = av;
        b[i] = bv;
        c[i] = cv;
        d[i] = dv;
    }
}

#[no_mangle]
fn rust_entry() {
    // RFC 8439 §2.1.1 test vector, replicated across N independent inputs
    let mut a: [u32; N] = [0x11111111; N];
    let mut b: [u32; N] = [0x01020304; N];
    let mut c: [u32; N] = [0x9b8d6f43; N];
    let mut d: [u32; N] = [0x01234567; N];
    chacha20_quarter_round(&mut a, &mut b, &mut c, &mut d);
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
