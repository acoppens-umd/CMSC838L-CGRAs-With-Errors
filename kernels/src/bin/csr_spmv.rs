#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const NUM_ROWS: usize = 3;
const NUM_COLS: usize = 2;
const VALUES_SIZE: usize = 6;

// Conducts sparse matrix-vector multiplcation on a matrix in CSR format and a dense vector
// Use compress_csr.rs function to compute CSR format from dense matrix
#[no_mangle]
#[inline(never)]
pub extern "C" fn csr_spmv(row_ptrs: &[i32; NUM_ROWS], col_indices: &[i32; VALUES_SIZE], 
                          matrix_values: &[i32; VALUES_SIZE], mult_vector: &[i32; NUM_COLS], out: &mut [i32; NUM_ROWS]) {
    let prev_row: i32 = 1;
    for curr_row in 0..NUM_ROWS {
        let mut curr_result: i32 = 0;
        if row_ptrs[curr_row as usize] != prev_row {
            //end index is the actual end index + 1 (for the for loop)
            let end_index: i32;
            if curr_row == NUM_ROWS - 1 {
                end_index = VALUES_SIZE as i32;
            }
            else {
                end_index = row_ptrs[curr_row + 1 as usize];
            }
            for i in row_ptrs[curr_row as usize]..end_index {
                unsafe { please_map_me() };
                curr_result += matrix_values[i as usize] * mult_vector[col_indices[i as usize] as usize];
            }
        }
        out[curr_row as usize] = curr_result;
    }
}

#[no_mangle]
pub extern "C" fn rust_entry() {
    let row: [i32; NUM_ROWS] = [0, 2, 4];
    let col: [i32; VALUES_SIZE] = [0, 1, 0, 1, 0, 1];
    let val: [i32; VALUES_SIZE] = [1, 2, 3, 4, 5, 6];
    let mult_vector: [i32; NUM_COLS] = [2, 3];
    let mut out: [i32; NUM_ROWS] = [0; NUM_ROWS];
    csr_spmv(&row, &col, &val, &mult_vector, &mut out);
    println!("{:?}", out);
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}

