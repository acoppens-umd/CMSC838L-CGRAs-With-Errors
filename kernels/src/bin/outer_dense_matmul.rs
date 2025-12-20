#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const M: usize = 4;
const K: usize = 5;
const N: usize = 6;


// Conducts outer product matrix multiplication with dense matrices
#[no_mangle]
#[inline(never)]
fn outermatmul(a: &[i32; K*M], b: &[i32; N*K], out: &mut[i32; N*M]) {
    for curr_k in 0..K {
        for curr_m in 0..M {
            for curr_n in 0..N {
                unsafe { please_map_me() };
                out[curr_m*N+curr_n] += a[curr_m*K+curr_k] * b[curr_k*N+curr_n];
            }
        }
    }
}

#[no_mangle]
fn rust_entry() {
    let a: [i32; K*M] = [1,2,3,4,5,
                         6,7,8,9,10,
                         11,12,13,14,15,
                         16,17,18,19,20];
    let b: [i32; N*K] = [1,2,3,4,5,6,
                         7,8,9,10,11,12,
                         13,14,15,16,17,18,
                         19,20,21,22,23,24,
                         25,26,27,28,29,30];
    let mut out: [i32; N*M] = [0; N*M];
    outermatmul(&a, &b, &mut out);
    println!("{:?}", out)
}


fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
