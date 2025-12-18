

// Compresses a vector into CSR format (with row pointers, column indices, and values)\
// Note: Do not map with Morpher since we cannot feasibly implement with fixed-size arrays
fn compress_csr(in_matrix: &Vec<Vec<i32>>) -> (Vec<usize>, Vec<usize>, Vec<i32>) {
    let mut row_ptrs: Vec<usize> = Vec::new();
    let mut column_indices: Vec<usize> = Vec::new();
    let mut values: Vec<i32> = Vec::new();
    let mut next_index: usize = 0;
    for curr_row in 0..in_matrix.len() {
        row_ptrs.push(next_index);
        for curr_col in 0..in_matrix[curr_row].len() {
            if in_matrix[curr_row][curr_col] != 0 {
                column_indices.push(curr_col);
                values.push(in_matrix[curr_row][curr_col]);
                next_index += 1;
            }
        }
    }
    (row_ptrs, column_indices, values)
}

fn main() {
    let a: Vec<Vec<i32>> = vec![vec![1, 2], vec![3, 4], vec![5, 6]];
    let (row_ptrs, column_indices, values) = compress_csr(&a);
    println!("{:?}", row_ptrs);
    println!("{:?}", column_indices);
    println!("{:?}", values)
}