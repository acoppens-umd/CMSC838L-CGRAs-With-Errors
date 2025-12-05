use std::f64;


// Checks matrix dimensions for a single matrix
fn check_single_matrix_dimensions(in_mat: &Vec<Vec<i32>>) {
    let num_rows = in_mat.len();
    let num_cols = in_mat[0].len();
    for i in 0..num_rows {
        if in_mat[i].len() != num_cols {
            panic!("Matrix Dimensions Wrong");
        }
    }
}

// Checks matrix dimensions for matmul to make sure they are correct
fn check_matrix_dimensions(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) {
    let m_size: usize = a.len();
    let k_size: usize = b.len();
    let curr_k: usize  = a[0].len();
    let curr_n: usize  = b[0].len();
    for i in 0..m_size {
        if a[i].len() != curr_k {
            panic!("Matrix Multiplication Dimensions Wrong");
        }
    }
    for i in 0..k_size {
        if b[i].len() != curr_n {
            panic!("Matrix Multiplication Dimensions Wrong");
        }
    }
    if k_size != curr_k {
        panic!("Matrix Multiplication Dimensions Wrong");
    }
}

// Conducts inner product matrix multiplication with dense matrices
fn innermatmul(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    check_matrix_dimensions(a, b);
    let m_size: usize = a.len();
    let k_size: usize = b.len();
    let n_size: usize = b[0].len();
    let mut output_vec: Vec<Vec<i32>> = vec![vec![0; n_size]; m_size];
    for m in 0..m_size {
        for n in 0..n_size {
            let mut curr_entry: i32 = 0;
            for k in 0..k_size {
                curr_entry += a[m][k] * b[k][n];
            }
            output_vec[m][n] = curr_entry;
        }
    }
    output_vec
}

// Conducts outer product matrix multiplication with dense matrices
fn outermatmul(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    check_matrix_dimensions(a, b);
    let m_size: usize = a.len();
    let k_size: usize = b.len();
    let n_size: usize = b[0].len();
    let mut output_vec: Vec<Vec<i32>> = vec![vec![0; n_size]; m_size];
    for k in 0..k_size {
        for m in 0..m_size {
            for n in 0..n_size {
                output_vec[m][n] += a[m][k] * b[k][n];
            }
        }
    }
    output_vec
}

// Conducts Gustavson's matrix multiplication on dense matrices
fn gustmatmul(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    check_matrix_dimensions(a, b);
    let m_size: usize = a.len();
    let k_size: usize = b.len();
    let n_size: usize = b[0].len();
    let mut output_vec: Vec<Vec<i32>> = Vec::new();
    for m in 0..m_size {
        let mut curr_row: Vec<i32> = vec![0; n_size];
        for k in 0..k_size {
            for n in 0..n_size {
                curr_row[n] += a[m][k] * b[k][n];
            }
        }
        output_vec.push(curr_row);
    }
    output_vec
}

