#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

pub enum DotProductError {
    ZeroValueFound
}

#[no_mangle]         // avoid Rust symbol-mangling (makes names obvious in IR)
#[inline(never)]     // prevent the compiler from inlining this function away

pub extern "C" fn foo(a: &[i32; 5], b: &[i32; 5], c: &mut [i32; 5]) -> Result<i32, DotProductError> {

    let mut sum = 0;

    for ((&x, &y), z) in (a.iter().zip(b.iter())).zip(c.iter_mut()) {
        unsafe { please_map_me() };
        if x == 0 || y == 0 {
            return Err(DotProductError::ZeroValueFound);
        }
        if x == 1 {
            panic!("xisone");
        }
        *z = x * y;
        sum += *z;
    }

    Ok(sum)
}

#[no_mangle]
pub extern "C" fn rust_entry() -> i32 {
    let v1 = [4, 5, 6, 1, 5];
    let v2 = [4, 5, 8, 2, 2];
    let mut v3 = [0, 0, 0, 0, 0];
    let _ = foo(&v1, &v2,  &mut v3 as &mut [i32; 5]);  
    0
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}

