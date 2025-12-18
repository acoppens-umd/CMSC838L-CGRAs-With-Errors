//#include <stdio.h>

#define NUM_ROWS 3
#define NUM_COLS 2
#define VALUES_SIZE 6

// Conducts sparse matrix-vector multiplcation on a matrix in CSR format and a dense vector
// Use compress_csr.rs function to compute CSR format from dense matrix
__attribute__((noinline))
void csr_spmv(unsigned int row_ptrs[NUM_ROWS], unsigned int col_indices[VALUES_SIZE], 
int matrix_values[VALUES_SIZE], int mult_vector[NUM_COLS], int out[NUM_ROWS]) {
    unsigned int prev_row = 1;
    for (unsigned int curr_row = 0; curr_row < NUM_ROWS; curr_row++) {
        int curr_result = 0;
        if (row_ptrs[curr_row] != prev_row) {
            //end index is the actual end index + 1 (for the for loop)
            unsigned int end_index;
            if (curr_row == NUM_ROWS - 1) {
                end_index = VALUES_SIZE;
            }
            else {
                end_index = row_ptrs[curr_row + 1];
            }
            for (unsigned int i = row_ptrs[curr_row]; i < end_index; i++) {
                #ifdef CGRA_COMPILER
                please_map_me();
                #endif
                curr_result += matrix_values[i] * mult_vector[col_indices[i]];
            }
        }
        out[curr_row] = curr_result;
    }
}

int main() {
    unsigned int row[NUM_ROWS] = {0, 2, 4};
    unsigned int col[VALUES_SIZE] = {0, 1, 0, 1, 0, 1};
    int val[VALUES_SIZE] = {1, 2, 3, 4, 5, 6};
    int mult_vector[NUM_COLS] = {2, 3};
    int out[NUM_ROWS] = {0};
    csr_spmv(row, col, val, mult_vector, out);
    /*for (int i = 0; i < NUM_ROWS; i++) {
        printf("%d ", out[i]);
    }
    printf("\n");*/
}