// Compresses a vector into CSR format (with row pointers, column indices, and values)
fn compress_csr(in_matrix: &Vec<Vec<i32>>) -> (Vec<usize>, Vec<usize>, Vec<i32>) {
    check_single_matrix_dimensions(in_matrix);
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

// Compresses a matrix into CSC format (with column pointers, row indices, and values)
fn compress_csc(in_matrix: &Vec<Vec<i32>>) -> (Vec<usize>, Vec<usize>, Vec<i32>) {
    let mut col_ptrs: Vec<usize> = Vec::new();
    let mut row_indices: Vec<usize> = Vec::new();
    let mut values: Vec<i32> = Vec::new();
    let mut next_index: usize = 0;
    for curr_col in 0..in_matrix[0].len() {
        col_ptrs.push(next_index);
        for curr_row in 0..in_matrix.len() {
            if in_matrix[curr_row][curr_col] != 0 {
                row_indices.push(curr_row);
                values.push(in_matrix[curr_row][curr_col]);
                next_index += 1;
            }
        }
    }
    (col_ptrs, row_indices, values)
}

// Conducts sparse matrix-vector multiplcation on a matrix in CSR format and a dense vector
fn csr_spmv(row_ptrs: &Vec<usize>, col_indices: &Vec<usize>, 
matrix_values: &Vec<i32>, mult_vector: &Vec<i32>) -> Vec<i32> {
    let mut final_vec: Vec<i32> = Vec::new();
    let prev_row: usize = 1;
    for curr_row in 0..row_ptrs.len() {
        let mut curr_result = 0;
        if row_ptrs[curr_row] != prev_row {
            //end index is the actual end index + 1 (for the for loop)
            let end_index: usize = if curr_row == row_ptrs.len() - 1 {
                col_indices.len()
            }
            else {
                row_ptrs[curr_row+1]
            };
            for i in row_ptrs[curr_row]..end_index {
                curr_result += matrix_values[i] * mult_vector[col_indices[i]];
            }
        }
        final_vec.push(curr_result);
    }
    final_vec
}

/* This function uses the Gauss-Sidel method to find an approximate solution to the linear
system of equations Ax=b, where the matrix A is represented as a 2D f64 vector and b is just
a f64 vector. An estimate of the solution x is then gradually refined through the method until
each iteration changes x by less than a tolerance or until a set maximum number of iterations
is reached. Note that the method is only guaranteed to converge when A is strictly diagonally
dominant, or when each diagonal value of A has higher absolute value than the sum of the
absolute values of the other entries in the row. */
fn gauss_sidel(a_matrix: &Vec<Vec<f64>>, b_vector: &Vec<f64>, x_guess: &Vec<f64>, tol: f64, max_it: i32) -> Vec<f64> {
    let n = b_vector.len();
    let mut result_vec: Vec<f64> = x_guess.clone();
    for _curr_iter in 0..max_it {
        let mut max_change = 0.0;
        for i in 0..n {
            let mut curr_sum = 0.0;
            for j in 0..n {
                if j != i {
                    curr_sum += a_matrix[i][j] * result_vec[j]
                }
            }
            let new_xi_val = (b_vector[i] - curr_sum) / a_matrix[i][i];
            let curr_difference = (new_xi_val - result_vec[i]).abs();
            if curr_difference > max_change {
                max_change = curr_difference
            }
            result_vec[i] = new_xi_val;
        }
        if max_change < tol {
            return result_vec
        }
    }
    return result_vec;
}

/* This function uses the Runge-Kutta method to find an approximate solution for y at
a given endpoint t_end, where we have a first-order differential equation taking y and
t, start points t0 and y0, and a step size h. The function iterates to find approximate
values for curr_t + h until we get to the final t value. All values here are of type f64. */
fn runge_kutta(t0: f64, y0: f64, t_end: f64, h: f64, diff_fn: fn(f64, f64) -> f64) -> f64 {
    let mut curr_t = t0;
    let mut curr_y = y0;
    while curr_t < t_end {
        let k1 = diff_fn(curr_t, curr_y);
        let k2 = diff_fn(curr_t + h / 2.0, curr_y + h * k1 / 2.0);
        let k3 = diff_fn(curr_t + h / 2.0, curr_y + h * k2 / 2.0);
        let k4 = diff_fn(curr_t + h, curr_y + h * k3);
        curr_y += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
        curr_t += h;
    }
    return curr_y;
}


fn main() {
    let a: Vec<Vec<i32>> = vec![vec![1, 2], vec![3, 4]];
    let b: Vec<Vec<i32>> = vec![vec![5, 6], vec![7, 8]];
    let innerresult: Vec<Vec<i32>> = innermatmul(&a, &b);
    let outerresult: Vec<Vec<i32>> = outermatmul(&a, &b);
    let gustresult: Vec<Vec<i32>> = gustmatmul(&a, &b);
    for i in 0..innerresult.len() {
        println!("{:?}", innerresult[i]);
    }
    for i in 0..innerresult.len() {
        println!("{:?}", outerresult[i]);
    }
    for i in 0..gustresult.len() {
        println!("{:?}", gustresult[i]);
    }
    let (row_a, col_a, val_a) = compress_csr(&a);
    let (row_b, col_b, val_b) = compress_csr(&b);
    let (csccol_a, cscrow_a, cscval_a) = compress_csc(&a);
    let (csccol_b, cscrow_b, cscval_b) = compress_csc(&b);
    println!("{:?}", row_a);
    println!("{:?}", col_a);
    println!("{:?}", val_a);
    println!("{:?}", row_b);
    println!("{:?}", col_b);
    println!("{:?}", val_b);

    println!("{:?}", csccol_a);
    println!("{:?}", cscrow_a);
    println!("{:?}", cscval_a);
    println!("{:?}", csccol_b);
    println!("{:?}", cscrow_b);
    println!("{:?}", cscval_b);

    let mult_vector: Vec<i32> = vec![4, 6];
    println!("{:?}", csr_spmv(&row_a, &col_a, &val_a, &mult_vector));
    println!("{:?}", csr_spmv(&row_b, &col_b, &val_b, &mult_vector));


    let gs_a: Vec<Vec<f64>> = vec![vec![7.0, 2.0, 3.0], vec![5.0, 8.0, 1.0], vec![2.0, 4.0, 9.0]];
    let gs_b: Vec<f64> = vec![2.0, 3.0, 4.0];
    let gs_guess: Vec<f64> = vec![0.0, 0.0, 0.0];
    let gs_result = gauss_sidel(&gs_a, &gs_b, &gs_guess, 0.01, 100);
    println!("{:?}", gs_result);

    let rk_fn = |t: f64, y: f64| -2.0 * y + t;
    println!("{:?}", runge_kutta(0.0, 10.0, 1.0, 0.01, rk_fn));

}
