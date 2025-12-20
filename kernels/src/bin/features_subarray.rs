#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const LIST_LEN: usize = 5;
const OUT_LEN: usize = 3;

#[no_mangle]
#[inline(never)]
pub extern "C" fn subarray(index: i32, list: &[i32; LIST_LEN], out: &mut [i32; OUT_LEN]) {
    for i in 0..OUT_LEN {
        unsafe { please_map_me() };
        out[i] = list[index as usize + i];
    }
}

const NUM_TRIALS: usize = 3;

#[no_mangle]
fn rust_entry() {
    let indices: [i32; NUM_TRIALS] = [0,2,4];
    let list: [i32; LIST_LEN] = [1, 2, 3, 4, 5];
    let mut out: [i32; OUT_LEN] = [0; 3];
    for i in 0..NUM_TRIALS {
        subarray(indices[i], &list, &mut out);
    }
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}

