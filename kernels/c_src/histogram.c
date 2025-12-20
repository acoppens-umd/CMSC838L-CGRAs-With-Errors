#include <stdio.h>

#define NUM_BINS 16
#define BIN_MASK (NUM_BINS - 1)

#define LENGTH 64

__attribute__((noinline))
void histogram_int(
    const int data[LENGTH],
    int hist[NUM_BINS],
) {
    int i;

    for (i = 0; i < LENGTH; i++) {
        #ifdef CGRA_COMPILER
        please_map_me();
        #endif
        int value = data[i];
        int bin = value & BIN_MASK;
        hist[bin] = hist[bin] + 1;
    }
}


int main(void)
{
    int data[LENGTH];
    int hist[NUM_BINS];

    int i;

    /* Initialize input data */
    for (i = 0; i < LENGTH; i++) {
        data[i] = i * 3;
    }

    /* Clear histogram */
    for (i = 0; i < NUM_BINS; i++) {
        hist[i] = 0;
    }

    /* Run histogram kernel */
    histogram_int(data, hist, LENGTH);

    /* Print histogram */
    for (i = 0; i < NUM_BINS; i++) {
        printf("bin[%2d] = %d\n", i, hist[i]);
    }

    return 0;
}

