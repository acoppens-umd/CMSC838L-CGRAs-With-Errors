
//#include <stdio.h>
#define M 4
#define K 5
#define N 6


// Conducts inner product matrix multiplication with dense matrices
__attribute__((noinline))
void inner_dense_matmul(int a[M][K], int b[K][N], int out[M][N]) {
    for (unsigned int curr_m = 0; curr_m < M; curr_m++) {
        for (unsigned int curr_n = 0; curr_n < N; curr_n++) {
            int curr_entry = 0;
            for (unsigned int curr_k = 0; curr_k < K; curr_k++) {
                #ifdef CGRA_COMPILER
                please_map_me();
                #endif
                curr_entry += a[curr_m][curr_k] * b[curr_k][curr_n];
            }
            out[curr_m][curr_n] = curr_entry;
        }
    }
}

int main() {
    int a[M][K] = {{1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20}};
    int b[K][N] = {{1,2,3,4,5,6},{7,8,9,10,11,12},{13,14,15,16,17,18},{19,20,21,22,23,24},{25,26,27,28,29,30}};
    int out[M][N] = {0};
    inner_dense_matmul(a, b, out);
    /*for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", out[i][j]);
        }
        printf("\n");
    }*/
}