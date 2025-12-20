
//#include <stdio.h>
#define LIST_LEN 4
#define KERNEL_LEN 2
#define RESULT_LEN (LIST_LEN - KERNEL_LEN + 1)

// Does convolution between a 1D list and a kernel
__attribute__((noinline))
void convolution(int list[], int kernel[], int result[]) {
    for (unsigned int i = 0; i < RESULT_LEN; i++) {
        int curr_result = 0;
        for (unsigned int j = 0; j < KERNEL_LEN; j++) {
            #ifdef CGRA_COMPILER
            please_map_me();
            #endif
            curr_result += list[i + j] * kernel[j];
        }
        result[i] = curr_result;
    }
}

int main() {
    int list[LIST_LEN] = {1, 2, 3, 4};
    int kernel[KERNEL_LEN] = {1, -2};
    int result[RESULT_LEN] = {0};
    convolution(list, kernel, result);
    /*for (int i = 0; i < RESULT_LEN; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");*/
}