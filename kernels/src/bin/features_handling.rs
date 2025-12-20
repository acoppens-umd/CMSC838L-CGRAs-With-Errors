#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

pub enum SpecialError {
    OutOfBounds
}

const LIST_LEN: usize = 10;
const INDICES_LEN: usize = 5;

#[no_mangle]
#[inline(never)]
pub extern "C" fn index(list: &[i32; LIST_LEN], indices: &[i32; INDICES_LEN], out: &mut [i32; INDICES_LEN]) {

    for i in 0..INDICES_LEN {
        for j in 0..3 {
            unsafe { please_map_me() };
            let k: i32 = j+indices[i as usize];
            let result: Result<i32, SpecialError> = if k > 6 {Err(SpecialError::OutOfBounds)} else {Ok(k)}; 
            match result {
                Ok(l) => {out[i] = list[indices[i] as usize]; },
                Err(e) => {out[i] = 1; },
            }
        }
    }
}

#[no_mangle]
fn rust_entry() {
    let indices: [i32; INDICES_LEN] = [0,1,6,2,4];
    let list: [i32; LIST_LEN] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let mut out: [i32; INDICES_LEN] = [0; INDICES_LEN];
    index(&list, &indices, &mut out);
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}

