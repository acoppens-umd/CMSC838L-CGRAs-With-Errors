
const NUM_ROWS: usize = 3;
const NUM_COLS: usize = 2;
const VALUES_SIZE: usize = 6;

// Conducts sparse matrix-vector multiplcation on a matrix in CSR format and a dense vector
// Use compress_csr.rs function to compute CSR format from dense matrix
fn csr_spmv(row_ptrs: &[usize; NUM_ROWS], col_indices: &[usize; VALUES_SIZE], 
matrix_values: &[i32; VALUES_SIZE], mult_vector: &[i32; NUM_COLS], out: &mut [i32; NUM_ROWS]) {
    let prev_row: usize = 1;
    for curr_row in 0..NUM_ROWS {
        let mut curr_result: i32 = 0;
        if row_ptrs[curr_row] != prev_row {
            //end index is the actual end index + 1 (for the for loop)
            let end_index: usize;
            if curr_row == NUM_ROWS - 1 {
                end_index = VALUES_SIZE;
            }
            else {
                end_index = row_ptrs[curr_row + 1];
            }
            for i in row_ptrs[curr_row]..end_index {
                curr_result += matrix_values[i] * mult_vector[col_indices[i]];
            }
        }
        out[curr_row] = curr_result;
    }
}

fn main() {
    let row: [usize; NUM_ROWS] = [0, 2, 4];
    let col: [usize; VALUES_SIZE] = [0, 1, 0, 1, 0, 1];
    let val: [i32; VALUES_SIZE] = [1, 2, 3, 4, 5, 6];
    let mult_vector: [i32; NUM_COLS] = [2, 3];
    let mut out: [i32; NUM_ROWS] = [0; NUM_ROWS];
    csr_spmv(&row, &col, &val, &mult_vector, &mut out);
    println!("{:?}", out);
}



