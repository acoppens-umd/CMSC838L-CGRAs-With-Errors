#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}


const NUM_BINS: usize = 16;
const BIN_MASK: i32 = (NUM_BINS as i32) - 1;
const LENGTH: usize = 64;

#[no_mangle]
#[inline(never)]
pub extern "C" fn histogram_int(
    data: &[i32; LENGTH] ,
    hist: &mut [i32; NUM_BINS],
) {
    let mut i: i32 = 0;
    while i < LENGTH as i32 {
        unsafe { please_map_me() };
        let value = data[i as usize];
        let bin = (value & BIN_MASK) as usize;
        hist[bin] = hist[bin] + 1;
        i += 1;
    }
}

#[no_mangle]
fn rust_entry() {
    let mut data: [i32; LENGTH] = [0; LENGTH];
    let mut hist: [i32; NUM_BINS] = [0; NUM_BINS];

    /* Initialize input data */
    let mut i: usize = 0;
    while i < LENGTH {
        data[i] = (i as i32) * 53 % 100;
        i += 1;
    }

    /* Clear histogram */
    let mut b: usize = 0;
    while b < NUM_BINS {
        hist[b] = 0;
        b += 1;
    }

    /* Run histogram kernel */
    histogram_int(&data, &mut hist);

    /* Print histogram */
    let mut j: usize = 0;
    while j < NUM_BINS {
        println!("bin[{:<2}] = {}", j, hist[j]);
        j += 1;
    }
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}