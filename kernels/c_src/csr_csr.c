#include <string.h>
#include <stdlib.h>
#include <unistd.h>  
#include <pthread.h>
#include <stdint.h>
#include <dirent.h>

#define DATA_SIZE 128
#define M 63
#define N 15
#define K 63

int I_ROW_PTR[M];
int I_COL_VAL[DATA_SIZE];
int I_DATA[DATA_SIZE];

int W_ROW_PTR[N];
int W_COL_VAL[DATA_SIZE];
int W_DATA[DATA_SIZE];

int I[M*K];
int W[K*N];
int O[M*N];


//Apply an row-wise-like algorithm to multiply CSRxCSR->Dense 

__attribute__((noinline))
void spgemm(){
   int i;
   for (i = 0; i < M; i++) {
        // For each non-zero entry A[i][k]
        int I_idx;
        int I_end = I_ROW_PTR[i + 1];
        for (I_idx = I_ROW_PTR[i]; I_idx < I_end; I_idx++) {
            int k = I_COL_VAL[I_idx];
            int I_val = I_DATA[I_idx];

            int W_idx;
            int W_end = W_ROW_PTR[k + 1];
            // For each non-zero entry B[k][j]
            for (W_idx = W_ROW_PTR[k]; W_idx < W_end; W_idx++) {
               #ifdef CGRA_COMPILER
               please_map_me();
               #endif

               int j = W_COL_VAL[W_idx];
               int W_val = W_DATA[W_idx];

               // Accumulate into dense C[i][j]
               O[i * N + j] += I_val * W_val;
            }
        }
    }
}

void gen_matrices() {
   int i, j;

   for (i = 0; i < M; i++)
      for (j = 0; j < N; j++) {
         O[i*N+j]=  0;
      }
    
   for (i = 0; i < DATA_SIZE; i++) {
      int r = rand() % M;
      int c = rand() % K;
      I[r*K+c] = rand() / 134217728;
   }

   I_ROW_PTR[0] = 0;
   int nnzs = 0;
   for (i = 0; i < M; i++) {
      for (j = 0; j < K; j++) {
         if (I[i*K+j] != 0) {
            I_COL_VAL[nnzs] = j;
            I_DATA[nnzs] = I[i*K+j];

            nnzs++;
         }
      }
      
      I_ROW_PTR[i + 1] = nnzs;
   }

   for (i = 0; i < DATA_SIZE; i++) {
      int r = rand() % K;
      int c = rand() % N;
      W[r*N+c] = rand() / 134217728;
   }

   W_ROW_PTR[0] = 0;
   nnzs = 0;
   for (i = 0; i < N; i++) {
      for (j = 0; j < K; j++) {
         if (W[j*N+i] != 0) {
            W_COL_VAL[nnzs] = j;
            W_DATA[nnzs] = W[j*N+i];

            nnzs++;
         }
      }
      
      W_ROW_PTR[i + 1] = nnzs;
   }
}

void main(){
   gen_matrices();

   spgemm();
   
   int i,j;
   for (i=0;i<M; i++)
      for (j=0;j<N; j++) {
         //printf("%d\n", O[i*N+j]);
      }

}


