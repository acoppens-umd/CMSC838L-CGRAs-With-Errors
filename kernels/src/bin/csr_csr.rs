#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const M: usize = 10;
const K: usize = 8;
const N: usize = 6;
const DATA_SIZE: usize = 10;


#[no_mangle]
#[inline(never)]
pub extern "C" fn csr_csr_matmul(a_row_ptr: &[i32; M + 1], a_col_ind: &[i32; DATA_SIZE], a_data: &[i32; DATA_SIZE],
                                 b_row_ptr: &[i32; K + 1], b_col_ind: &[i32; DATA_SIZE], b_data: &[i32; DATA_SIZE],
                                 out: &mut[i32; M*N]) {
    for curr_m in 0..M {
        // For each non-zero entry A[i][k]
        let a_row_start: i32 = a_row_ptr[curr_m];
        let a_row_end: i32 = a_row_ptr[curr_m + 1];
        for a_ind in a_row_start..a_row_end {
            let k: i32 = a_col_ind[a_ind as usize];
            let a_val: i32 = a_data[a_ind as usize];

            let b_row_start: i32 = b_row_ptr[k as usize];
            let b_row_end: i32 = b_row_ptr[(k + 1) as usize];
            // For each non-zero entry B[k][j]
            for b_ind in b_row_start..b_row_end {
               unsafe { please_map_me() };

               let n: i32 = b_col_ind[b_ind as usize];
               let b_val: i32 = b_data[b_ind as usize];

               // Accumulate into dense C[i][j]
               out[(k * N as i32 + n) as usize] += a_val * b_val;
            }
        }
    }
}

#[no_mangle]
fn rust_entry() {  
    let a_row_ptr: [i32; M + 1] = [0, 1, 2, 2, 3, 5, 6, 6, 6, 8, 10];
    let a_col_ind: [i32; DATA_SIZE] = [5, 3, 6, 2, 1, 1, 0, 7, 3, 4];
    let a_data: [i32; DATA_SIZE] = [5, 2, 2, 4, 16, 3, 9, 0, 5, 10];
    let b_row_ptr: [i32; K + 1] = [0, 2, 2, 3, 5, 5, 6, 8, 10];
    let b_col_ind: [i32; DATA_SIZE] = [5, 4, 2, 1, 3, 4, 0, 0, 2, 1];
    let b_data: [i32; DATA_SIZE] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let mut out: [i32; M*N] = [0; M*N];
    csr_csr_matmul(&a_row_ptr, &a_col_ind, &a_data, 
                   &b_row_ptr, &b_col_ind, &b_data, 
                   &mut out);
    println!("{:?}", out)
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
