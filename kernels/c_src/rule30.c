#include <stdio.h>

#define LENGTH 32
#define STEPS  16

__attribute__((noinline))
void ca_step_1d(
    const int *cur,
    int *next,
    int length
) {
    int i;

    /* Boundary cells are handled externally */
    for (i = 1; i < length - 1; i++) {
        #ifdef CGRA_COMPILER
        please_map_me();
        #endif
        int left   = cur[i - 1];
        int center = cur[i];
        int right  = cur[i + 1];

        /* Rule 30: left XOR (center OR right) */
        next[i] = left ^ (center | right);
    }
}

int main(void)
{
    int cur[LENGTH];
    int next[LENGTH];

    int i, step;

    /* Initialize state: single 1 in the center */
    for (i = 0; i < LENGTH; i++) {
        cur[i] = 0;
        next[i] = 0;
    }
    cur[LENGTH / 2] = 1;

    /* Run automaton */
    for (step = 0; step < STEPS; step++) {

        /* Print current state */
        for (i = 0; i < LENGTH; i++) {
            printf("%c", cur[i] ? '#' : '.');
        }
        printf("\n");

        /* Apply kernel */
        ca_step_1d(cur, next, LENGTH);

        /* Swap buffers */
        for (i = 0; i < LENGTH; i++) {
            cur[i] = next[i];
            next[i] = 0;
        }
    }

    return 0;
}