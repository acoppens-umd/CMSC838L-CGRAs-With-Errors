#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const LIST_LEN: usize = 5;
const OUT_LEN: usize = 2;

#[no_mangle]
#[inline(never)]
pub extern "C" fn subarray_mul(index: i32, list1: &[i32; LIST_LEN], list2: &[i32; LIST_LEN], out: &mut [i32; OUT_LEN]) {
    for i in 0..OUT_LEN {
        unsafe { please_map_me() };
        out[i] = list1[index as usize + i]*list2[index as usize + i];
    }
}

const NUM_TRIALS: usize = 3;

#[no_mangle]
fn rust_entry() {
    let indices: [i32; NUM_TRIALS] = [0,3,2];
    let list1: [i32; LIST_LEN] = [1500000000, 2, 0, 4, 5];
    let list2: [i32; LIST_LEN] = [1500000000, 2, 2, 4, 5];
    let mut out: [i32; OUT_LEN] = [0; OUT_LEN];
    for i in 0..NUM_TRIALS {
        subarray_mul(indices[i], &list1, &list2, &mut out);
    }
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}