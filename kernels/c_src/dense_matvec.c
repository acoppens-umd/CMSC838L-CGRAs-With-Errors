
//#include <stdio.h>
#define M 3
#define K 2

// Does dense matrix-vector multiplication.
__attribute__((noinline))
void dense_matvec(int matrix[M][K], int vector[K], int out[M]) {
    for (unsigned int curr_m = 0; curr_m < M; curr_m++) {
        for (unsigned int curr_k = 0; curr_k < K; curr_k++) {
            #ifdef CGRA_COMPILER
            please_map_me();
            #endif
            out[curr_m] += matrix[curr_m][curr_k] * vector[curr_k];
        }
    }
}

int main() {
    int matrix[M][K] = {{1,2},{3,4},{5,6}};
    int vector[K] = {2,3};
    int out[M] = {0};
    dense_matvec(matrix, vector, out);
    /*for (int i = 0; i < M; i++) {
        printf("%d ", out[i]);
    }
    printf("\n");*/
}